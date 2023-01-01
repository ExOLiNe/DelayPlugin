/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioFifoTestAudioProcessor::AudioFifoTestAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

AudioFifoTestAudioProcessor::~AudioFifoTestAudioProcessor()
{
}

//==============================================================================
const juce::String AudioFifoTestAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AudioFifoTestAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool AudioFifoTestAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool AudioFifoTestAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double AudioFifoTestAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AudioFifoTestAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int AudioFifoTestAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AudioFifoTestAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String AudioFifoTestAudioProcessor::getProgramName (int index)
{
    return {};
}

void AudioFifoTestAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void AudioFifoTestAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    delaySamples = (float)delayMs / 1000 * getSampleRate();

    for (int voice = 0; voice < feedback; voice++)
    {
        vector<queue<float>> q;
        q.push_back(queue<float>());
        q.push_back(queue<float>());
        delayBuffers.push_back(q);

        for (int j = 0; j < getNumInputChannels(); j++)
        {
            for (int i = 0; i < delaySamples * (voice + 1); i++)
            {
                pushSample(0, j, voice);
            }
        }
    }
}

void AudioFifoTestAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool AudioFifoTestAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void AudioFifoTestAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    for (int j = 0; j < buffer.getNumChannels(); j++)
    {
        auto* channelData = buffer.getWritePointer(j);

        for (auto i = 0; i < buffer.getNumSamples(); ++i)
        {
            for (auto repeat = 0; repeat < feedback; repeat++)
            {
                pushSample(channelData[i], j, repeat);
            }
        }

        for (auto i = 0; i < buffer.getNumSamples(); ++i)
        {
            channelData[i] = channelData[i] * dryGain;
            for (auto repeat = 0; repeat < feedback; repeat++)
            {
                channelData[i] += readSample(j, repeat) * wetGain * pow(fadingFactor, repeat);
            }
        }
    }
}

void AudioFifoTestAudioProcessor::setDryWet(int percentage)
{
    dryGain = (float)percentage / 100 * 0.9;
    wetGain = (float)(100 - percentage) / 100 * 0.9;
}

int AudioFifoTestAudioProcessor::getDryWet()
{
    return dryWet;
}

void AudioFifoTestAudioProcessor::setDelayMs(int ms)
{
    delaySamples = (float)delayMs / 1000 * getSampleRate();
}

int AudioFifoTestAudioProcessor::getDelayMs()
{
    return delayMs;
}

void AudioFifoTestAudioProcessor::setFeedback(int ms)
{
    feedback = ms;
}
int AudioFifoTestAudioProcessor::getFeedback()
{
    return feedback;
}

void AudioFifoTestAudioProcessor::pushSample(float sample, int channel, int repeatVoice)
{
    delayBuffers[repeatVoice][channel].push(sample);
}

float AudioFifoTestAudioProcessor::readSample(int channel, int repeatVoice)
{
    auto sample = delayBuffers[repeatVoice][channel].front();
    delayBuffers[repeatVoice][channel].pop();
    return sample;
}

//==============================================================================
bool AudioFifoTestAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* AudioFifoTestAudioProcessor::createEditor()
{
    return new AudioFifoTestAudioProcessorEditor (*this);
}

//==============================================================================
void AudioFifoTestAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void AudioFifoTestAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AudioFifoTestAudioProcessor();
}
