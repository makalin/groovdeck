#pragma once

#include <JuceHeader.h>

/** Empty rack-style slot until a real module is wired in. */
class PlaceholderPanel final : public juce::Component
{
public:
    explicit PlaceholderPanel (juce::String shortDescription);

    void paint (juce::Graphics& g) override;

private:
    juce::String description;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaceholderPanel)
};
