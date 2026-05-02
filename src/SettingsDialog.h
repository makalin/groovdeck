#pragma once

#include <JuceHeader.h>
#include <memory>

class MainComponent;
class GroovDeckLookAndFeel;

/** Floating settings window (theme, layout persistence, app info). */
class SettingsDialog final : public juce::DocumentWindow
{
public:
    SettingsDialog (MainComponent& main, GroovDeckLookAndFeel& laf, juce::ApplicationProperties& props);
    ~SettingsDialog() override;

    void closeButtonPressed() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SettingsDialog)
};
