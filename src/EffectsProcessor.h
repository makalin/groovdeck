#pragma once

#include <JuceHeader.h>

class EffectsProcessor : public juce::AudioProcessor
{
public:
    EffectsProcessor();
    ~EffectsProcessor() override;

    // AudioProcessor methods
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    // Effect controls
    void setReverbParameters(float roomSize, float damping, float wetLevel, float dryLevel);
    void setDelayParameters(float time, float feedback, float mix);
    void setFilterParameters(float cutoff, float resonance);
    void setDistortionParameters(float drive, float mix);

    // Effect bypass
    void setEffectEnabled(bool enabled) { isEnabled = enabled; }
    bool isEffectEnabled() const { return isEnabled; }

    // AudioProcessor interface
    const juce::String getName() const override { return "EffectsProcessor"; }
    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }
    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int) override {}
    const juce::String getProgramName(int) override { return {}; }
    void changeProgramName(int, const juce::String&) override {}
    bool hasEditor() const override { return false; }
    juce::AudioProcessorEditor* createEditor() override { return nullptr; }
    void getStateInformation(juce::MemoryBlock&) override {}
    void setStateInformation(const void*, int) override {}

private:
    juce::dsp::Reverb reverb;
    juce::dsp::Delay<float> delay;
    juce::dsp::StateVariableFilter::Filter<float> filter;
    juce::dsp::Gain<float> distortion;

    juce::AudioBuffer<float> delayBuffer;
    int delayWritePosition;
    bool isEnabled;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EffectsProcessor)
}; 