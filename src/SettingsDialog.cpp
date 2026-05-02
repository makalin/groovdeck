#include "SettingsDialog.h"
#include "MainComponent.h"
#include "GroovDeckLookAndFeel.h"

namespace
{
class SettingsContent final : public juce::Component,
                              private juce::ComboBox::Listener,
                              private juce::Button::Listener
{
public:
    SettingsContent (MainComponent& main, GroovDeckLookAndFeel& laf, juce::ApplicationProperties& props)
        : mainRef (main), lookRef (laf), appProps (props)
    {
        addAndMakeVisible (title);
        title.setText ("GroovDeck", juce::dontSendNotification);
        title.setFont (juce::Font (juce::FontOptions (18.0f, juce::Font::bold)));
        title.setColour (juce::Label::textColourId, GroovDeckLookAndFeel::text());

        addAndMakeVisible (themeLabel);
        themeLabel.setText ("Colour theme", juce::dontSendNotification);
        themeLabel.setColour (juce::Label::textColourId, GroovDeckLookAndFeel::textMuted());
        themeLabel.setJustificationType (juce::Justification::centredLeft);

        addAndMakeVisible (themeBox);
        themeBox.addItemList (GroovDeckLookAndFeel::getThemeNames(), 1);
        const int savedTheme = appProps.getUserSettings() != nullptr
            ? appProps.getUserSettings()->getIntValue ("theme", 0)
            : 0;
        themeBox.setSelectedId (savedTheme + 1, juce::dontSendNotification);
        themeBox.addListener (this);

        addAndMakeVisible (audioHint);
        audioHint.setText ("Audio routing uses the system default device. Configure buffers in your OS or ASIO control panel.",
                           juce::dontSendNotification);
        audioHint.setColour (juce::Label::textColourId, GroovDeckLookAndFeel::textMuted());
        audioHint.setJustificationType (juce::Justification::topLeft);
        audioHint.setFont (juce::Font (juce::FontOptions (10.0f)));

        addAndMakeVisible (versionLabel);
        versionLabel.setText ("Version " + juce::String (JUCE_APPLICATION_VERSION_STRING),
                              juce::dontSendNotification);
        versionLabel.setColour (juce::Label::textColourId, GroovDeckLookAndFeel::textMuted());
        versionLabel.setJustificationType (juce::Justification::centredLeft);

        addAndMakeVisible (applyButton);
        applyButton.setButtonText ("Apply");
        applyButton.addListener (this);

        addAndMakeVisible (closeButton);
        closeButton.setButtonText ("Close");
        closeButton.addListener (this);

        setSize (400, 260);
    }

    void resized() override
    {
        auto r = getLocalBounds().reduced (14, 12);
        title.setBounds (r.removeFromTop (28));
        r.removeFromTop (10);

        auto row = r.removeFromTop (26);
        themeLabel.setBounds (row.removeFromLeft (110));
        themeBox.setBounds (row.reduced (4, 0));

        r.removeFromTop (10);
        audioHint.setBounds (r.removeFromTop (52));
        r.removeFromTop (8);
        versionLabel.setBounds (r.removeFromTop (22));
        r.removeFromTop (16);

        auto btnRow = r.removeFromTop (28);
        applyButton.setBounds (btnRow.removeFromLeft (88).reduced (0, 2));
        btnRow.removeFromLeft (8);
        closeButton.setBounds (btnRow.removeFromLeft (88).reduced (0, 2));
    }

private:
    void comboBoxChanged (juce::ComboBox*) override
    {
        previewTheme();
    }

    void buttonClicked (juce::Button* b) override
    {
        if (b == &applyButton)
            persistAndApply();
        else if (b == &closeButton)
            if (auto* w = findParentComponentOfClass<juce::DocumentWindow>())
                w->setVisible (false);
    }

    void previewTheme()
    {
        const int idx = themeBox.getSelectedId() - 1;
        lookRef.applyTheme (idx);
        mainRef.themeChanged();
    }

    void persistAndApply()
    {
        if (auto* p = appProps.getUserSettings())
            p->setValue ("theme", themeBox.getSelectedId() - 1);

        appProps.saveIfNeeded();
        previewTheme();
    }

    MainComponent& mainRef;
    GroovDeckLookAndFeel& lookRef;
    juce::ApplicationProperties& appProps;

    juce::Label title, themeLabel, audioHint, versionLabel;
    juce::ComboBox themeBox;
    juce::TextButton applyButton, closeButton;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SettingsContent)
};
}

SettingsDialog::SettingsDialog (MainComponent& main, GroovDeckLookAndFeel& laf, juce::ApplicationProperties& props)
    : DocumentWindow ("Settings",
                      GroovDeckLookAndFeel::background(),
                      DocumentWindow::closeButton)
{
    setUsingNativeTitleBar (true);
    setResizable (true, false);
    setResizeLimits (360, 260, 520, 380);
    setContentOwned (new SettingsContent (main, laf, props), true);
    centreWithSize (getWidth(), getHeight());
}

SettingsDialog::~SettingsDialog() = default;

void SettingsDialog::closeButtonPressed()
{
    setVisible (false);
}
