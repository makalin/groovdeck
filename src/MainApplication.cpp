#include "MainApplication.h"
#include "MainComponent.h"
#include "GroovDeckLookAndFeel.h"

void MainApplication::initialise(const juce::String& commandLine)
{
    juce::ignoreUnused(commandLine);

    lookAndFeel = std::make_unique<GroovDeckLookAndFeel>();
    juce::LookAndFeel::setDefaultLookAndFeel(lookAndFeel.get());

    mainWindow.reset(new MainWindow(getApplicationName()));
}

void MainApplication::shutdown()
{
    mainWindow = nullptr;
    juce::LookAndFeel::setDefaultLookAndFeel(nullptr);
    lookAndFeel.reset();
}

void MainApplication::systemRequestedQuit()
{
    quit();
}

void MainApplication::anotherInstanceStarted(const juce::String& commandLine)
{
    juce::ignoreUnused(commandLine);
}

MainApplication::MainWindow::MainWindow(juce::String name)
    : DocumentWindow(name,
                    juce::Desktop::getInstance().getDefaultLookAndFeel()
                        .findColour(juce::ResizableWindow::backgroundColourId),
                    DocumentWindow::allButtons)
{
    setUsingNativeTitleBar(true);
    setResizable(true, true);
    setContentOwned(new MainComponent(), true);
    centreWithSize(1280, 900);
    setVisible(true);
}

void MainApplication::MainWindow::closeButtonPressed()
{
    JUCEApplication::getInstance()->systemRequestedQuit();
}

// This creates the application instance
START_JUCE_APPLICATION(MainApplication) 