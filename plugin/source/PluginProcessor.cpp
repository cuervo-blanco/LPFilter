#include "IIRFilter/PluginProcessor.h"
#include "IIRFilter/PluginEditor.h"

IIRFilterAudioProcessor::IIRFilterAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
          #if ! JucePlugin_IsMidiEffect
           #if JucePlugin_IsSynth
            .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
           #else
            .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
            .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
           #endif
          #endif
       ),
       parameters(*this, nullptr, "PARAMETERS", {
           std::make_unique<juce::AudioParameterFloat>(
               "cutoff",
               "Cutoff",
               juce::NormalisableRange<float>(20.0f, 20000.0f, 0.01f, 0.5f),
               1000.0f),
           std::make_unique<juce::AudioParameterFloat>(
               "qFactor",
               "Q Factor",
               juce::NormalisableRange<float>(0.1f, 10.0f, 0.01f),
               0.707f)
       })
#endif
{
}

IIRFilterAudioProcessor::~IIRFilterAudioProcessor()
{
}

const juce::String IIRFilterAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool IIRFilterAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool IIRFilterAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool IIRFilterAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double IIRFilterAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int IIRFilterAudioProcessor::getNumPrograms()
{
    return 1;
}

int IIRFilterAudioProcessor::getCurrentProgram()
{
    return 0;
}

void IIRFilterAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String IIRFilterAudioProcessor::getProgramName (int index)
{
    return {};
}

void IIRFilterAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void IIRFilterAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Perhaps add recognition of mono/stereo support
    filterLeft.reset();
    filterRight.reset();
    updateFilter();
}

void IIRFilterAudioProcessor::releaseResources()
{
}

#if JucePlugin_Enable_ARA
bool IIRFilterAudioProcessor::isBusesLayoutSupported (const juce::AudioProcessor::BusesLayout& layouts) const
{
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;
    return true;
}
#else
bool IIRFilterAudioProcessor::isBusesLayoutSupported (const juce::AudioProcessor::BusesLayout& layouts) const
{
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;
    return true;
}
#endif

void IIRFilterAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    updateFilter();

    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i) {
        buffer.clear (i, 0, buffer.getNumSamples());
    }

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        float* channelData = buffer.getWritePointer(channel);
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            if (channel == 0)
                channelData[sample] = filterLeft.processSample(channelData[sample]);
            else
                channelData[sample] = filterRight.processSample(channelData[sample]);
        }
    }
}

//==============================================================================
bool IIRFilterAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* IIRFilterAudioProcessor::createEditor()
{
    return new IIRFilterAudioProcessorEditor (*this);
}

//==============================================================================
void IIRFilterAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    if (auto xmlState = parameters.copyState().createXml())
        copyXmlToBinary (*xmlState, destData);
}

void IIRFilterAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    if (auto xmlState = getXmlFromBinary (data, sizeInBytes))
        parameters.replaceState (juce::ValueTree::fromXml (*xmlState));
}

//==============================================================================
void IIRFilterAudioProcessor::updateFilter() {
    float cutoff = *parameters.getRawParameterValue("cutoff");
    float qVal   = *parameters.getRawParameterValue("qFactor");

    auto coeffs = calculateLPFCoefficients(cutoff, getSampleRate(), qVal);

    filterLeft.setCoefficients(coeffs.b0, coeffs.b1, coeffs.b2, coeffs.a1, coeffs.a2);
    filterRight.setCoefficients(coeffs.b0, coeffs.b1, coeffs.b2, coeffs.a1, coeffs.a2);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new IIRFilterAudioProcessor();
}

