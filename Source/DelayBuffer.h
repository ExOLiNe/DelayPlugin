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
#include "SlicedArray.h"

using namespace juce;
using namespace std;

class DelayBuffer
{
public:
    DelayBuffer(int maxSampleDelay, int sampleDelay, int voices);
    void pushSamples(float* samples, int numSamples);
    void fillSamples(float value, int numSamples);
    SlicedArray readSamples(int numSamples, int voice);
    void setDelaySamples(int newSampleDelay);
    void setStaticDelay(int delay);
private:
    const int bufSize;
    int sampleDelay;
    int staticSampleDelay = 0;
    float* buffer;
    CircularIterator currWritePos;
    vector<CircularIterator> readPositions;
};