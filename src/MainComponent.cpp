#include "MainComponent.h"
#include "GroovDeckLookAndFeel.h"
#include "SettingsDialog.h"

namespace
{
constexpr int kHeaderH = 30;
constexpr int kStripH = 42;
constexpr int kOuterPad = 6;
constexpr int kGridPad = 4;
constexpr int kCols = 3;
constexpr int kRows = 2;
}

MainComponent::MainComponent (juce::ApplicationProperties& appProps, GroovDeckLookAndFeel& laf)
    : appProperties (appProps),
      lookAndFeel (laf),
      effectsPanel (audioEngine),
      liveLoopPanel (audioEngine.getLiveLooper()),
      sequencerPanel (audioEngine.getSequencer()),
      sampleSlicerPanel (audioEngine.getSampleSlicer()),
      rackMixerPanel ("Submix / buses — connect channels here (placeholder)."),
      rackRoutingPanel ("Routing & I/O — device matrix (placeholder).")
{
    addAndMakeVisible (effectsPanel);
    addAndMakeVisible (liveLoopPanel);
    addAndMakeVisible (sequencerPanel);
    addAndMakeVisible (sampleSlicerPanel);
    addAndMakeVisible (rackMixerPanel);
    addAndMakeVisible (rackRoutingPanel);

    loadButton.setButtonText ("Load");
    playButton.setButtonText ("Play");
    stopButton.setButtonText ("Stop");
    loopButton.setButtonText ("Loop");

    volumeSlider.setRange (0.0, 1.0, 0.01);
    volumeSlider.setValue (1.0);
    volumeSlider.setSliderStyle (juce::Slider::LinearHorizontal);
    volumeSlider.setTextBoxStyle (juce::Slider::TextBoxRight, false, 38, 16);
    volumeSlider.setName ("Master");

    volumeLabel.setText ("Mstr", juce::dontSendNotification);
    volumeLabel.setJustificationType (juce::Justification::centredRight);
    volumeLabel.setColour (juce::Label::textColourId, GroovDeckLookAndFeel::textMuted());
    volumeLabel.setFont (juce::Font (juce::FontOptions (9.5f)));

    settingsButton.setButtonText ("Settings");

    addAndMakeVisible (loadButton);
    addAndMakeVisible (playButton);
    addAndMakeVisible (stopButton);
    addAndMakeVisible (loopButton);
    addAndMakeVisible (volumeSlider);
    addAndMakeVisible (volumeLabel);
    addAndMakeVisible (settingsButton);
    addAndMakeVisible (launcherStrip);

    loadButton.addListener (this);
    playButton.addListener (this);
    stopButton.addListener (this);
    loopButton.addListener (this);
    volumeSlider.addListener (this);
    settingsButton.addListener (this);

    launcherStrip.onRequestOpenModule = [this] (ModuleId id) { focusPanel (id); };
    launcherStrip.onOrderChanged = [this] (const juce::Array<ModuleId>& o) { savePanelOrder (o); };

    setSize (1288, 698);
    loadSavedUiState();
}

MainComponent::~MainComponent()
{
    settingsDialog.reset();

    loadButton.removeListener (this);
    playButton.removeListener (this);
    stopButton.removeListener (this);
    loopButton.removeListener (this);
    volumeSlider.removeListener (this);
    settingsButton.removeListener (this);
}

juce::Rectangle<int> MainComponent::getWorkspaceBounds() const
{
    auto a = getLocalBounds().reduced (kOuterPad);
    a.removeFromBottom (kStripH);
    a.removeFromTop (kHeaderH);
    return a;
}

juce::Component* MainComponent::findPanel (ModuleId id) noexcept
{
    switch (id)
    {
        case ModuleId::Effects:       return &effectsPanel;
        case ModuleId::LiveLoop:      return &liveLoopPanel;
        case ModuleId::Sequencer:     return &sequencerPanel;
        case ModuleId::SampleSlicer:  return &sampleSlicerPanel;
        case ModuleId::RackMixer:     return &rackMixerPanel;
        case ModuleId::RackRouting:   return &rackRoutingPanel;
        default:                      return nullptr;
    }
}

void MainComponent::layoutWorkspaceGrid (juce::Rectangle<int> workspace)
{
    if (workspace.getWidth() <= 0 || workspace.getHeight() <= 0)
        return;

    const int innerW = workspace.getWidth() - kGridPad * (kCols - 1);
    const int innerH = workspace.getHeight() - kGridPad * (kRows - 1);
    const int cellW = innerW / kCols;
    const int cellH = innerH / kRows;

    const auto& order = launcherStrip.getPanelOrder();
    int slot = 0;

    for (int row = 0; row < kRows; ++row)
    {
        for (int col = 0; col < kCols; ++col, ++slot)
        {
            if (slot >= order.size())
                return;

            const auto id = order.getReference (slot);
            if (auto* comp = findPanel (id))
            {
                comp->setBounds (workspace.getX() + col * (cellW + kGridPad),
                                  workspace.getY() + row * (cellH + kGridPad),
                                  cellW,
                                  cellH);
            }
        }
    }
}

void MainComponent::focusPanel (ModuleId id)
{
    if (auto* c = findPanel (id))
        c->toFront (false);
}

