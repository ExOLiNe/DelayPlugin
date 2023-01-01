/*
  ==============================================================================

    DelayBuffer.h
    Created: 31 Dec 2022 9:06:55pm
    Author:  Aleksandr

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include <vector>
#include "CircularIterator.h"

using namespace juce;
using namespace std;

class DelayBuffer
{
public:
    DelayBuffer(int size);
    void pushSamples(float* samples, int numSamples);
    SampleIterator readSamples(int numSamples);
    void setDelaySamples(int samples);
private:
    float* buffer;
    int bufSize = 0;
    int currWritePos = 0;
    int currReadPos = 0;
};