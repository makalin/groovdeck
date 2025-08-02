#include "SampleSlicerPanel.h"

SampleSlicerPanel::SampleSlicerPanel(SampleSlicer& slicer)
    : sampleSlicer(slicer)
{
    // Setup buttons
    loadSampleButton.setButtonText("Load Sample");
    unloadSampleButton.setButtonText("Unload Sample");
    autoSliceButton.setButtonText("Auto Slice");
    beatSliceButton.setButtonText("Beat Slice");
    transientSliceButton.setButtonText("Transient Slice");
    clearSlicesButton.setButtonText("Clear Slices");
    playSliceButton.setButtonText("Play Slice");
    stopSliceButton.setButtonText("Stop Slice");
    
    // Setup sliders
    setupSlider(sliceLengthSlider, sliceLengthLabel, "Slice Length", 0.1, 5.0, 0.1, 1.0);
    setupSlider(bpmSlider, bpmLabel, "BPM", 60.0, 200.0, 1.0, 120.0);
    setupSlider(sensitivitySlider, sensitivityLabel, "Sensitivity", 0.1, 2.0, 0.1, 1.0);
    setupSlider(sliceGainSlider, sliceGainLabel, "Slice Gain", 0.0, 2.0, 0.01, 1.0);
    
    // Setup labels
    sampleInfoLabel.setText("No sample loaded", juce::dontSendNotification);
    sampleInfoLabel.setJustificationType(juce::Justification::centred);
    
    // Add components
    addAndMakeVisible(loadSampleButton);
    addAndMakeVisible(unloadSampleButton);
    addAndMakeVisible(autoSliceButton);
    addAndMakeVisible(beatSliceButton);
    addAndMakeVisible(transientSliceButton);
    addAndMakeVisible(clearSlicesButton);
    addAndMakeVisible(playSliceButton);
    addAndMakeVisible(stopSliceButton);
    
    addAndMakeVisible(sliceLengthSlider);
    addAndMakeVisible(bpmSlider);
    addAndMakeVisible(sensitivitySlider);
    addAndMakeVisible(sliceGainSlider);
    
    addAndMakeVisible(sliceLengthLabel);
    addAndMakeVisible(bpmLabel);
    addAndMakeVisible(sensitivityLabel);
    addAndMakeVisible(sliceGainLabel);
    addAndMakeVisible(sampleInfoLabel);
    
    // Add listeners
    loadSampleButton.addListener(this);
    unloadSampleButton.addListener(this);
    autoSliceButton.addListener(this);
    beatSliceButton.addListener(this);
    transientSliceButton.addListener(this);
    clearSlicesButton.addListener(this);
    playSliceButton.addListener(this);
    stopSliceButton.addListener(this);
    
    sliceLengthSlider.addListener(this);
    bpmSlider.addListener(this);
    sensitivitySlider.addListener(this);
    sliceGainSlider.addListener(this);
}

SampleSlicerPanel::~SampleSlicerPanel()
{
    // Remove listeners
    loadSampleButton.removeListener(this);
    unloadSampleButton.removeListener(this);
    autoSliceButton.removeListener(this);
    beatSliceButton.removeListener(this);
    transientSliceButton.removeListener(this);
    clearSlicesButton.removeListener(this);
    playSliceButton.removeListener(this);
    stopSliceButton.removeListener(this);
    
    sliceLengthSlider.removeListener(this);
    bpmSlider.removeListener(this);
    sensitivitySlider.removeListener(this);
    sliceGainSlider.removeListener(this);
}

