#include "MainApplication.h"
#include "MainComponent.h"
#include "GroovDeckLookAndFeel.h"

void MainApplication::initialise(const juce::String& commandLine)
{
    juce::ignoreUnused(commandLine);

    juce::PropertiesFile::Options opts;
    opts.applicationName = "GroovDeck";
    opts.filenameSuffix = ".settings";
    opts.folderName = "GroovDeck";
    opts.osxLibrarySubFolder = "Application Support/GroovDeck";
    opts.storageFormat = juce::PropertiesFile::storeAsXML;
    appProperties.setStorageParameters (opts);

    lookAndFeel = std::make_unique<GroovDeckLookAndFeel>();
    juce::LookAndFeel::setDefaultLookAndFeel(lookAndFeel.get());

    if (auto* user = appProperties.getUserSettings())
        lookAndFeel->applyTheme (user->getIntValue ("theme", 0));

    mainWindow.reset (new MainWindow (getApplicationName(), *lookAndFeel, appProperties));
}

void MainApplication::shutdown()
{
    mainWindow = nullptr;
    juce::LookAndFeel::setDefaultLookAndFeel(nullptr);
    lookAndFeel.reset();
    appProperties.saveIfNeeded();
}

void MainApplication::systemRequestedQuit()
{
    quit();
}

void MainApplication::anotherInstanceStarted(const juce::String& commandLine)
{
    juce::ignoreUnused(commandLine);
}

MainApplication::MainWindow::MainWindow (juce::String name,
                                         GroovDeckLookAndFeel& laf,
                                         juce::ApplicationProperties& props)
    : DocumentWindow (name,
                      laf.background(),
                      DocumentWindow::allButtons)
{
    setUsingNativeTitleBar (true);
    setResizable (true, true);
    setResizeLimits (960, 520, 1366, 768);
    setContentOwned (new MainComponent (props, laf), true);
    centreWithSize (1288, 698);
    setVisible (true);
}

void MainApplication::MainWindow::closeButtonPressed()
{
    JUCEApplication::getInstance()->systemRequestedQuit();
}

// This creates the application instance
START_JUCE_APPLICATION(MainApplication) 