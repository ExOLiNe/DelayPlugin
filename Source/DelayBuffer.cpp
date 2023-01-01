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

DelayBuffer::DelayBuffer(int size)
{
    bufSize = size;
    buffer = new float[size];
}

void DelayBuffer::pushSamples(float* samples, int numSamples)
{
    if (currWritePos + numSamples <= bufSize)
    {
        memcpy(&buffer + currWritePos, samples, numSamples);
        currWritePos += numSamples;
    }
    else
    {
        auto toEnd = bufSize - currWritePos;
        memcpy(&buffer + currWritePos, samples, toEnd);

        memcpy(&buffer, samples + toEnd, numSamples);
        currWritePos = numSamples - toEnd;
    }
}
SampleIterator DelayBuffer::readSamples(int numSamples)
{
    if (currReadPos + numSamples <= bufSize)
    {
        currReadPos += numSamples;
        return SampleIterator(buffer + currReadPos, bufSize - currReadPos, nullptr, 0, 0);
    }
    else
    {
        auto toEnd = bufSize - currReadPos;
        return SampleIterator(buffer + currReadPos, toEnd, buffer, numSamples - toEnd);
    }
}
void DelayBuffer::setDelaySamples(int samples)
{
    for (int i = 0; i < samples; i++)
    {
        memset(buffer, 0, sizeof(buffer));
        currReadPos = 0;
        currWritePos = samples;
    }
}