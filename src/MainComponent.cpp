#include "MainComponent.h"

MainComponent::MainComponent()
    : effectsPanel(audioEngine)
{
    // Initialize buttons
    loadButton.setButtonText("Load Audio");
    playButton.setButtonText("Play");
    stopButton.setButtonText("Stop");
    loopButton.setButtonText("Loop");
    
    // Initialize volume slider
    volumeSlider.setRange(0.0, 1.0, 0.01);
    volumeSlider.setValue(1.0);
    volumeSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    volumeLabel.setText("Volume", juce::dontSendNotification);
    
    // Add components
    addAndMakeVisible(loadButton);
    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(loopButton);
    addAndMakeVisible(volumeSlider);
    addAndMakeVisible(volumeLabel);
    addAndMakeVisible(effectsPanel);
    
    // Add listeners
    loadButton.addListener(this);
    playButton.addListener(this);
    stopButton.addListener(this);
    loopButton.addListener(this);
    volumeSlider.addListener(this);
    
    setSize(800, 800); // Increased height to accommodate effects panel
}

MainComponent::~MainComponent()
{
    // Remove listeners
    loadButton.removeListener(this);
    playButton.removeListener(this);
    stopButton.removeListener(this);
    loopButton.removeListener(this);
    volumeSlider.removeListener(this);
}

void MainComponent::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void MainComponent::resized()
{
    auto area = getLocalBounds();
    auto buttonHeight = 40;
    auto margin = 10;
    
    // Layout transport controls
    auto transportArea = area.removeFromTop(buttonHeight * 5);
    loadButton.setBounds(transportArea.removeFromTop(buttonHeight).reduced(margin));
    playButton.setBounds(transportArea.removeFromTop(buttonHeight).reduced(margin));
    stopButton.setBounds(transportArea.removeFromTop(buttonHeight).reduced(margin));
    loopButton.setBounds(transportArea.removeFromTop(buttonHeight).reduced(margin));
    
    auto volumeArea = transportArea.removeFromTop(buttonHeight).reduced(margin);
    volumeLabel.setBounds(volumeArea.removeFromLeft(100));
    volumeSlider.setBounds(volumeArea);
    
    // Layout effects panel
    effectsPanel.setBounds(area.reduced(margin));
}

void MainComponent::buttonClicked(juce::Button* button)
{
    if (button == &loadButton)
    {
        loadAudioFile();
    }
    else if (button == &playButton)
    {
        audioEngine.startPlayback();
        updatePlayButtonState();
    }
    else if (button == &stopButton)
    {
        audioEngine.stopPlayback();
        updatePlayButtonState();
    }
    else if (button == &loopButton)
    {
        audioEngine.setLooping(loopButton.getToggleState());
    }
}

void MainComponent::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volumeSlider)
    {
        audioEngine.setGain(static_cast<float>(volumeSlider.getValue()));
    }
}

void MainComponent::loadAudioFile()
{
    juce::FileChooser chooser("Select an audio file...",
                             juce::File::getSpecialLocation(juce::File::userHomeDirectory),
                             "*.wav;*.mp3;*.aif;*.aiff;*.ogg;*.flac");
    
    if (chooser.browseForFileToOpen())
    {
        auto file = chooser.getResult();
        if (audioEngine.loadAudioFile(file))
        {
            updatePlayButtonState();
        }
    }
}

void MainComponent::updatePlayButtonState()
{
    playButton.setEnabled(true);
    stopButton.setEnabled(true);
} 