void SampleSlicerPanel::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void SampleSlicerPanel::resized()
{
    auto area = getLocalBounds();
    auto buttonHeight = 30;
    auto margin = 10;
    
    // Sample loading section
    auto loadArea = area.removeFromTop(buttonHeight * 2).reduced(margin);
    loadSampleButton.setBounds(loadArea.removeFromTop(buttonHeight).reduced(5));
    unloadSampleButton.setBounds(loadArea.removeFromTop(buttonHeight).reduced(5));
    
    // Sample info
    sampleInfoLabel.setBounds(area.removeFromTop(30).reduced(margin));
    
    // Slicing controls
    auto sliceArea = area.removeFromTop(buttonHeight * 2).reduced(margin);
    autoSliceButton.setBounds(sliceArea.removeFromTop(buttonHeight).removeFromLeft(sliceArea.getWidth() / 2).reduced(5));
    beatSliceButton.setBounds(sliceArea.removeFromLeft(sliceArea.getWidth() / 2).reduced(5));
    transientSliceButton.setBounds(sliceArea.removeFromLeft(sliceArea.getWidth() / 2).reduced(5));
    clearSlicesButton.setBounds(sliceArea.reduced(5));
    
    // Parameters
    auto paramArea = area.removeFromTop(120).reduced(margin);
    sliceLengthSlider.setBounds(paramArea.removeFromTop(30).reduced(5));
    bpmSlider.setBounds(paramArea.removeFromTop(30).reduced(5));
    sensitivitySlider.setBounds(paramArea.removeFromTop(30).reduced(5));
    sliceGainSlider.setBounds(paramArea.removeFromTop(30).reduced(5));
    
    // Slice playback controls
    auto playbackArea = area.removeFromTop(buttonHeight).reduced(margin);
    playSliceButton.setBounds(playbackArea.removeFromLeft(playbackArea.getWidth() / 2).reduced(5));
    stopSliceButton.setBounds(playbackArea.reduced(5));
}

void SampleSlicerPanel::buttonClicked(juce::Button* button)
{
    if (button == &loadSampleButton)
    {
        loadSample();
    }
    else if (button == &unloadSampleButton)
    {
        sampleSlicer.unloadSample();
        updateSampleInfo();
    }
    else if (button == &autoSliceButton)
    {
        sampleSlicer.autoSlice(sliceLengthSlider.getValue());
    }
    else if (button == &beatSliceButton)
    {
        sampleSlicer.sliceAtBeats(bpmSlider.getValue());
    }
    else if (button == &transientSliceButton)
    {
        sampleSlicer.sliceAtTransients(sensitivitySlider.getValue());
    }
    else if (button == &clearSlicesButton)
    {
        sampleSlicer.clearSlices();
    }
    else if (button == &playSliceButton)
    {
        // Play first slice for now
        if (sampleSlicer.getNumSlices() > 0)
        {
            sampleSlicer.playSlice(0);
        }
    }
    else if (button == &stopSliceButton)
    {
        sampleSlicer.stopSlice();
    }
}

void SampleSlicerPanel::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &sliceGainSlider)
    {
        sampleSlicer.setSliceGain(0, static_cast<float>(sliceGainSlider.getValue()));
    }
}

void SampleSlicerPanel::loadSample()
{
    juce::FileChooser chooser("Select a sample file...",
                             juce::File::getSpecialLocation(juce::File::userHomeDirectory),
                             "*.wav;*.mp3;*.aif;*.aiff;*.ogg;*.flac");
    
    if (chooser.browseForFileToOpen())
    {
        auto file = chooser.getResult();
        if (sampleSlicer.loadSample(file))
        {
            updateSampleInfo();
        }
    }
}

void SampleSlicerPanel::updateSampleInfo()
{
    if (sampleSlicer.hasSample())
    {
        juce::String info = "Sample: " + juce::String(sampleSlicer.getSampleLength(), 2) + "s, ";
        info += juce::String(sampleSlicer.getNumSlices()) + " slices";
        sampleInfoLabel.setText(info, juce::dontSendNotification);
    }
    else
    {
        sampleInfoLabel.setText("No sample loaded", juce::dontSendNotification);
    }
}

void SampleSlicerPanel::updateSliceList()
{
    // TODO: Update slice list display
}

void SampleSlicerPanel::setupSlider(juce::Slider& slider, juce::Label& label, const juce::String& name,
                                   double min, double max, double interval, double defaultValue)
{
    slider.setRange(min, max, interval);
    slider.setValue(defaultValue);
    slider.setSliderStyle(juce::Slider::LinearHorizontal);
    label.setText(name, juce::dontSendNotification);
} 