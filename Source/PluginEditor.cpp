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

    bpmMappings.push_back(32);
    bpmMappings.push_back(24);
    bpmMappings.push_back(16);
    bpmMappings.push_back(12);
    bpmMappings.push_back(8);
    bpmMappings.push_back(6);
    bpmMappings.push_back(4);
    bpmMappings.push_back(3);
    bpmMappings.push_back(2);
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (600, 400);

    dryWetSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    dryWetSlider.setTextValueSuffix(" %");
    dryWetSlider.setRange(0, 100, 1);
    dryWetSlider.addListener(this);
    dryWetSlider.setValue(audioProcessor.getDryWet());
    addAndMakeVisible(dryWetSlider);

    delaySlider.setSliderStyle(Slider::SliderStyle::Rotary);
    delaySlider.setTextValueSuffix(" ms");
    delaySlider.setRange(50, 1200, 1);
    delaySlider.addListener(this);
    delaySlider.setValue(audioProcessor.getDelayMs());
    addAndMakeVisible(delaySlider);

    feedbackSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    feedbackSlider.setTextValueSuffix(" times");
    feedbackSlider.setRange(0.4, 1, 0.01f);
    feedbackSlider.setValue(audioProcessor.getFeedback());
    feedbackSlider.addListener(this);
    addAndMakeVisible(feedbackSlider);

    modeBox.addItem("stereo", 1);
    modeBox.addItem("mono", 2);
    modeBox.addItem("ping pong", 3);
    modeBox.setSize(100, 20);
    modeBox.addListener(this);
    addAndMakeVisible(modeBox);

    lowpassSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    lowpassSlider.setTextValueSuffix(" hz");
    lowpassSlider.setRange(2000.0f, 20000.0f, 1.0f);
    lowpassSlider.setValue(audioProcessor.getLowpassFrequency());
    lowpassSlider.addListener(this);
    addAndMakeVisible(lowpassSlider);

    highpassSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    highpassSlider.setTextValueSuffix(" hz");
    highpassSlider.setRange(20.0f, 2000.0f, 1.0f);
    highpassSlider.setValue(audioProcessor.getHighpassFrequency());
    highpassSlider.addListener(this);
    addAndMakeVisible(highpassSlider);

    tiedToBpmButton.addListener(this);
    addAndMakeVisible(tiedToBpmButton);
}

AudioFifoTestAudioProcessorEditor::~AudioFifoTestAudioProcessorEditor()
{
}

void AudioFifoTestAudioProcessorEditor::comboBoxChanged(ComboBox* comboBoxThatHasChanged)
{
    switch (comboBoxThatHasChanged->getSelectedId())
    {
    case 1:
        audioProcessor.setMode(Mode::STEREO);
        break;
    case 2:
        audioProcessor.setMode(Mode::MONO);
        break;
    case 3:
        audioProcessor.setMode(Mode::PING_PONG);
        break;
    }
}

void AudioFifoTestAudioProcessorEditor::sliderValueChanged(Slider* slider)
{
    if (slider == &delaySlider) 
    {
        if (delayTimeMode == DelayTimeMode::BPM)
        {
            audioProcessor.setDuration(bpmMappings[slider->getValue()]);
        }
        else
        {
            audioProcessor.setDelayMs(slider->getValue());
        }
    }
    else if (slider == &feedbackSlider)
    {
        audioProcessor.setFeedback(slider->getValue());
    }
    else if (slider == &dryWetSlider)
    {
        audioProcessor.setDryWet(slider->getValue()) ;
    }
    else if (slider == &lowpassSlider)
    {
        audioProcessor.setLowpassFrequency(slider->getValue());
    }
    else if (slider == &highpassSlider)
    {
        audioProcessor.setHighpassFrequency(slider->getValue());
    }
}

void AudioFifoTestAudioProcessorEditor::setDelayTimeMode(DelayTimeMode mode)
{
    this->delayTimeMode = mode;
}

void AudioFifoTestAudioProcessorEditor::buttonStateChanged(Button* button)
{
    auto state = button->getToggleState();
    if (state)
    {
        delaySlider.setTextValueSuffix("");
        delaySlider.setRange(0, bpmMappings.size() - 1, 1.0f);
        delayTimeMode = DelayTimeMode::BPM;
        audioProcessor.setBpmTied(state);
    }
    else
    {
        delaySlider.setTextValueSuffix(" ms");
        delaySlider.setRange(50, 1200, 1);
        delayTimeMode = DelayTimeMode::MS;
    }
}

void AudioFifoTestAudioProcessorEditor::buttonClicked(Button*)
{

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
    dryWetSlider.setBounds(
        getLocalBounds().removeFromBottom(getHeight() / 2).removeFromLeft(getWidth() / 3));
    delaySlider.setBounds(
        getLocalBounds().removeFromTop(getHeight() / 2).removeFromLeft(getWidth() / 3));
    feedbackSlider.setBounds(
        getLocalBounds().removeFromTop(getHeight() / 2).removeFromRight(getWidth() / 3));
    modeBox.setBounds(
        getLocalBounds().removeFromBottom(getHeight() / 2).removeFromRight(getWidth() / 3));
    lowpassSlider.setBounds(
        getLocalBounds().removeFromBottom(getHeight() / 2).removeFromLeft(getWidth() / 3 * 2).removeFromRight(getWidth() / 7 * 2));
    highpassSlider.setBounds(
        getLocalBounds().removeFromTop(getHeight() / 2).removeFromLeft(getWidth() / 3 * 2).removeFromRight(getWidth() / 5 * 2));
    tiedToBpmButton.setBounds(
        getLocalBounds().removeFromBottom(getHeight() / 3).removeFromRight(getWidth() / 3));
}
