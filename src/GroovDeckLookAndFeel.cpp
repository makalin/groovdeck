#include "GroovDeckLookAndFeel.h"

namespace
{
constexpr auto corner = 5.0f;
constexpr auto trackH = 3.5f;

struct Palette
{
    juce::uint32 bg, surf, elevated, brd, acc, accMut, txt, txtMut;
};

constexpr Palette kMidnight { 0xff14161c, 0xff1c2029, 0xff252a35, 0xff343b4a, 0xff3dd6c6, 0xff2a9d90, 0xffe8ebf2, 0xff8b93a6 };
constexpr Palette kOcean    { 0xff101820, 0xff152230, 0xff1c2d40, 0xff2a3f55, 0xff4da3ff, 0xff2e6fbf, 0xffe8f2ff, 0xff8aa0b8 };
constexpr Palette kEmber    { 0xff1a1210, 0xff261a16, 0xff35241e, 0xff4a342c, 0xffff8c5a, 0xffc45c38, 0xfff2e8e4, 0xffb89a8e };
constexpr Palette kDawn     { 0xfff0f2f7, 0xffe4e8f0, 0xffd8dde8, 0xffb8c0d0, 0xff2563eb, 0xff1d4ed8, 0xff111827, 0xff4b5563 };

Palette paletteForTheme (int t)
{
    switch (t)
    {
        case 1:  return kOcean;
        case 2:  return kEmber;
        case 3:  return kDawn;
        default: return kMidnight;
    }
}
}

GroovDeckLookAndFeel* GroovDeckLookAndFeel::activeInstance = nullptr;

GroovDeckLookAndFeel::GroovDeckLookAndFeel()
{
    activeInstance = this;
    applyTheme (0);
}

GroovDeckLookAndFeel::~GroovDeckLookAndFeel()
{
    if (activeInstance == this)
        activeInstance = nullptr;
}

juce::StringArray GroovDeckLookAndFeel::getThemeNames()
{
    return juce::StringArray ("Midnight", "Ocean", "Ember", "Dawn");
}

void GroovDeckLookAndFeel::applyTheme (int themeIndex)
{
    currentTheme = juce::jlimit (0, 3, themeIndex);
    const Palette p = paletteForTheme (currentTheme);

    cBackground = juce::Colour (p.bg);
    cSurface = juce::Colour (p.surf);
    cSurfaceElevated = juce::Colour (p.elevated);
    cBorder = juce::Colour (p.brd);
    cAccent = juce::Colour (p.acc);
    cAccentMuted = juce::Colour (p.accMut);
    cText = juce::Colour (p.txt);
    cTextMuted = juce::Colour (p.txtMut);

    syncColoursFromPalette();
}

void GroovDeckLookAndFeel::syncColoursFromPalette()
{
    setColour (juce::ResizableWindow::backgroundColourId, cBackground);
    setColour (juce::Label::textColourId, cText);
    setColour (juce::TextButton::buttonColourId, cSurfaceElevated);
    setColour (juce::TextButton::buttonOnColourId, cAccent.withAlpha (0.35f));
    setColour (juce::TextButton::textColourOffId, cText);
    setColour (juce::TextButton::textColourOnId, cText);
    setColour (juce::ToggleButton::textColourId, cText);
    setColour (juce::ToggleButton::tickColourId, cAccent);
    setColour (juce::ToggleButton::tickDisabledColourId, cTextMuted);
    setColour (juce::Slider::thumbColourId, cAccent);
    setColour (juce::Slider::trackColourId, cBorder.brighter (0.15f));
    setColour (juce::Slider::backgroundColourId, cSurface);
    setColour (juce::Slider::textBoxTextColourId, cText);
    setColour (juce::Slider::textBoxOutlineColourId, cBorder);
    setColour (juce::Slider::textBoxBackgroundColourId, cSurfaceElevated);
    setColour (juce::ListBox::backgroundColourId, cSurface);
    setColour (juce::ListBox::outlineColourId, cBorder);
}

juce::Colour GroovDeckLookAndFeel::background()        { return activeInstance ? activeInstance->cBackground        : juce::Colour (kMidnight.bg); }
juce::Colour GroovDeckLookAndFeel::surface()           { return activeInstance ? activeInstance->cSurface           : juce::Colour (kMidnight.surf); }
juce::Colour GroovDeckLookAndFeel::surfaceElevated()   { return activeInstance ? activeInstance->cSurfaceElevated   : juce::Colour (kMidnight.elevated); }
juce::Colour GroovDeckLookAndFeel::border()            { return activeInstance ? activeInstance->cBorder            : juce::Colour (kMidnight.brd); }
juce::Colour GroovDeckLookAndFeel::accent()            { return activeInstance ? activeInstance->cAccent            : juce::Colour (kMidnight.acc); }
juce::Colour GroovDeckLookAndFeel::accentMuted()       { return activeInstance ? activeInstance->cAccentMuted       : juce::Colour (kMidnight.accMut); }
juce::Colour GroovDeckLookAndFeel::text()              { return activeInstance ? activeInstance->cText              : juce::Colour (kMidnight.txt); }
juce::Colour GroovDeckLookAndFeel::textMuted()         { return activeInstance ? activeInstance->cTextMuted         : juce::Colour (kMidnight.txtMut); }

