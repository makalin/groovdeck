#pragma once

#include <memory>
#include <JuceHeader.h>
#include "AudioEngine.h"
#include "EffectsPanel.h"
#include "LiveLoopPanel.h"
#include "SequencerPanel.h"
#include "SampleSlicerPanel.h"
#include "PlaceholderPanel.h"
#include "PanelLauncherStrip.h"
#include "ModuleId.h"

class GroovDeckLookAndFeel;
class SettingsDialog;

class MainComponent : public juce::Component,
                      public juce::Button::Listener,
                      public juce::Slider::Listener
{
public:
    MainComponent (juce::ApplicationProperties& appProps, GroovDeckLookAndFeel& laf);
    ~MainComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void buttonClicked (juce::Button* button) override;
    void sliderValueChanged (juce::Slider* slider) override;

    /** Call after theme / colours change (from settings or startup). */
    void themeChanged();

private:
    void loadSavedUiState();
    void openSettingsDialog();
    void savePanelOrder (const juce::Array<ModuleId>& order);
    void focusPanel (ModuleId id);
    void layoutWorkspaceGrid (juce::Rectangle<int> workspace);
    juce::Rectangle<int> getWorkspaceBounds() const;
    juce::Component* findPanel (ModuleId id) noexcept;
    void loadAudioFile();
    void updatePlayButtonState();

    juce::ApplicationProperties& appProperties;
    GroovDeckLookAndFeel& lookAndFeel;

    AudioEngine audioEngine;

    std::unique_ptr<juce::FileChooser> audioFileChooser;

    EffectsPanel effectsPanel;
    LiveLoopPanel liveLoopPanel;
    SequencerPanel sequencerPanel;
    SampleSlicerPanel sampleSlicerPanel;
    PlaceholderPanel rackMixerPanel;
    PlaceholderPanel rackRoutingPanel;

    juce::TextButton loadButton;
    juce::TextButton playButton;
    juce::TextButton stopButton;
    juce::ToggleButton loopButton;
    juce::Slider volumeSlider;
    juce::Label volumeLabel;
    juce::TextButton settingsButton;

    PanelLauncherStrip launcherStrip;

    std::unique_ptr<SettingsDialog> settingsDialog;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
