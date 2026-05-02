#include "PlaceholderPanel.h"
#include "GroovDeckLookAndFeel.h"

PlaceholderPanel::PlaceholderPanel (juce::String shortDescription)
    : description (std::move (shortDescription))
{
}

void PlaceholderPanel::paint (juce::Graphics& g)
{
    GroovDeckLookAndFeel::drawModulePanel (g, getLocalBounds(), "Coming soon");
    auto inner = getLocalBounds().reduced (10, 22);
    g.setColour (GroovDeckLookAndFeel::textMuted());
    g.setFont (juce::Font (juce::FontOptions (11.0f)));
    g.drawFittedText (description, inner, juce::Justification::centred, 4);
}
