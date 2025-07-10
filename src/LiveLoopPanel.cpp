#include "LiveLoopPanel.h"

LiveLoopPanel::LiveLoopPanel(LiveLooper& looper)
    : liveLooper(looper)
{
    // Setup buttons
    recordButton.setButtonText("Record");
    playButton.setButtonText("Play");
    stopButton.setButtonText("Stop");
    clearButton.setButtonText("Clear");
    reverseButton.setButtonText("Reverse");
    
    // Setup sliders
    setupSlider(loopLengthSlider, loopLengthLabel, "Loop Length", 1.0, 30.0, 0.1, 4.0);
    setupSlider(loopGainSlider, loopGainLabel, "Loop Gain", 0.0, 2.0, 0.01, 1.0);
    setupSlider(loopStartSlider, loopStartLabel, "Loop Start", 0.0, 30.0, 0.1, 0.0);
    setupSlider(loopEndSlider, loopEndLabel, "Loop End", 0.0, 30.0, 0.1, 4.0);
    
    // Setup status label
    statusLabel.setText("Ready", juce::dontSendNotification);
    statusLabel.setJustificationType(juce::Justification::centred);
    
    // Add components
    addAndMakeVisible(recordButton);
    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(clearButton);
    addAndMakeVisible(reverseButton);
    
    addAndMakeVisible(loopLengthSlider);
    addAndMakeVisible(loopGainSlider);
    addAndMakeVisible(loopStartSlider);
    addAndMakeVisible(loopEndSlider);
    
    addAndMakeVisible(loopLengthLabel);
    addAndMakeVisible(loopGainLabel);
    addAndMakeVisible(loopStartLabel);
    addAndMakeVisible(loopEndLabel);
    addAndMakeVisible(statusLabel);
    
    // Add listeners
    recordButton.addListener(this);
    playButton.addListener(this);
    stopButton.addListener(this);
    clearButton.addListener(this);
    reverseButton.addListener(this);
    
    loopLengthSlider.addListener(this);
    loopGainSlider.addListener(this);
    loopStartSlider.addListener(this);
    loopEndSlider.addListener(this);
    
    updateButtonStates();
}

LiveLoopPanel::~LiveLoopPanel()
{
    // Remove listeners
    recordButton.removeListener(this);
    playButton.removeListener(this);
    stopButton.removeListener(this);
    clearButton.removeListener(this);
    reverseButton.removeListener(this);
    
    loopLengthSlider.removeListener(this);
    loopGainSlider.removeListener(this);
    loopStartSlider.removeListener(this);
    loopEndSlider.removeListener(this);
}

void LiveLoopPanel::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void LiveLoopPanel::resized()
{
    auto area = getLocalBounds();
    auto buttonHeight = 40;
    auto margin = 10;
    
    // Control buttons
    auto buttonArea = area.removeFromTop(buttonHeight * 2).reduced(margin);
    recordButton.setBounds(buttonArea.removeFromTop(buttonHeight).reduced(5));
    playButton.setBounds(buttonArea.removeFromLeft(buttonArea.getWidth() / 4).reduced(5));
    stopButton.setBounds(buttonArea.removeFromLeft(buttonArea.getWidth() / 3).reduced(5));
    clearButton.setBounds(buttonArea.removeFromLeft(buttonArea.getWidth() / 2).reduced(5));
    reverseButton.setBounds(buttonArea.reduced(5));
    
    // Status display
    statusLabel.setBounds(area.removeFromTop(30).reduced(margin));
    
    // Parameter sliders
    auto sliderArea = area.removeFromTop(120).reduced(margin);
    loopLengthSlider.setBounds(sliderArea.removeFromTop(30).reduced(5));
    loopGainSlider.setBounds(sliderArea.removeFromTop(30).reduced(5));
    loopStartSlider.setBounds(sliderArea.removeFromTop(30).reduced(5));
    loopEndSlider.setBounds(sliderArea.removeFromTop(30).reduced(5));
}

void LiveLoopPanel::buttonClicked(juce::Button* button)
{
    if (button == &recordButton)
    {
        if (liveLooper.isRecording())
        {
            liveLooper.stopRecording();
        }
        else
        {
            liveLooper.startRecording();
        }
    }
    else if (button == &playButton)
    {
        liveLooper.startPlayback();
    }
    else if (button == &stopButton)
    {
        liveLooper.stopPlayback();
    }
    else if (button == &clearButton)
    {
        liveLooper.clearLoop();
    }
    else if (button == &reverseButton)
    {
        liveLooper.reverseLoop();
    }
    
    updateButtonStates();
    updateStatus();
}

void LiveLoopPanel::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &loopLengthSlider)
    {
        liveLooper.setLoopLength(loopLengthSlider.getValue());
    }
    else if (slider == &loopGainSlider)
    {
        liveLooper.setLoopGain(static_cast<float>(loopGainSlider.getValue()));
    }
    else if (slider == &loopStartSlider)
    {
        liveLooper.setLoopStart(loopStartSlider.getValue());
    }
    else if (slider == &loopEndSlider)
    {
        liveLooper.setLoopEnd(loopEndSlider.getValue());
    }
}

void LiveLoopPanel::updateButtonStates()
{
    recordButton.setEnabled(!liveLooper.isPlaying());
    playButton.setEnabled(liveLooper.hasLoop() && !liveLooper.isRecording());
    stopButton.setEnabled(liveLooper.isPlaying() || liveLooper.isRecording());
    clearButton.setEnabled(liveLooper.hasLoop() || liveLooper.isRecording());
    reverseButton.setEnabled(liveLooper.hasLoop());
}

void LiveLoopPanel::updateStatus()
{
    if (liveLooper.isRecording())
    {
        statusLabel.setText("Recording...", juce::dontSendNotification);
    }
    else if (liveLooper.isPlaying())
    {
        statusLabel.setText("Playing Loop", juce::dontSendNotification);
    }
    else if (liveLooper.hasLoop())
    {
        statusLabel.setText("Loop Ready", juce::dontSendNotification);
    }
    else
    {
        statusLabel.setText("Ready", juce::dontSendNotification);
    }
}

void LiveLoopPanel::setupSlider(juce::Slider& slider, juce::Label& label, const juce::String& name,
                               double min, double max, double interval, double defaultValue)
{
    slider.setRange(min, max, interval);
    slider.setValue(defaultValue);
    slider.setSliderStyle(juce::Slider::LinearHorizontal);
    label.setText(name, juce::dontSendNotification);
} 