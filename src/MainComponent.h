#pragma once

#include <JuceHeader.h>
#include "AudioEngine.h"
#include "EffectsPanel.h"
#include "LiveLoopPanel.h"
#include "SequencerPanel.h"
#include "SampleSlicerPanel.h"

class MainComponent : public juce::Component,
                     public juce::Button::Listener,
                     public juce::Slider::Listener
{
public:
    MainComponent();
    ~MainComponent() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void buttonClicked(juce::Button* button) override;
    void sliderValueChanged(juce::Slider* slider) override;

private:
    AudioEngine audioEngine;
    EffectsPanel effectsPanel;
    LiveLoopPanel liveLoopPanel;
    SequencerPanel sequencerPanel;
    SampleSlicerPanel sampleSlicerPanel;
    
    juce::TextButton loadButton;
    juce::TextButton playButton;
    juce::TextButton stopButton;
    juce::ToggleButton loopButton;
    juce::Slider volumeSlider;
    juce::Label volumeLabel;
    
    void loadAudioFile();
    void updatePlayButtonState();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
}; 