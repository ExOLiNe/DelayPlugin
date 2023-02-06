/*
  ==============================================================================

    Settings.h
    Created: 5 Feb 2023 7:55:31am
    Author:  Aleksandr

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

template <typename T>
struct KnobSettings {
public:
    KnobSettings(T minValue, T maxValue, T defaultValue, const char* postfix) :
        minValue(minValue), maxValue(maxValue),
        defaultValue(defaultValue), postfix(postfix) {}
    const T minValue;
    const T maxValue;
    const T defaultValue;
    const char* postfix;
};

namespace settings
{
    const extern juce::String stereoModeID, monoModeID, pingPongModeID;
    const extern KnobSettings<int> feedback, mix, hiPassFilter, lowPassFilter, delay;
}