void GroovDeckLookAndFeel::drawModulePanel(juce::Graphics& g, juce::Rectangle<int> bounds, const juce::String& title)
{
    auto r = bounds.toFloat().reduced(0.5f);
    juce::ColourGradient grad(surfaceElevated(), r.getCentreX(), r.getY(),
                               surface(), r.getCentreX(), r.getBottom(), false);
    g.setGradientFill(grad);
    g.fillRoundedRectangle(r, corner);

    g.setColour(border());
    g.drawRoundedRectangle(r, corner, 1.0f);

    if (title.isNotEmpty())
    {
        auto titleBar = r.removeFromTop(17.0f);
        g.setColour(accent().withAlpha(0.12f));
        g.fillRect(titleBar);

        g.setColour(text());
        g.setFont(juce::Font(juce::FontOptions(10.0f, juce::Font::bold)));
        g.drawText(title, titleBar.reduced(6.0f, 0.0f).toNearestIntEdges(), juce::Justification::centredLeft, true);
    }
}

void GroovDeckLookAndFeel::drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
                                            float sliderPos, float minSliderPos, float maxSliderPos,
                                            const juce::Slider::SliderStyle style, juce::Slider& slider)
{
    if (style != juce::Slider::LinearHorizontal && style != juce::Slider::LinearVertical)
    {
        LookAndFeel_V4::drawLinearSlider(g, x, y, width, height, sliderPos, minSliderPos, maxSliderPos, style, slider);
        return;
    }

    const auto track = style == juce::Slider::LinearHorizontal
        ? juce::Rectangle<float>((float) x, (float) y + (float) height * 0.5f - trackH * 0.5f,
                                 (float) width, trackH)
        : juce::Rectangle<float>((float) x + (float) width * 0.5f - trackH * 0.5f, (float) y,
                                 trackH, (float) height);

    g.setColour(findColour(juce::Slider::backgroundColourId));
    g.fillRoundedRectangle(track, trackH * 0.5f);

    const bool isH = style == juce::Slider::LinearHorizontal;
    const float norm = (sliderPos - minSliderPos) / juce::jmax(1.0e-5f, maxSliderPos - minSliderPos);
    auto filled = track;

    if (isH)
        filled.setWidth(juce::jmax(trackH, norm * track.getWidth()));
    else
    {
        const float h = norm * track.getHeight();
        filled.setY(track.getBottom() - h);
        filled.setHeight(juce::jmax(trackH, h));
    }

    g.setColour(findColour(juce::Slider::trackColourId));
    g.fillRoundedRectangle(filled, trackH * 0.5f);

    const float thumb = 10.0f;
    juce::Rectangle<float> thumbR(thumb, thumb);
    if (isH)
        thumbR.setCentre(track.getX() + norm * track.getWidth(), track.getCentreY());
    else
        thumbR.setCentre(track.getCentreX(), track.getBottom() - norm * track.getHeight());

    g.setColour(findColour(juce::Slider::thumbColourId));
    g.fillEllipse(thumbR);

    g.setColour(border());
    g.drawEllipse(thumbR, 1.0f);
}

void GroovDeckLookAndFeel::drawButtonBackground(juce::Graphics& g, juce::Button& button,
                                                const juce::Colour& backgroundColour,
                                                bool shouldDrawButtonAsHighlighted,
                                                bool shouldDrawButtonAsDown)
{
    auto bounds = button.getLocalBounds().toFloat().reduced(0.5f);
    juce::Colour base = backgroundColour;

    if (shouldDrawButtonAsDown)
        base = base.darker(0.15f);
    else if (shouldDrawButtonAsHighlighted)
        base = base.brighter(0.12f);

    if (! button.isEnabled())
        base = base.withMultipliedAlpha(0.45f);

    g.setColour(base);
    g.fillRoundedRectangle(bounds, 4.0f);

    g.setColour(border());
    g.drawRoundedRectangle(bounds, 4.0f, 1.0f);
}

void GroovDeckLookAndFeel::drawToggleButton(juce::Graphics& g, juce::ToggleButton& button,
                                            bool shouldDrawButtonAsHighlighted,
                                            bool shouldDrawButtonAsDown)
{
    juce::ignoreUnused(shouldDrawButtonAsHighlighted, shouldDrawButtonAsDown);

    auto area = button.getLocalBounds().toFloat();
    const float box = juce::jmin(12.0f, area.getHeight() - 4.0f);
    auto textArea = area;
    auto boxBounds = textArea.removeFromLeft(box + 6.0f).withSizeKeepingCentre(box, box);

    g.setColour(surfaceElevated());
    g.fillRoundedRectangle(boxBounds, 3.0f);
    g.setColour(border());
    g.drawRoundedRectangle(boxBounds, 3.0f, 1.0f);

    if (button.getToggleState())
    {
        g.setColour(findColour(juce::ToggleButton::tickColourId));
        g.fillRoundedRectangle(boxBounds.reduced(3.0f), 1.5f);
    }

    g.setColour(button.findColour(juce::ToggleButton::textColourId));
    g.setFont(juce::Font(juce::FontOptions(10.0f)));
    g.drawFittedText(button.getButtonText(), textArea.toNearestIntEdges(), juce::Justification::centredLeft, 1);
}

juce::Font GroovDeckLookAndFeel::getLabelFont(juce::Label& label)
{
    juce::ignoreUnused(label);
    return juce::Font(juce::FontOptions(10.0f));
}

juce::Font GroovDeckLookAndFeel::getTextButtonFont(juce::TextButton&, int buttonHeight)
{
    return juce::Font(juce::FontOptions(juce::jmin(10.5f, (float) buttonHeight * 0.42f)));
}
