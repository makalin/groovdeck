#pragma once

#include <JuceHeader.h>
#include "Sequencer.h"

class SequencerPanel : public juce::Component,
                      public juce::Button::Listener,
                      public juce::Slider::Listener
{
public:
    SequencerPanel(Sequencer& sequencer);
    ~SequencerPanel() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void buttonClicked(juce::Button* button) override;
    void sliderValueChanged(juce::Slider* slider) override;

private:
    Sequencer& sequencer;
    
    // Sequencer control buttons
    juce::TextButton startButton;
    juce::TextButton stopButton;
    juce::TextButton resetButton;
    juce::TextButton clearButton;
    juce::TextButton randomButton;
    juce::TextButton shiftLeftButton;
    juce::TextButton shiftRightButton;
    
    // Step buttons (16 steps)
    std::array<juce::ToggleButton, 16> stepButtons;
    
    // Sequencer parameters
    juce::Slider tempoSlider;
    juce::Slider stepsSlider;
    
    // Labels
    juce::Label tempoLabel;
    juce::Label stepsLabel;
    juce::Label sequencerLabel;
    
    void updateButtonStates();
    void updateStepButtons();
    void setupSlider(juce::Slider& slider, juce::Label& label, const juce::String& name,
                    double min, double max, double interval, double defaultValue);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SequencerPanel)
}; 