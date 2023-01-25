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
                       ), chains(vector<Chain>(getNumInputChannels()))
    , treeState(*this, nullptr, "PARAMETERS", createParameterLayout())
                        
#endif
{
}

AudioFifoTestAudioProcessor::~AudioFifoTestAudioProcessor()
{
}

AudioProcessorValueTreeState::ParameterLayout AudioFifoTestAudioProcessor::createParameterLayout()
{
    vector<unique_ptr<RangedAudioParameter>> params;

    params.emplace_back(make_unique<AudioParameterInt>(DRY_WET_ID, "drywet", 0.0f, 100.f, 30.0f));
    return { params.begin(), params.end() };
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
    voiceIterators.resize(voices);
    
    dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();

    delaySamples = (float)delayMs / 1000 * getSampleRate();
    auto maxSampleDelayMs = MAX_SAMPLE_DELAY_MS / 1000 * getSampleRate();
    auto some = maxSampleDelayMs * MAX_VOICES + (1 << 12);


    for (int channel = 0; channel < getNumInputChannels(); channel++)
    {
        chains[channel].prepare(spec);
        chains[channel].get<lowPass>().coefficients = dsp::IIR::Coefficients<float>::makeLowPass(sampleRate, 4000.0f);
        chains[channel].get<highPass>().coefficients = dsp::IIR::Coefficients<float>::makeHighPass(sampleRate, 800.0f);
        chains[channel].get<compressor>().setRatio(compressionRatio);
        chains[channel].get<compressor>().setThreshold(compressionThreshold);
        chains[channel].get<compressor>().setAttack(compressionAttack);
        chains[channel].get<compressor>().setRelease(compressionRelease);

        auto buffer = DelayBuffer(maxSampleDelayMs, delaySamples, MAX_VOICES);
        buffer.fillSamples(0, delaySamples);
        delayBuffers.push_back(move(buffer));
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
    auto bpm = (*getPlayHead()->getPosition()).getBpm();
    if (bpm.hasValue())
    {
        this->bpm = *bpm;
    }

    postProcessBuffer.setSize(buffer.getNumChannels(), buffer.getNumSamples());
    postProcessBuffer.clear();


    if (mode == Mode::PING_PONG)
    {
        delayBuffers[0].setStaticDelay(delaySamples / 2);
    }
    else
    {
        delayBuffers[0].setStaticDelay(0);
    }

    auto wetGain = (float) treeState.getRawParameterValue(DRY_WET_ID)->load() / 100;
    auto dryGain = (float) (100 - treeState.getRawParameterValue(DRY_WET_ID)->load()) / 100;

    //Write real-time signal to delayed buffer and sum all voices
    for (int channel = 0; channel < buffer.getNumChannels(); channel++)
    {
        auto* channelData = buffer.getWritePointer(channel);
        auto* postProcessChannelData = postProcessBuffer.getWritePointer(channel);

        delayBuffers[channel].pushSamples(*channelData, buffer.getNumSamples());

        for (auto voice = 0; voice < voices; voice++)
        {
            //TODO - why is this shit copying??
            voiceIterators[voice] = move(delayBuffers[channel].readSamples(buffer.getNumSamples(), voice));
        }

        for (auto i = 0; i < buffer.getNumSamples(); ++i)
        {
            for (auto voice = 0; voice < voices; voice++)
            {
                postProcessChannelData[i] += 0.05f * voiceIterators[voice][i] * wetGain * pow(fadingFactor, voice);
            }
        }
    }

    //Delayed voices post processing
    dsp::AudioBlock<float> block(postProcessBuffer);
    for (int channel = 0; channel < postProcessBuffer.getNumChannels(); ++channel)
    {
        auto channelBlock = block.getSingleChannelBlock(channel);
        chains[channel].process(dsp::ProcessContextReplacing<float>(channelBlock));
    }

    //Wrap read pointers per channel to one entity for choosing between stereo and mono
    auto postProcessedChannels = vector<const float*>(postProcessBuffer.getNumChannels());
    for (int channel = 0; channel < postProcessBuffer.getNumChannels(); ++channel)
    {
        postProcessedChannels[channel] = postProcessBuffer.getReadPointer(channel);
    }
    auto postProcessedChannelsPointers = ChannelMixingWritePointer(
        postProcessedChannels, mode == Mode::MONO);

    //Mixing dry and wet signals together
    for (int channel = 0; channel < buffer.getNumChannels(); channel++)
    {
        auto* channelData = buffer.getWritePointer(channel);
        for (auto i = 0; i < buffer.getNumSamples(); ++i)
        {
            channelData[i] = channelData[i] * dryGain;
            for (auto voice = 0; voice < voices; voice++)
            {
                channelData[i] += postProcessedChannelsPointers.get(channel, i);
            }
        }
    }

    //Update chain params from editor
    updateChains();
}

void AudioFifoTestAudioProcessor::updateChains()
{
    for (int channel = 0; channel < chains.size(); ++channel)
    {
        chains[channel].get<lowPass>().coefficients = dsp::IIR::Coefficients<float>::makeLowPass(getSampleRate(),
            lowpassFrequency);
        chains[channel].get<highPass>().coefficients = dsp::IIR::Coefficients<float>::makeHighPass(getSampleRate(),
            highpassFrequency);
    }
}

void AudioFifoTestAudioProcessor::setMode(Mode mode)
{
    this->mode = mode;
}
Mode AudioFifoTestAudioProcessor::getMode()
{
    return this->mode;
}

void AudioFifoTestAudioProcessor::setBpmTied(bool tied)
{
    this->tiedToBpm = tied;
}

/*void AudioFifoTestAudioProcessor::setDryWet(int percentage)
{
    wetGain = (float)percentage / 100 * 0.9;
    dryGain = (float)(100 - percentage) / 100 * 0.9;
}

int AudioFifoTestAudioProcessor::getDryWet()
{
    return dryWet;
}*/

void AudioFifoTestAudioProcessor::setDelaySamples(int delaySamples)
{
    for (int channel = 0; channel < getNumInputChannels(); ++channel)
    {
        delayBuffers[channel].setDelaySamples(delaySamples);
    }
}

void AudioFifoTestAudioProcessor::setDelayMs(int ms)
{
    setDelaySamples((double)ms * getSampleRate() / 1000);
}

void AudioFifoTestAudioProcessor::setDuration(int divider)
{
    this->durationDivider = divider;
    if (bpm > 0)
    {
        setDelaySamples(getSampleRate() / (bpm / 4 / 60 * durationDivider));
    }
}

int AudioFifoTestAudioProcessor::getDelayMs()
{
    return delayMs;
}

void AudioFifoTestAudioProcessor::setFeedback(float ms)
{
    fadingFactor = ms;
}
float AudioFifoTestAudioProcessor::getFeedback()
{
    return fadingFactor;
}

void AudioFifoTestAudioProcessor::setLowpassFrequency(int freq)
{
    this->lowpassFrequency = freq;
}
int AudioFifoTestAudioProcessor::getLowpassFrequency()
{
    return this->lowpassFrequency;
}

void AudioFifoTestAudioProcessor::setHighpassFrequency(int freq)
{
    this->highpassFrequency = freq;
}

int AudioFifoTestAudioProcessor::getHighpassFrequency()
{
    return this->highpassFrequency;
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
