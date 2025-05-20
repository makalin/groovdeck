#include "EffectsPanel.h"

EffectsPanel::EffectsPanel(AudioEngine& engine)
    : audioEngine(engine)
{
    // Setup effect toggles
    reverbToggle.setButtonText("Reverb");
    delayToggle.setButtonText("Delay");
    filterToggle.setButtonText("Filter");
    distortionToggle.setButtonText("Distortion");
    
    // Setup sliders
    setupSlider(reverbRoomSize, reverbLabel, "Reverb Room Size", 0.0, 1.0, 0.01, 0.5);
    setupSlider(reverbDamping, reverbLabel, "Reverb Damping", 0.0, 1.0, 0.01, 0.5);
    setupSlider(reverbWetLevel, reverbLabel, "Reverb Wet", 0.0, 1.0, 0.01, 0.33);
    setupSlider(reverbDryLevel, reverbLabel, "Reverb Dry", 0.0, 1.0, 0.01, 0.67);
    
    setupSlider(delayTime, delayLabel, "Delay Time", 0.0, 2.0, 0.01, 0.5);
    setupSlider(delayFeedback, delayLabel, "Delay Feedback", 0.0, 0.9, 0.01, 0.3);
    setupSlider(delayMix, delayLabel, "Delay Mix", 0.0, 1.0, 0.01, 0.3);
    
    setupSlider(filterCutoff, filterLabel, "Filter Cutoff", 20.0, 20000.0, 1.0, 1000.0);
    setupSlider(filterResonance, filterLabel, "Filter Resonance", 0.1, 10.0, 0.1, 0.7);
    
    setupSlider(distortionDrive, distortionLabel, "Distortion Drive", 1.0, 10.0, 0.1, 1.0);
    setupSlider(distortionMix, distortionLabel, "Distortion Mix", 0.0, 1.0, 0.01, 0.5);
    
    // Add components
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
    
    // Add listeners
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
}

EffectsPanel::~EffectsPanel()
{
    // Remove listeners
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
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void EffectsPanel::resized()
{
    auto area = getLocalBounds();
    auto sectionHeight = area.getHeight() / 4;
    auto margin = 10;
    
    // Reverb section
    auto reverbArea = area.removeFromTop(sectionHeight).reduced(margin);
    reverbToggle.setBounds(reverbArea.removeFromTop(30));
    auto reverbSliders = reverbArea.removeFromTop(80);
    reverbRoomSize.setBounds(reverbSliders.removeFromLeft(reverbSliders.getWidth() / 4).reduced(5));
    reverbDamping.setBounds(reverbSliders.removeFromLeft(reverbSliders.getWidth() / 3).reduced(5));
    reverbWetLevel.setBounds(reverbSliders.removeFromLeft(reverbSliders.getWidth() / 2).reduced(5));
    reverbDryLevel.setBounds(reverbSliders.reduced(5));
    
    // Delay section
    auto delayArea = area.removeFromTop(sectionHeight).reduced(margin);
    delayToggle.setBounds(delayArea.removeFromTop(30));
    auto delaySliders = delayArea.removeFromTop(80);
    delayTime.setBounds(delaySliders.removeFromLeft(delaySliders.getWidth() / 3).reduced(5));
    delayFeedback.setBounds(delaySliders.removeFromLeft(delaySliders.getWidth() / 2).reduced(5));
    delayMix.setBounds(delaySliders.reduced(5));
    
    // Filter section
    auto filterArea = area.removeFromTop(sectionHeight).reduced(margin);
    filterToggle.setBounds(filterArea.removeFromTop(30));
    auto filterSliders = filterArea.removeFromTop(80);
    filterCutoff.setBounds(filterSliders.removeFromLeft(filterSliders.getWidth() / 2).reduced(5));
    filterResonance.setBounds(filterSliders.reduced(5));
    
    // Distortion section
    auto distortionArea = area.removeFromTop(sectionHeight).reduced(margin);
    distortionToggle.setBounds(distortionArea.removeFromTop(30));
    auto distortionSliders = distortionArea.removeFromTop(80);
    distortionDrive.setBounds(distortionSliders.removeFromLeft(distortionSliders.getWidth() / 2).reduced(5));
    distortionMix.setBounds(distortionSliders.reduced(5));
}

void EffectsPanel::sliderValueChanged(juce::Slider* slider)
{
    updateEffectParameters();
}

void EffectsPanel::buttonClicked(juce::Button* button)
{
    updateEffectParameters();
}

void EffectsPanel::setupSlider(juce::Slider& slider, juce::Label& label, const juce::String& name,
                             double min, double max, double interval, double defaultValue)
{
    slider.setRange(min, max, interval);
    slider.setValue(defaultValue);
    slider.setSliderStyle(juce::Slider::LinearHorizontal);
    label.setText(name, juce::dontSendNotification);
}

void EffectsPanel::updateEffectParameters()
{
    auto& effects = audioEngine.getEffectsProcessor();
    
    // Update reverb parameters
    effects.setReverbParameters(
        static_cast<float>(reverbRoomSize.getValue()),
        static_cast<float>(reverbDamping.getValue()),
        static_cast<float>(reverbWetLevel.getValue()),
        static_cast<float>(reverbDryLevel.getValue())
    );
    
    // Update delay parameters
    effects.setDelayParameters(
        static_cast<float>(delayTime.getValue()),
        static_cast<float>(delayFeedback.getValue()),
        static_cast<float>(delayMix.getValue())
    );
    
    // Update filter parameters
    effects.setFilterParameters(
        static_cast<float>(filterCutoff.getValue()),
        static_cast<float>(filterResonance.getValue())
    );
    
    // Update distortion parameters
    effects.setDistortionParameters(
        static_cast<float>(distortionDrive.getValue()),
        static_cast<float>(distortionMix.getValue())
    );
} 