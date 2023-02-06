/*
  ==============================================================================

    Settings.cpp
    Created: 5 Feb 2023 7:55:31am
    Author:  Aleksandr

  ==============================================================================
*/

#include "Settings.h"

namespace settings
{
    const juce::String stereoModeID("STEREO_MODE");
    const juce::String monoModeID("MONO_MODE");
    const juce::String pingPongModeID("PINGPONG_MODE");

    const KnobSettings<int> feedback{0, 100, 0, "%"};
    const KnobSettings<int> mix{ 0, 100, 0, "%" };
    const KnobSettings<int> hiPassFilter{ 20, 20'000, 20, "hz" };
    const KnobSettings<int> lowPassFilter{ 20, 20'000, 20'000, "hz" };
    const KnobSettings<int> delay{ 20, 1000, 300, "ms" };
}