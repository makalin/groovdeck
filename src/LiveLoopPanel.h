#pragma once

#include <JuceHeader.h>
#include "LiveLooper.h"

class LiveLoopPanel : public juce::Component,
                     public juce::Button::Listener,
                     public juce::Slider::Listener
{
public:
    LiveLoopPanel(LiveLooper& looper);
    ~LiveLoopPanel() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void buttonClicked(juce::Button* button) override;
    void sliderValueChanged(juce::Slider* slider) override;

private:
    LiveLooper& liveLooper;
    
    // Loop control buttons
    juce::TextButton recordButton;
    juce::TextButton playButton;
    juce::TextButton stopButton;
    juce::TextButton clearButton;
    juce::TextButton reverseButton;
    
    // Loop parameters
    juce::Slider loopLengthSlider;
    juce::Slider loopGainSlider;
    juce::Slider loopStartSlider;
    juce::Slider loopEndSlider;
    
    // Labels
    juce::Label loopLengthLabel;
    juce::Label loopGainLabel;
    juce::Label loopStartLabel;
    juce::Label loopEndLabel;
    
    // Status display
    juce::Label statusLabel;
    
    void updateButtonStates();
    void updateStatus();
    void setupSlider(juce::Slider& slider, juce::Label& label, const juce::String& name,
                    double min, double max, double interval, double defaultValue);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LiveLoopPanel)
}; 