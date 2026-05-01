#include "LiveLoopPanel.h"
#include "GroovDeckLookAndFeel.h"

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
    
    statusLabel.setText("Ready", juce::dontSendNotification);
    statusLabel.setJustificationType(juce::Justification::centred);
    statusLabel.setColour(juce::Label::textColourId, GroovDeckLookAndFeel::accent());
    statusLabel.setFont(juce::Font(juce::FontOptions(14.0f, juce::Font::bold)));

    for (auto* s : { &loopLengthSlider, &loopGainSlider, &loopStartSlider, &loopEndSlider })
        s->setTextBoxStyle(juce::Slider::TextBoxRight, false, 52, 20);

    loopLengthSlider.setName("Length");
    loopGainSlider.setName("Gain");
    loopStartSlider.setName("Start");
    loopEndSlider.setName("End");
    
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
    GroovDeckLookAndFeel::drawModulePanel(g, getLocalBounds(), "Live looper");
}

void LiveLoopPanel::resized()
{
    auto a = getLocalBounds().reduced(12, 10);
    a.removeFromTop(30);

    const int gap = 6;
    const int bh = 34;
    int x = a.getX();
    int y = a.getY();
    const int fullW = a.getWidth();

    recordButton.setBounds(x, y, fullW, bh);
    y += bh + gap;

    const int qw = (fullW - 3 * gap) / 4;
    playButton.setBounds(x, y, qw, bh);
    stopButton.setBounds(x + qw + gap, y, qw, bh);
    clearButton.setBounds(x + 2 * (qw + gap), y, qw, bh);
    reverseButton.setBounds(x + 3 * (qw + gap), y, qw, bh);
    y += bh + gap;

    statusLabel.setBounds(x, y, fullW, 28);
    y += 28 + gap;

    auto row = [&](juce::Label& lb, juce::Slider& sl)
    {
        const int lh = 26;
        lb.setBounds(x, y, 108, lh);
        sl.setBounds(x + 112, y, fullW - 116, lh);
        y += lh + gap;
    };

    row(loopLengthLabel, loopLengthSlider);
    row(loopGainLabel, loopGainSlider);
    row(loopStartLabel, loopStartSlider);
    row(loopEndLabel, loopEndSlider);

    for (auto* lb : { &loopLengthLabel, &loopGainLabel, &loopStartLabel, &loopEndLabel })
        lb->setColour(juce::Label::textColourId, GroovDeckLookAndFeel::textMuted());
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