/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "ChannelMixingWritePointer.h"
#include "MyLookAndFeel.h"
#include "LightKnobLookAndFeel.h"
#include "Settings.h"

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
    enum GroupStyle {
        SOLID, BORDERED
    };
    void drawGroup(juce::Graphics&, juce::Rectangle<float> rect, GroupStyle style);
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AudioFifoTestAudioProcessor& audioProcessor;

    vector<int> bpmMappings;

    /*Slider dryWetSlider;
    unique_ptr<AudioProcessorValueTreeState::SliderAttachment> dryWetSliderValue;

    Slider delaySlider;
    Slider feedbackSlider;
    Slider lowpassSlider;
    Slider highpassSlider;
    ComboBox modeBox;
    ToggleButton tiedToBpmButton;*/
    MyLookAndFeel myLookAndFeel;
    LightKnobLookAndFeel lightKnobLookAndFeel;
    juce::Label titleLabel, versionLabel, siteLabel, presetsLabel, delayModeLabel;
    juce::ComboBox presets;
    juce::TextButton analogButton, stereoModeButton, monoModeButton, pingPongModeButton;

    juce::ImageComponent logo;

    //KnobComponent<Style::SIMPLE, int> feedbackKnob, mixKnob, hiPassKnob, lowPassKnob;
    //KnobComponent<Style::LIGHT, int> delayKnob;
    juce::Slider feedbackKnob, mixKnob, hiPassKnob, lowPassKnob, delayKnob;

    std::unique_ptr<juce::Drawable> background;

    //juce::GroupComponent leftGroup, centerGroup, rightGroup;

    int border = 10;

    DelayTimeMode delayTimeMode = DelayTimeMode::MS;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioFifoTestAudioProcessorEditor)
};