void MainComponent::themeChanged()
{
    volumeLabel.setColour (juce::Label::textColourId, GroovDeckLookAndFeel::textMuted());

    juce::Component* panels[] = { &effectsPanel,
                                    &liveLoopPanel,
                                    &sequencerPanel,
                                    &sampleSlicerPanel,
                                    &rackMixerPanel,
                                    &rackRoutingPanel };

    for (auto* c : panels)
        c->sendLookAndFeelChange();

    repaint();
}

void MainComponent::loadSavedUiState()
{
    auto* p = appProperties.getUserSettings();
    if (p != nullptr)
    {
        launcherStrip.setPanelOrder (PanelLauncherStrip::parseOrderString (p->getValue ("panelOrder")));
        lookAndFeel.applyTheme (p->getIntValue ("theme", 0));
    }

    themeChanged();
    layoutWorkspaceGrid (getWorkspaceBounds());
}

void MainComponent::openSettingsDialog()
{
    if (settingsDialog == nullptr)
        settingsDialog = std::make_unique<SettingsDialog> (*this, lookAndFeel, appProperties);

    settingsDialog->setVisible (true);
    settingsDialog->toFront (true);
}

void MainComponent::savePanelOrder (const juce::Array<ModuleId>& order)
{
    if (auto* p = appProperties.getUserSettings())
        p->setValue ("panelOrder", PanelLauncherStrip::orderToString (order));

    appProperties.saveIfNeeded();
    layoutWorkspaceGrid (getWorkspaceBounds());
}

void MainComponent::paint (juce::Graphics& g)
{
    auto full = getLocalBounds().toFloat();
    juce::ColourGradient grad (GroovDeckLookAndFeel::background().brighter (0.06f),
                               full.getCentreX(), full.getY(),
                               GroovDeckLookAndFeel::background(),
                               full.getCentreX(), full.getBottom(),
                               false);
    g.setGradientFill (grad);
    g.fillRect (getLocalBounds());

    auto workspace = getWorkspaceBounds().toFloat();

    const auto accent = GroovDeckLookAndFeel::accent();
    g.setColour (accent.withAlpha (0.06f));
    for (float x = workspace.getX(); x < workspace.getRight(); x += 22.0f)
        g.drawVerticalLine (juce::roundToInt (x), workspace.getY(), workspace.getBottom());

    g.setColour (accent.withAlpha (0.04f));
    for (int i = -2; i < 48; ++i)
    {
        const float x0 = workspace.getX() + (float) i * 36.0f;
        g.drawLine (x0, workspace.getY(), x0 + workspace.getHeight() * 0.55f, workspace.getBottom(), 1.0f);
    }

    g.setColour (GroovDeckLookAndFeel::textMuted().withAlpha (0.3f));
    g.setFont (juce::Font (juce::FontOptions (12.0f, juce::Font::italic)));
    g.drawText ("All modules live in this window — reorder chips below (3×2 grid)",
                workspace.toNearestIntEdges().withSizeKeepingCentre (juce::jmin (520, (int) workspace.getWidth()), 36),
                juce::Justification::centred,
                true);

    g.setColour (GroovDeckLookAndFeel::border().withAlpha (0.45f));
    g.drawLine (0.0f, 34.0f, (float) getWidth(), 34.0f, 1.0f);
}

void MainComponent::resized()
{
    auto a = getLocalBounds().reduced (kOuterPad);
    auto stripArea = a.removeFromBottom (kStripH);
    launcherStrip.setBounds (stripArea);

    auto header = a.removeFromTop (kHeaderH);
    const int gap = 4;
    int x = header.getX();
    const int y = header.getY();
    const int bh = 26;

    loadButton.setBounds (x, y, 72, bh);
    x += 72 + gap;
    playButton.setBounds (x, y, 52, bh);
    x += 52 + gap;
    stopButton.setBounds (x, y, 52, bh);
    x += 52 + gap;
    loopButton.setBounds (x, y, 56, bh);
    x += 56 + gap * 2;

    settingsButton.setBounds (x, y, 76, bh);
    x += 76 + gap * 2;

    volumeLabel.setBounds (x, y, 36, bh);
    x += 36 + gap;
    volumeSlider.setBounds (x, y, juce::jmax (80, header.getRight() - x), bh);

    layoutWorkspaceGrid (a);
}

void MainComponent::buttonClicked (juce::Button* button)
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
        audioEngine.setLooping (loopButton.getToggleState());
    else if (button == &settingsButton)
        openSettingsDialog();
}

void MainComponent::sliderValueChanged (juce::Slider* slider)
{
    if (slider == &volumeSlider)
        audioEngine.setGain (static_cast<float> (volumeSlider.getValue()));
}

void MainComponent::loadAudioFile()
{
    audioFileChooser = std::make_unique<juce::FileChooser> (
        "Select an audio file",
        juce::File::getSpecialLocation (juce::File::userHomeDirectory),
        "*.wav;*.mp3;*.aif;*.aiff;*.ogg;*.flac");

    constexpr auto browserFlags =
        juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;

    audioFileChooser->launchAsync (
        browserFlags,
        [this] (const juce::FileChooser& chooser)
        {
            const juce::File file (chooser.getResult());
            if (file.existsAsFile() && audioEngine.loadAudioFile (file))
                updatePlayButtonState();

            audioFileChooser = nullptr;
        });
}

void MainComponent::updatePlayButtonState()
{
    playButton.setEnabled (true);
    stopButton.setEnabled (true);
}
