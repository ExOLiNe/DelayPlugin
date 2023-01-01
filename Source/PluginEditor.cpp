/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioFifoTestAudioProcessorEditor::AudioFifoTestAudioProcessorEditor (AudioFifoTestAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);

    dryWetSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    dryWetSlider.setTextValueSuffix(" %");
    dryWetSlider.setRange(0, 100, 1);
    dryWetSlider.addListener(this);
    addAndMakeVisible(dryWetSlider);

    delaySlider.setSliderStyle(Slider::SliderStyle::Rotary);
    delaySlider.setTextValueSuffix(" ms");
    delaySlider.setRange(50, 1200, 1);
    delaySlider.addListener(this);
    addAndMakeVisible(delaySlider);

    feedbackSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    feedbackSlider.setTextValueSuffix(" times");
    feedbackSlider.setRange(1, 15, 1);
    feedbackSlider.addListener(this);
    addAndMakeVisible(feedbackSlider);
}

AudioFifoTestAudioProcessorEditor::~AudioFifoTestAudioProcessorEditor()
{
}

void AudioFifoTestAudioProcessorEditor::sliderValueChanged(Slider* slider)
{
    if (slider == &delaySlider) 
    {
        audioProcessor.setDelayMs(slider->getValue());
    }
    else if (slider == &feedbackSlider)
    {
        audioProcessor.setFeedback(slider->getValue());
    }
    else if (slider == &dryWetSlider)
    {
        audioProcessor.setDryWet(slider->getValue()) ;
    }
}

//==============================================================================
void AudioFifoTestAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (Colours::black);
    g.setColour(Colours::black);
}

void AudioFifoTestAudioProcessorEditor::resized()
{
    dryWetSlider.setBounds(getLocalBounds().removeFromBottom(getHeight() / 2).removeFromLeft(getWidth() / 2));
    delaySlider.setBounds(getLocalBounds().removeFromTop(getHeight() / 2).removeFromLeft(getWidth() / 2));
    feedbackSlider.setBounds(getLocalBounds().removeFromTop(getHeight() / 2).removeFromRight(getWidth() / 2));
}
