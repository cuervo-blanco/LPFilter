#pragma once

#include <JuceHeader.h>
#include <cmath>

class IIRFilter
{
public:
    IIRFilter() = default;
    ~IIRFilter() = default;

    void setCoefficients(double b0, double b1, double b2, double a1, double a2)
    {
        b_[0] = b0;
        b_[1] = b1;
        b_[2] = b2;

        a_[0] = 1.0; // we always normalize so that a0 = 1
        a_[1] = a1;
        a_[2] = a2;

        reset();
    }

    void reset()
    {
        s1_ = 0.0;
        s2_ = 0.0;
    }

    float processSample(float x)
    {
        double y = b_[0] * x + s1_;
        s1_ = b_[1] * x + s2_ - a_[1] * y;
        s2_ = b_[2] * x - a_[2] * y;

        return static_cast<float>(y);
    }

private:
    double b_[3] { 0.0, 0.0, 0.0 };
    double a_[3] { 1.0, 0.0, 0.0 };
    double s1_ { 0.0 }, s2_ { 0.0 };
};

struct FilterCoefficients
{
    double b0, b1, b2;
    double a1, a2;
};

inline FilterCoefficients calculateLPFCoefficients(double frequency, double sampleRate, double qFactor)
{

    const double w0 = 2.0 * MathConstants<double>::pi * frequency / sampleRate;
    const double alpha = std::sin(w0) / (2.0 * qFactor);
    const double cosW0 = std::cos(w0);

    double b0 = (1.0 - cosW0) * 0.5;
    double b1 = 1.0 - cosW0;
    double b2 = (1.0 - cosW0) * 0.5;
    double a0 = 1.0 + alpha;
    double a1 = -2.0 * cosW0;
    double a2 = 1.0 - alpha;

    b0 /= a0;
    b1 /= a0;
    b2 /= a0;
    a1 /= a0;
    a2 /= a0;

    FilterCoefficients coeffs;
    coeffs.b0 = b0;
    coeffs.b1 = b1;
    coeffs.b2 = b2;
    coeffs.a1 = a1;
    coeffs.a2 = a2;
    return coeffs;
}

class IIRFilterAudioProcessor  : public juce::AudioProcessor
{
public:
    IIRFilterAudioProcessor();
    ~IIRFilterAudioProcessor() override;

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #if JucePlugin_Enable_ARA
    bool isBusesLayoutSupported (const juce::AudioProcessor::BusesLayout& layouts) const override;
   #else
    bool isBusesLayoutSupported (const juce::AudioProcessor::BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const juce::String getName() const override;
    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    juce::AudioProcessorValueTreeState parameters;

    void updateFilter();

private:
    ::IIRFilter filterLeft;
    ::IIRFilter filterRight;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IIRFilterAudioProcessor)
};


