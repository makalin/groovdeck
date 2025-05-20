#pragma once

#include <JuceHeader.h>

class MainApplication : public juce::JUCEApplication
{
public:
    MainApplication() = default;
    ~MainApplication() override = default;

    const juce::String getApplicationName() override { return "GroovDeck"; }
    const juce::String getApplicationVersion() override { return "0.1.0"; }
    bool moreThanOneInstanceAllowed() override { return false; }

    void initialise(const juce::String& commandLine) override;
    void shutdown() override;
    void systemRequestedQuit() override;
    void anotherInstanceStarted(const juce::String& commandLine) override;

    class MainWindow : public juce::DocumentWindow
    {
    public:
        MainWindow(juce::String name);
        void closeButtonPressed() override;

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainWindow)
    };

private:
    std::unique_ptr<MainWindow> mainWindow;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainApplication)
}; 