#pragma once

#include <JuceHeader.h>

/** Dark DAW-style look used across GroovDeck. */
class GroovDeckLookAndFeel final : public juce::LookAndFeel_V4
{
public:
    GroovDeckLookAndFeel();
    ~GroovDeckLookAndFeel() override = default;

    static juce::Colour background();
    static juce::Colour surface();
    static juce::Colour surfaceElevated();
    static juce::Colour border();
    static juce::Colour accent();
    static juce::Colour accentMuted();
    static juce::Colour text();
    static juce::Colour textMuted();

    /** Rounded panel with optional title strip. */
    static void drawModulePanel(juce::Graphics& g, juce::Rectangle<int> bounds, const juce::String& title);

    void drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
                          float sliderPos, float minSliderPos, float maxSliderPos,
                          const juce::Slider::SliderStyle style, juce::Slider& slider) override;

    void drawButtonBackground(juce::Graphics& g, juce::Button& button,
                              const juce::Colour& backgroundColour,
                              bool shouldDrawButtonAsHighlighted,
                              bool shouldDrawButtonAsDown) override;

    void drawToggleButton(juce::Graphics& g, juce::ToggleButton& button,
                          bool shouldDrawButtonAsHighlighted,
                          bool shouldDrawButtonAsDown) override;

    juce::Font getLabelFont(juce::Label& label) override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GroovDeckLookAndFeel)
};
