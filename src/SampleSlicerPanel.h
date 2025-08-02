#pragma once

#include <JuceHeader.h>
#include "SampleSlicer.h"

class SampleSlicerPanel : public juce::Component,
                         public juce::Button::Listener,
                         public juce::Slider::Listener
{
public:
    SampleSlicerPanel(SampleSlicer& slicer);
    ~SampleSlicerPanel() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void buttonClicked(juce::Button* button) override;
    void sliderValueChanged(juce::Slider* slider) override;

private:
    SampleSlicer& sampleSlicer;
    
    // Sample loading
    juce::TextButton loadSampleButton;
    juce::TextButton unloadSampleButton;
    
    // Slicing controls
    juce::TextButton autoSliceButton;
    juce::TextButton beatSliceButton;
    juce::TextButton transientSliceButton;
    juce::TextButton clearSlicesButton;
    
    // Slice parameters
    juce::Slider sliceLengthSlider;
    juce::Slider bpmSlider;
    juce::Slider sensitivitySlider;
    
    // Labels
    juce::Label sliceLengthLabel;
    juce::Label bpmLabel;
    juce::Label sensitivityLabel;
    
    // Slice display
    juce::ListBox sliceListBox;
    juce::Label sampleInfoLabel;
    
    // Slice playback
    juce::TextButton playSliceButton;
    juce::TextButton stopSliceButton;
    juce::Slider sliceGainSlider;
    juce::Label sliceGainLabel;
    
    void loadSample();
    void updateSampleInfo();
    void updateSliceList();
    void setupSlider(juce::Slider& slider, juce::Label& label, const juce::String& name,
                    double min, double max, double interval, double defaultValue);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SampleSlicerPanel)
}; 