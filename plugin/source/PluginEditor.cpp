#include "IIRFilter/PluginEditor.h"
#include "IIRFilter/PluginProcessor.h"

//==============================================================================
IIRFilterAudioProcessorEditor::IIRFilterAudioProcessorEditor (IIRFilterAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    cutoffSlider.setSliderStyle(juce::Slider::Rotary);
    cutoffSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 20);
    addAndMakeVisible(cutoffSlider);

    qSlider.setSliderStyle(juce::Slider::Rotary);
    qSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 20);
    addAndMakeVisible(qSlider);

    cutoffAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.parameters, "cutoff", cutoffSlider);

    qAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.parameters, "qFactor", qSlider);

    setSize (600, 400);
}

IIRFilterAudioProcessorEditor::~IIRFilterAudioProcessorEditor()
{
}

//==============================================================================
void IIRFilterAudioProcessorEditor::paint (juce::Graphics& g)
{
    // Maybe in future add a graphical representation of the sound and the
    // filter (as a curve)
    g.fillAll (juce::Colours::darkgrey);
    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("IIR Low-Pass Filter", getLocalBounds(), juce::Justification::centredTop, 1);
}

void IIRFilterAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced (20);
    auto sliderWidth = 100;
    auto sliderHeight = 100;

    cutoffSlider.setBounds(area.removeFromLeft(sliderWidth).removeFromTop(sliderHeight).withSizeKeepingCentre(sliderWidth, sliderHeight));
    qSlider.setBounds(area.removeFromLeft(sliderWidth).removeFromTop(sliderHeight).withSizeKeepingCentre(sliderWidth, sliderHeight));
}

