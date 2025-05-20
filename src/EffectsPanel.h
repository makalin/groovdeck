#pragma once

#include <JuceHeader.h>
#include "AudioEngine.h"

class EffectsPanel : public juce::Component,
                    public juce::Slider::Listener,
                    public juce::Button::Listener
{
public:
    EffectsPanel(AudioEngine& engine);
    ~EffectsPanel() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void sliderValueChanged(juce::Slider* slider) override;
    void buttonClicked(juce::Button* button) override;

private:
    AudioEngine& audioEngine;
    
    // Effect toggles
    juce::ToggleButton reverbToggle;
    juce::ToggleButton delayToggle;
    juce::ToggleButton filterToggle;
    juce::ToggleButton distortionToggle;
    
    // Effect parameters
    juce::Slider reverbRoomSize;
    juce::Slider reverbDamping;
    juce::Slider reverbWetLevel;
    juce::Slider reverbDryLevel;
    
    juce::Slider delayTime;
    juce::Slider delayFeedback;
    juce::Slider delayMix;
    
    juce::Slider filterCutoff;
    juce::Slider filterResonance;
    
    juce::Slider distortionDrive;
    juce::Slider distortionMix;
    
    // Labels
    juce::Label reverbLabel;
    juce::Label delayLabel;
    juce::Label filterLabel;
    juce::Label distortionLabel;
    
    void updateEffectParameters();
    void setupSlider(juce::Slider& slider, juce::Label& label, const juce::String& name,
                    double min, double max, double interval, double defaultValue);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EffectsPanel)
}; 