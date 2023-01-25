/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DelayBuffer.h"

using namespace std;
using namespace juce;
//using namespace juce::dsp;

#define MAX_SAMPLE_DELAY_MS 1000
#define MAX_VOICES 16

using Filter = dsp::IIR::Filter<float>;
using Chain = dsp::ProcessorChain<Filter, Filter, dsp::Compressor<float>>;

#define DRY_WET_ID "drywet"

enum Mode
{
    STEREO,
    MONO,
    PING_PONG
};

enum
{
    lowPass,
    highPass,
    compressor
};

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

    //==============================================================================
    bool acceptsMidi() const;
    bool producesMidi() const;
    double getTailLengthSeconds() const;
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    //void setDryWet(int percentage);
    //int getDryWet();

    void setDelayMs(int ms);
    int getDelayMs();

    void setFeedback(float ms);
    float getFeedback();

    void setLowpassFrequency(int freq);
    int getLowpassFrequency();

    void setHighpassFrequency(int freq);
    int getHighpassFrequency();

    void setMode(Mode mode);
    Mode getMode();

    void setDuration(int divider);
    void setBpmTied(bool tied);

    AudioProcessorValueTreeState treeState;

private:
    //UI params
    //int dryWet = 90;
    int delayMs = 200;
    float fadingFactor = 0.8;
    Mode mode;
    int durationDivider;
    bool tiedToBpm = false;
    //=============================================================================

    int compressionRatio = 1;
    float compressionThreshold = 0.005;
    float compressionAttack = 10;
    float compressionRelease = 200;

    int voices = MAX_VOICES;
    float delaySamples = 0;
    //float dryGain = 0.8;
    //float wetGain = 0.4;

    double bpm = 0;

    void updateChains();
    void setDelaySamples(int delaySamples);
    AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    vector<DelayBuffer> delayBuffers;
    int lowpassFrequency = 2000;
    int highpassFrequency = 600;
    AudioBuffer<float> postProcessBuffer;
    vector<Chain> chains;

    vector<SlicedArray> voiceIterators;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioFifoTestAudioProcessor)
};
