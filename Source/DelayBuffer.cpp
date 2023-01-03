/*
  ==============================================================================

    DelayBuffer.cpp
    Created: 31 Dec 2022 9:06:55pm
    Author:  Aleksandr

  ==============================================================================
*/

#include "DelayBuffer.h"
#include <iterator>
#include <algorithm>
#include <format>

#define RESERVED (1 << 12)

DelayBuffer::DelayBuffer(int maxSampleDelay, int sampleDelay, int voices) :
    bufSize(maxSampleDelay * voices + RESERVED),
    sampleDelay(sampleDelay),
    buffer(new float[bufSize]),
    currWritePos(CircularIterator(buffer, bufSize, 0))
{
    for (int voice = 0; voice < voices; voice++)
    {
        readPositions.push_back(CircularIterator(buffer, bufSize, (voice + 1) * sampleDelay));
    }
}

void DelayBuffer::pushSamples(float* samples, int numSamples)
{
    auto pos = currWritePos.getValue();
    if (pos + numSamples <= bufSize)
    {
        copy(samples, samples + numSamples, buffer + pos);
    }
    else
    {
        Logger::getCurrentLogger()->writeToLog("buffer push exception");
        auto toEnd = bufSize - pos;
        copy(samples, samples + toEnd, buffer + pos);

        copy(samples + toEnd, samples + numSamples - toEnd, buffer);
    }
    currWritePos += numSamples;
}
void DelayBuffer::fillSamples(float value, int numSamples)
{
    memset(buffer, value, bufSize * sizeof(float));
    currWritePos = numSamples;
}
SlicedArray DelayBuffer::readSamples(int numSamples, int voice)
{
    auto pos = readPositions[voice].getValue();
    readPositions[voice] += numSamples;
    return SlicedArray(buffer, pos, bufSize);
}
void DelayBuffer::setDelaySamples(int newSampleDelay)
{
    this->sampleDelay = newSampleDelay;
    for (int i = 0; i < readPositions.size(); i++)
    {
        readPositions[i].setPos(currWritePos.getValue() - newSampleDelay * (i + 1) - staticSampleDelay);
    }
}

void DelayBuffer::setStaticDelay(int delay)
{
    staticSampleDelay = delay;
    setDelaySamples(sampleDelay);
}