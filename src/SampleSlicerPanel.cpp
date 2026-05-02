#include "SampleSlicerPanel.h"
#include "GroovDeckLookAndFeel.h"

SampleSlicerPanel::SampleSlicerPanel(SampleSlicer& slicer)
    : sampleSlicer(slicer)
{
    // Setup buttons
    loadSampleButton.setButtonText("Load");
    unloadSampleButton.setButtonText("Unload");
    autoSliceButton.setButtonText("Auto");
    beatSliceButton.setButtonText("Beat");
    transientSliceButton.setButtonText("Transient");
    clearSlicesButton.setButtonText("Clear");
    playSliceButton.setButtonText("Play");
    stopSliceButton.setButtonText("Stop");
    
    // Setup sliders
    setupSlider(sliceLengthSlider, sliceLengthLabel, "Slice Length", 0.1, 5.0, 0.1, 1.0);
    setupSlider(bpmSlider, bpmLabel, "BPM", 60.0, 200.0, 1.0, 120.0);
    setupSlider(sensitivitySlider, sensitivityLabel, "Sensitivity", 0.1, 2.0, 0.1, 1.0);
    setupSlider(sliceGainSlider, sliceGainLabel, "Slice Gain", 0.0, 2.0, 0.01, 1.0);
    
    sampleInfoLabel.setText("No sample loaded", juce::dontSendNotification);
    sampleInfoLabel.setJustificationType(juce::Justification::centred);
    sampleInfoLabel.setColour(juce::Label::textColourId, GroovDeckLookAndFeel::textMuted());
    sampleInfoLabel.setFont(juce::Font(juce::FontOptions(9.0f, juce::Font::italic)));

    for (auto* s : { &sliceLengthSlider, &bpmSlider, &sensitivitySlider, &sliceGainSlider })
        s->setTextBoxStyle(juce::Slider::TextBoxRight, false, 36, 14);

    for (auto* lb : { &sliceLengthLabel, &bpmLabel, &sensitivityLabel, &sliceGainLabel })
    {
        lb->setColour(juce::Label::textColourId, GroovDeckLookAndFeel::textMuted());
        lb->setFont(juce::Font(juce::FontOptions(9.0f)));
    }
    
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
    GroovDeckLookAndFeel::drawModulePanel(g, getLocalBounds(), "Sample slicer");
}

void SampleSlicerPanel::resized()
{
    auto a = getLocalBounds().reduced(5, 3);
    a.removeFromTop(17);

    const int gap = 3;
    const int bh = 22;
    int x = a.getX();
    int y = a.getY();
    const int w = a.getWidth();

    const int hw = (w - gap) / 2;
    loadSampleButton.setBounds(x, y, hw, bh);
    unloadSampleButton.setBounds(x + hw + gap, y, hw, bh);
    y += bh + gap;

    sampleInfoLabel.setBounds(x, y, w, 18);
    y += 18 + gap;

    const int qw = (w - 3 * gap) / 4;
    autoSliceButton.setBounds(x, y, qw, bh);
    beatSliceButton.setBounds(x + qw + gap, y, qw, bh);
    transientSliceButton.setBounds(x + 2 * (qw + gap), y, qw, bh);
    clearSlicesButton.setBounds(x + 3 * (qw + gap), y, qw, bh);
    y += bh + gap;

    const int lh = 15;
    auto row = [&](juce::Label& lb, juce::Slider& sl)
    {
        lb.setBounds(x, y, 72, lh);
        sl.setBounds(x + 74, y, w - 78, lh);
        y += lh + gap;
    };

    row(sliceLengthLabel, sliceLengthSlider);
    row(bpmLabel, bpmSlider);
    row(sensitivityLabel, sensitivitySlider);
    row(sliceGainLabel, sliceGainSlider);

    y += gap;
    const int pw = (w - gap) / 2;
    playSliceButton.setBounds(x, y, pw, bh);
    stopSliceButton.setBounds(x + pw + gap, y, pw, bh);
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
    sampleFileChooser = std::make_unique<juce::FileChooser>(
        "Select a sample file...",
        juce::File::getSpecialLocation(juce::File::userHomeDirectory),
        "*.wav;*.mp3;*.aif;*.aiff;*.ogg;*.flac");

    constexpr auto browserFlags =
        juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;

    sampleFileChooser->launchAsync(
        browserFlags,
        [this](const juce::FileChooser& chooser)
        {
            const juce::File file(chooser.getResult());
            if (file.existsAsFile() && sampleSlicer.loadSample(file))
                updateSampleInfo();
            sampleFileChooser = nullptr;
        });
}

void SampleSlicerPanel::updateSampleInfo()
{
    if (sampleSlicer.hasSample())
    {
        juce::String info = "Sample: " + juce::String(sampleSlicer.getSampleLength(), 2) + "s, ";
        info += juce::String(sampleSlicer.getNumSlices()) + " slices";
        sampleInfoLabel.setText(info, juce::dontSendNotification);
        sampleInfoLabel.setColour(juce::Label::textColourId, GroovDeckLookAndFeel::text());
        sampleInfoLabel.setFont(juce::Font(juce::FontOptions(9.0f)));
    }
    else
    {
        sampleInfoLabel.setText("No sample loaded", juce::dontSendNotification);
        sampleInfoLabel.setColour(juce::Label::textColourId, GroovDeckLookAndFeel::textMuted());
        sampleInfoLabel.setFont(juce::Font(juce::FontOptions(9.0f, juce::Font::italic)));
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