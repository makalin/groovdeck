#include "EffectsProcessor.h"

EffectsProcessor::EffectsProcessor()
    : delayWritePosition(0), isEnabled(true)
{
    // Initialize delay buffer
    delayBuffer.setSize(2, 44100 * 2); // 2 seconds at 44.1kHz
    delayBuffer.clear();
}

EffectsProcessor::~EffectsProcessor()
{
}

void EffectsProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    // Prepare DSP modules
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = 2;

    reverb.prepare(spec);
    delay.prepare(spec);
    filter.prepare(spec);
    distortion.prepare(spec);

    // Set default parameters
    setReverbParameters(0.5f, 0.5f, 0.33f, 0.67f);
    setDelayParameters(0.5f, 0.3f, 0.3f);
    setFilterParameters(1000.0f, 0.7f);
    setDistortionParameters(1.0f, 0.5f);
}

void EffectsProcessor::releaseResources()
{
    delayBuffer.clear();
}

void EffectsProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    if (!isEnabled)
        return;

    juce::dsp::AudioBlock<float> block(buffer);
    juce::dsp::ProcessContextReplacing<float> context(block);

    // Apply effects in series
    filter.process(context);
    delay.process(context);
    reverb.process(context);
    distortion.process(context);
}

void EffectsProcessor::setReverbParameters(float roomSize, float damping, float wetLevel, float dryLevel)
{
    juce::Reverb::Parameters params;
    params.roomSize = roomSize;
    params.damping = damping;
    params.wetLevel = wetLevel;
    params.dryLevel = dryLevel;
    reverb.setParameters(params);
}

void EffectsProcessor::setDelayParameters(float time, float feedback, float mix)
{
    delay.setDelay(time);
    delay.setFeedback(feedback);
    delay.setMix(mix);
}

void EffectsProcessor::setFilterParameters(float cutoff, float resonance)
{
    filter.setCutoffFrequency(cutoff);
    filter.setResonance(resonance);
}

void EffectsProcessor::setDistortionParameters(float drive, float mix)
{
    distortion.setGainDecibels(drive * 24.0f);
} 