#include "MainApplication.h"

void MainApplication::initialise(const juce::String& commandLine)
{
    mainWindow.reset(new MainWindow(getApplicationName()));
}

void MainApplication::shutdown()
{
    mainWindow = nullptr;
}

void MainApplication::systemRequestedQuit()
{
    quit();
}

void MainApplication::anotherInstanceStarted(const juce::String& commandLine)
{
    // Handle another instance being launched
}

MainApplication::MainWindow::MainWindow(juce::String name)
    : DocumentWindow(name,
                    juce::Desktop::getInstance().getDefaultLookAndFeel()
                        .findColour(juce::ResizableWindow::backgroundColourId),
                    DocumentWindow::allButtons)
{
    setUsingNativeTitleBar(true);
    setResizable(true, true);
    centreWithSize(800, 600);
    setVisible(true);
}

void MainApplication::MainWindow::closeButtonPressed()
{
    JUCEApplication::getInstance()->systemRequestedQuit();
}

// This creates the application instance
START_JUCE_APPLICATION(MainApplication) 