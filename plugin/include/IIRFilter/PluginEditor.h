#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
class IIRFilterAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    IIRFilterAudioProcessorEditor (IIRFilterAudioProcessor&);
    ~IIRFilterAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    IIRFilterAudioProcessor& audioProcessor;

    juce::Slider cutoffSlider;
    juce::Slider qSlider;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> cutoffAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> qAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IIRFilterAudioProcessorEditor)
};

