#pragma once

#include <JuceHeader.h>

/** DAW-style look used across GroovDeck; supports multiple themes via applyTheme(). */
class GroovDeckLookAndFeel final : public juce::LookAndFeel_V4
{
public:
    GroovDeckLookAndFeel();
    ~GroovDeckLookAndFeel() override;

    /** 0 Midnight, 1 Ocean, 2 Ember, 3 Dawn (light). */
    void applyTheme(int themeIndex);
    int getCurrentTheme() const { return currentTheme; }

    static juce::StringArray getThemeNames();

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
    juce::Font getTextButtonFont(juce::TextButton&, int buttonHeight) override;

private:
    void syncColoursFromPalette();
    int currentTheme = 0;

    juce::Colour cBackground, cSurface, cSurfaceElevated, cBorder, cAccent, cAccentMuted, cText, cTextMuted;

    static GroovDeckLookAndFeel* activeInstance;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GroovDeckLookAndFeel)
};
