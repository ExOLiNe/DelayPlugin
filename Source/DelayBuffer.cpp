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
    buffer(vector<float>(bufSize)),
    currWritePos(CircularIterator(bufSize, 0))
{
    for (int voice = 0; voice < voices; voice++)
    {
        readPositions.push_back(CircularIterator(bufSize, (voice + 1) * sampleDelay));
    }
}

DelayBuffer::~DelayBuffer()
{
}

void DelayBuffer::pushSamples(const float& samples, int numSamples)
{
    auto pos = currWritePos.getValue();
    if (pos + numSamples <= bufSize)
    {
        copy(&samples, &samples + numSamples, buffer.begin() + pos);
    }
    else
    {
        auto toEnd = bufSize - pos;
        copy(&samples, &samples + toEnd, buffer.begin() + pos);

        copy(&samples + toEnd, &samples + numSamples, buffer.begin());
    }
    currWritePos += numSamples;
}
void DelayBuffer::fillSamples(float value, int numSamples)
{
    memset(&buffer[0], value, bufSize * sizeof(float));
    currWritePos = numSamples;
}
SlicedArray DelayBuffer::readSamples(int numSamples, int voice)
{
    auto pos = readPositions[voice].getValue();
    readPositions[voice] += numSamples;
    return SlicedArray(buffer[0], pos, bufSize);
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