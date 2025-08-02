#include "MainComponent.h"

MainComponent::MainComponent()
    : effectsPanel(audioEngine),
      liveLoopPanel(audioEngine.getLiveLooper()),
      sequencerPanel(audioEngine.getSequencer()),
      sampleSlicerPanel(audioEngine.getSampleSlicer())
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
    addAndMakeVisible(liveLoopPanel);
    addAndMakeVisible(sequencerPanel);
    addAndMakeVisible(sampleSlicerPanel);
    
    // Add listeners
    loadButton.addListener(this);
    playButton.addListener(this);
    stopButton.addListener(this);
    loopButton.addListener(this);
    volumeSlider.addListener(this);
    
    setSize(1400, 1200); // Increased size to accommodate all panels
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
    
    // Transport controls at the top
    auto transportArea = area.removeFromTop(buttonHeight * 2).reduced(margin);
    loadButton.setBounds(transportArea.removeFromTop(buttonHeight).reduced(5));
    playButton.setBounds(transportArea.removeFromLeft(transportArea.getWidth() / 4).reduced(5));
    stopButton.setBounds(transportArea.removeFromLeft(transportArea.getWidth() / 3).reduced(5));
    loopButton.setBounds(transportArea.removeFromLeft(transportArea.getWidth() / 2).reduced(5));
    
    auto volumeArea = transportArea.removeFromTop(buttonHeight).reduced(5);
    volumeLabel.setBounds(volumeArea.removeFromLeft(100));
    volumeSlider.setBounds(volumeArea);
    
    // Split remaining area into panels (2x2 grid)
    auto panelHeight = area.getHeight() / 2;
    auto panelWidth = area.getWidth() / 2;
    
    // Top row: Effects and Live Loop
    auto topRow = area.removeFromTop(panelHeight);
    effectsPanel.setBounds(topRow.removeFromLeft(panelWidth).reduced(margin));
    liveLoopPanel.setBounds(topRow.reduced(margin));
    
    // Bottom row: Sequencer and Sample Slicer
    sequencerPanel.setBounds(area.removeFromLeft(panelWidth).reduced(margin));
    sampleSlicerPanel.setBounds(area.reduced(margin));
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