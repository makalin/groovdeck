#include "MainComponent.h"
#include "GroovDeckLookAndFeel.h"

MainComponent::MainComponent()
    : effectsPanel(audioEngine),
      liveLoopPanel(audioEngine.getLiveLooper()),
      sequencerPanel(audioEngine.getSequencer()),
      sampleSlicerPanel(audioEngine.getSampleSlicer())
{
    loadButton.setButtonText("Load audio");
    playButton.setButtonText("Play");
    stopButton.setButtonText("Stop");
    loopButton.setButtonText("Loop");

    volumeSlider.setRange(0.0, 1.0, 0.01);
    volumeSlider.setValue(1.0);
    volumeSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    volumeSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 52, 22);
    volumeSlider.setName("Master");

    volumeLabel.setText("Master", juce::dontSendNotification);
    volumeLabel.setJustificationType(juce::Justification::centredRight);
    volumeLabel.setColour(juce::Label::textColourId, GroovDeckLookAndFeel::textMuted());

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

    loadButton.addListener(this);
    playButton.addListener(this);
    stopButton.addListener(this);
    loopButton.addListener(this);
    volumeSlider.addListener(this);

    setSize(1280, 900);
}

MainComponent::~MainComponent()
{
    loadButton.removeListener(this);
    playButton.removeListener(this);
    stopButton.removeListener(this);
    loopButton.removeListener(this);
    volumeSlider.removeListener(this);
}

void MainComponent::paint(juce::Graphics& g)
{
    auto r = getLocalBounds().toFloat();
    juce::ColourGradient grad(GroovDeckLookAndFeel::background().brighter(0.04f), r.getCentreX(), r.getY(),
                               GroovDeckLookAndFeel::background(), r.getCentreX(), r.getBottom(), false);
    g.setGradientFill(grad);
    g.fillRect(getLocalBounds());

    g.setColour(GroovDeckLookAndFeel::border().withAlpha(0.35f));
    g.drawLine(0.0f, 52.0f, (float) getWidth(), 52.0f, 1.0f);
}

void MainComponent::resized()
{
    auto a = getLocalBounds().reduced(14);
    const int headerH = 48;
    auto header = a.removeFromTop(headerH);

    const int gap = 8;
    int x = header.getX();
    const int y = header.getY();
    const int bh = 36;

    loadButton.setBounds(x, y, 118, bh);
    x += 118 + gap;
    playButton.setBounds(x, y, 80, bh);
    x += 80 + gap;
    stopButton.setBounds(x, y, 80, bh);
    x += 80 + gap;
    loopButton.setBounds(x, y, 88, bh);
    x += 88 + gap * 2;

    volumeLabel.setBounds(x, y, 56, bh);
    x += 56 + gap;
    volumeSlider.setBounds(x, y, juce::jmax(120, header.getRight() - x), bh);

    a.removeFromTop(10);

    const int rowH = a.getHeight() / 2;
    const int colW = a.getWidth() / 2;
    const int pad = 8;

    auto topRow = a.removeFromTop(rowH);
    effectsPanel.setBounds(topRow.removeFromLeft(colW).reduced(pad, 0));
    liveLoopPanel.setBounds(topRow.reduced(pad, 0));

    sequencerPanel.setBounds(a.removeFromLeft(colW).reduced(pad, 0));
    sampleSlicerPanel.setBounds(a.reduced(pad, 0));
}

void MainComponent::buttonClicked(juce::Button* button)
{
    if (button == &loadButton)
        loadAudioFile();
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
        audioEngine.setLooping(loopButton.getToggleState());
}

void MainComponent::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volumeSlider)
        audioEngine.setGain(static_cast<float>(volumeSlider.getValue()));
}

void MainComponent::loadAudioFile()
{
    audioFileChooser = std::make_unique<juce::FileChooser>(
        "Select an audio file",
        juce::File::getSpecialLocation(juce::File::userHomeDirectory),
        "*.wav;*.mp3;*.aif;*.aiff;*.ogg;*.flac");

    constexpr auto browserFlags =
        juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;

    audioFileChooser->launchAsync(
        browserFlags,
        [this](const juce::FileChooser& chooser)
        {
            const juce::File file(chooser.getResult());
            if (file.existsAsFile() && audioEngine.loadAudioFile(file))
                updatePlayButtonState();

            audioFileChooser = nullptr;
        });
}

void MainComponent::updatePlayButtonState()
{
    playButton.setEnabled(true);
    stopButton.setEnabled(true);
}
