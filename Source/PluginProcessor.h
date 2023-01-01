/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <fstream>

using namespace std;

//==============================================================================
/**
*/
class AudioFifoTestAudioProcessor  : public juce::AudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    AudioFifoTestAudioProcessor();
    ~AudioFifoTestAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    void setDryWet(int percentage);
    int getDryWet();

    void setDelayMs(int ms);
    int getDelayMs();

    void setFeedback(int ms);
    int getFeedback();

private:
    int dryWet = 0;
    int delayMs = 20;
    int feedback = 1;
    //static constexpr auto fftOrder = 10;
    //static constexpr auto fftSize = 1 << fftOrder;
    float delaySamples = 0;
    float fadingFactor = 0.8;
    float dryGain = 0.9;
    float wetGain = 0;

    vector<vector<queue<float>>> delayBuffers;
    void pushSample(float sample, int channel, int repeatVoice);
    float readSample(int channel, int repeatVoice);
    juce::Logger* logger = juce::Logger::getCurrentLogger();
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioFifoTestAudioProcessor)
};
