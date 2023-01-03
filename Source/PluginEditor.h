/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "ChannelMixingWritePointer.h"

using namespace juce;

//==============================================================================
/**
*/
class AudioFifoTestAudioProcessorEditor  : public juce::AudioProcessorEditor,
    public Slider::Listener, public ComboBox::Listener
{
public:
    AudioFifoTestAudioProcessorEditor (AudioFifoTestAudioProcessor&);
    ~AudioFifoTestAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void sliderValueChanged(Slider* slider) override;
    void comboBoxChanged(ComboBox* comboBoxThatHasChanged) override;
private:

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AudioFifoTestAudioProcessor& audioProcessor;

    Slider dryWetSlider;
    Slider delaySlider;
    Slider feedbackSlider;
    Slider lowpassSlider;
    Slider highpassSlider;
    ComboBox modeBox;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioFifoTestAudioProcessorEditor)
};
