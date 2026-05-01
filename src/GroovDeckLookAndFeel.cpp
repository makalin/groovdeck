#include "GroovDeckLookAndFeel.h"

namespace
{
constexpr auto corner = 8.0f;
constexpr auto trackH = 5.0f;
}

GroovDeckLookAndFeel::GroovDeckLookAndFeel()
{
    setColour(juce::ResizableWindow::backgroundColourId, background());
    setColour(juce::Label::textColourId, text());
    setColour(juce::TextButton::buttonColourId, surfaceElevated());
    setColour(juce::TextButton::buttonOnColourId, accent().withAlpha(0.35f));
    setColour(juce::TextButton::textColourOffId, text());
    setColour(juce::TextButton::textColourOnId, text());
    setColour(juce::ToggleButton::textColourId, text());
    setColour(juce::ToggleButton::tickColourId, accent());
    setColour(juce::ToggleButton::tickDisabledColourId, textMuted());
    setColour(juce::Slider::thumbColourId, accent());
    setColour(juce::Slider::trackColourId, border().brighter(0.15f));
    setColour(juce::Slider::backgroundColourId, surface());
    setColour(juce::Slider::textBoxTextColourId, text());
    setColour(juce::Slider::textBoxOutlineColourId, border());
    setColour(juce::Slider::textBoxBackgroundColourId, surfaceElevated());
    setColour(juce::ListBox::backgroundColourId, surface());
    setColour(juce::ListBox::outlineColourId, border());
}

juce::Colour GroovDeckLookAndFeel::background() { return juce::Colour(0xff14161c); }
juce::Colour GroovDeckLookAndFeel::surface() { return juce::Colour(0xff1c2029); }
juce::Colour GroovDeckLookAndFeel::surfaceElevated() { return juce::Colour(0xff252a35); }
juce::Colour GroovDeckLookAndFeel::border() { return juce::Colour(0xff343b4a); }
juce::Colour GroovDeckLookAndFeel::accent() { return juce::Colour(0xff3dd6c6); }
juce::Colour GroovDeckLookAndFeel::accentMuted() { return juce::Colour(0xff2a9d90); }
juce::Colour GroovDeckLookAndFeel::text() { return juce::Colour(0xffe8ebf2); }
juce::Colour GroovDeckLookAndFeel::textMuted() { return juce::Colour(0xff8b93a6); }

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
        auto titleBar = r.removeFromTop(26.0f);
        g.setColour(accent().withAlpha(0.14f));
        g.fillRect(titleBar);

        g.setColour(text());
        g.setFont(juce::Font(juce::FontOptions(15.0f, juce::Font::bold)));
        g.drawText(title, titleBar.reduced(12.0f, 0.0f).toNearestIntEdges(), juce::Justification::centredLeft, true);
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

    const float thumb = 14.0f;
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
    g.fillRoundedRectangle(bounds, 6.0f);

    g.setColour(border());
    g.drawRoundedRectangle(bounds, 6.0f, 1.0f);
}

void GroovDeckLookAndFeel::drawToggleButton(juce::Graphics& g, juce::ToggleButton& button,
                                            bool shouldDrawButtonAsHighlighted,
                                            bool shouldDrawButtonAsDown)
{
    juce::ignoreUnused(shouldDrawButtonAsHighlighted, shouldDrawButtonAsDown);

    auto area = button.getLocalBounds().toFloat();
    const float box = juce::jmin(18.0f, area.getHeight() - 6.0f);
    auto textArea = area;
    auto boxBounds = textArea.removeFromLeft(box + 8.0f).withSizeKeepingCentre(box, box);

    g.setColour(surfaceElevated());
    g.fillRoundedRectangle(boxBounds, 4.0f);
    g.setColour(border());
    g.drawRoundedRectangle(boxBounds, 4.0f, 1.0f);

    if (button.getToggleState())
    {
        g.setColour(findColour(juce::ToggleButton::tickColourId));
        g.fillRoundedRectangle(boxBounds.reduced(4.0f), 2.0f);
    }

    g.setColour(button.findColour(juce::ToggleButton::textColourId));
    g.setFont(juce::Font(juce::FontOptions(13.0f)));
    g.drawFittedText(button.getButtonText(), textArea.toNearestIntEdges(), juce::Justification::centredLeft, 2);
}

juce::Font GroovDeckLookAndFeel::getLabelFont(juce::Label& label)
{
    juce::ignoreUnused(label);
    return juce::Font(juce::FontOptions(13.0f));
}
