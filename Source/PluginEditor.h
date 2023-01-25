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

enum DelayTimeMode
{
    MS,
    BPM
};

//==============================================================================
/**
*/
class AudioFifoTestAudioProcessorEditor  : public juce::AudioProcessorEditor,
    public Slider::Listener, public ComboBox::Listener, public Button::Listener
{
public:
    AudioFifoTestAudioProcessorEditor (AudioFifoTestAudioProcessor&);
    ~AudioFifoTestAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void sliderValueChanged(Slider* slider) override;
    void comboBoxChanged(ComboBox* comboBoxThatHasChanged) override;
    void setDelayTimeMode(DelayTimeMode mode);
    void buttonStateChanged(Button*);
    void buttonClicked(Button*);
private:

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AudioFifoTestAudioProcessor& audioProcessor;

    vector<int> bpmMappings;

    Slider dryWetSlider;
    unique_ptr<AudioProcessorValueTreeState::SliderAttachment> dryWetSliderValue;

    Slider delaySlider;
    Slider feedbackSlider;
    Slider lowpassSlider;
    Slider highpassSlider;
    ComboBox modeBox;
    ToggleButton tiedToBpmButton;

    DelayTimeMode delayTimeMode = DelayTimeMode::MS;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioFifoTestAudioProcessorEditor)
};
