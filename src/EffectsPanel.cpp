#include "EffectsPanel.h"
#include "GroovDeckLookAndFeel.h"

namespace
{
void styleValueSlider(juce::Slider& s)
{
    s.setTextBoxStyle(juce::Slider::TextBoxRight, false, 52, 20);
    s.setSliderSnapsToMousePosition(false);
}
}

EffectsPanel::EffectsPanel(AudioEngine& engine)
    : audioEngine(engine)
{
    reverbToggle.setButtonText("Reverb");
    delayToggle.setButtonText("Delay");
    filterToggle.setButtonText("Filter");
    distortionToggle.setButtonText("Distortion");

    setupSlider(reverbRoomSize, reverbLabel, "Room", 0.0, 1.0, 0.01, 0.5);
    setupSlider(reverbDamping, reverbLabel, "Damping", 0.0, 1.0, 0.01, 0.5);
    setupSlider(reverbWetLevel, reverbLabel, "Wet", 0.0, 1.0, 0.01, 0.33);
    setupSlider(reverbDryLevel, reverbLabel, "Dry", 0.0, 1.0, 0.01, 0.67);

    setupSlider(delayTime, delayLabel, "Time (s)", 0.0, 2.0, 0.01, 0.5);
    setupSlider(delayFeedback, delayLabel, "Feedback", 0.0, 0.9, 0.01, 0.3);
    setupSlider(delayMix, delayLabel, "Mix", 0.0, 1.0, 0.01, 0.3);

    setupSlider(filterCutoff, filterLabel, "Cutoff Hz", 20.0, 20000.0, 1.0, 1000.0);
    setupSlider(filterResonance, filterLabel, "Resonance", 0.1, 10.0, 0.1, 0.7);

    setupSlider(distortionDrive, distortionLabel, "Drive", 1.0, 10.0, 0.1, 1.0);
    setupSlider(distortionMix, distortionLabel, "Mix", 0.0, 1.0, 0.01, 0.5);

    for (auto* s : { &reverbRoomSize, &reverbDamping, &reverbWetLevel, &reverbDryLevel,
                     &delayTime, &delayFeedback, &delayMix, &filterCutoff, &filterResonance,
                     &distortionDrive, &distortionMix })
        styleValueSlider(*s);

    addAndMakeVisible(reverbToggle);
    addAndMakeVisible(delayToggle);
    addAndMakeVisible(filterToggle);
    addAndMakeVisible(distortionToggle);

    addAndMakeVisible(reverbRoomSize);
    addAndMakeVisible(reverbDamping);
    addAndMakeVisible(reverbWetLevel);
    addAndMakeVisible(reverbDryLevel);

    addAndMakeVisible(delayTime);
    addAndMakeVisible(delayFeedback);
    addAndMakeVisible(delayMix);

    addAndMakeVisible(filterCutoff);
    addAndMakeVisible(filterResonance);

    addAndMakeVisible(distortionDrive);
    addAndMakeVisible(distortionMix);

    reverbToggle.addListener(this);
    delayToggle.addListener(this);
    filterToggle.addListener(this);
    distortionToggle.addListener(this);

    reverbRoomSize.addListener(this);
    reverbDamping.addListener(this);
    reverbWetLevel.addListener(this);
    reverbDryLevel.addListener(this);

    delayTime.addListener(this);
    delayFeedback.addListener(this);
    delayMix.addListener(this);

    filterCutoff.addListener(this);
    filterResonance.addListener(this);

    distortionDrive.addListener(this);
    distortionMix.addListener(this);

    updateEffectParameters();
}

EffectsPanel::~EffectsPanel()
{
    reverbToggle.removeListener(this);
    delayToggle.removeListener(this);
    filterToggle.removeListener(this);
    distortionToggle.removeListener(this);

    reverbRoomSize.removeListener(this);
    reverbDamping.removeListener(this);
    reverbWetLevel.removeListener(this);
    reverbDryLevel.removeListener(this);

    delayTime.removeListener(this);
    delayFeedback.removeListener(this);
    delayMix.removeListener(this);

    filterCutoff.removeListener(this);
    filterResonance.removeListener(this);

    distortionDrive.removeListener(this);
    distortionMix.removeListener(this);
}

void EffectsPanel::paint(juce::Graphics& g)
{
    GroovDeckLookAndFeel::drawModulePanel(g, getLocalBounds(), "Mix & effects");
}

void EffectsPanel::resized()
{
    auto a = getLocalBounds().reduced(12, 10);
    a.removeFromTop(30);

    const int rowH = 30;
    const int gap = 6;
    const int toggleH = 28;

    auto laySection = [&](juce::ToggleButton& toggle, std::initializer_list<juce::Slider*> sliders)
    {
        toggle.setBounds(a.getX(), a.getY(), 140, toggleH);
        a.removeFromTop(toggleH + gap);
        for (auto* s : sliders)
        {
            s->setBounds(a.removeFromTop(rowH).reduced(0, 2));
            a.removeFromTop(2);
        }
        a.removeFromTop(8);
    };

    laySection(reverbToggle, { &reverbRoomSize, &reverbDamping, &reverbWetLevel, &reverbDryLevel });
    laySection(delayToggle, { &delayTime, &delayFeedback, &delayMix });
    laySection(filterToggle, { &filterCutoff, &filterResonance });
    laySection(distortionToggle, { &distortionDrive, &distortionMix });
}

void EffectsPanel::sliderValueChanged(juce::Slider* slider)
{
    juce::ignoreUnused(slider);
    updateEffectParameters();
}

void EffectsPanel::buttonClicked(juce::Button* button)
{
    juce::ignoreUnused(button);
    updateEffectParameters();
}

void EffectsPanel::setupSlider(juce::Slider& slider, juce::Label& label, const juce::String& name,
                               double min, double max, double interval, double defaultValue)
{
    slider.setRange(min, max, interval);
    slider.setValue(defaultValue);
    slider.setSliderStyle(juce::Slider::LinearHorizontal);
    slider.setName(name);
    label.setText(name, juce::dontSendNotification);
}

void EffectsPanel::updateEffectParameters()
{
    auto& effects = audioEngine.getEffectsProcessor();

    const float wet = reverbToggle.getToggleState() ? (float) reverbWetLevel.getValue() : 0.0f;
    const float dry = reverbToggle.getToggleState() ? (float) reverbDryLevel.getValue() : 1.0f;

    effects.setReverbParameters(
        static_cast<float>(reverbRoomSize.getValue()),
        static_cast<float>(reverbDamping.getValue()),
        wet,
        dry);

    const float dTime = delayToggle.getToggleState() ? (float) delayTime.getValue() : 0.0f;
    const float dFb = delayToggle.getToggleState() ? (float) delayFeedback.getValue() : 0.0f;
    const float dMix = delayToggle.getToggleState() ? (float) delayMix.getValue() : 0.0f;
    effects.setDelayParameters(dTime, dFb, dMix);

    const float cutoff = filterToggle.getToggleState() ? (float) filterCutoff.getValue() : 20000.0f;
    const float res = filterToggle.getToggleState() ? (float) filterResonance.getValue() : 0.1f;
    effects.setFilterParameters(cutoff, res);

    const float drive = distortionToggle.getToggleState() ? (float) distortionDrive.getValue() : 0.0f;
    const float dMix2 = distortionToggle.getToggleState() ? (float) distortionMix.getValue() : 0.0f;
    effects.setDistortionParameters(drive, dMix2);
}
