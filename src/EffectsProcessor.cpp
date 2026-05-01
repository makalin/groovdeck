#include "EffectsProcessor.h"

EffectsProcessor::EffectsProcessor()
    : delayTimeSeconds(0.5f), delayFeedbackAmount(0.3f), delayWetMix(0.3f),
      isEnabled(true)
{
    filter.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
}

EffectsProcessor::~EffectsProcessor() = default;

void EffectsProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    storedSampleRate = sampleRate;

    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = (juce::uint32) juce::jmax(1, samplesPerBlock);
    spec.numChannels = 2;

    reverb.prepare(spec);
    filter.prepare(spec);
    distortion.prepare(spec);

    delayLine.prepare(spec);

    const int maxDelaySamples = juce::roundToInt(sampleRate * 2.0) + 32;
    delayLine.setMaximumDelayInSamples(juce::jmax(samplesPerBlock, maxDelaySamples));

    delayLine.reset();
    delayLine.setDelay(juce::jlimit(
        1.0f,
        juce::jmax(2.0f, (float) delayLine.getMaximumDelayInSamples() - 1.0f),
        delayTimeSeconds * (float) sampleRate));

    setReverbParameters(0.5f, 0.5f, 0.33f, 0.67f);
    setDelayParameters(delayTimeSeconds, delayFeedbackAmount, delayWetMix);
    setFilterParameters(1000.0f, 0.7f);
    setDistortionParameters(1.0f, 0.5f);
}

void EffectsProcessor::releaseResources()
{
    delayLine.reset();
    filter.reset();
}

void EffectsProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    if (! isEnabled)
        return;

    const unsigned nCh = juce::jmin((unsigned) buffer.getNumChannels(), 2u);
    if (nCh < 1)
        return;

    juce::dsp::AudioBlock<float> block(buffer);
    auto stereo = block.getSubsetChannelBlock(0, (size_t) nCh);
    juce::dsp::ProcessContextReplacing<float> context(stereo);

    filter.process(context);
    delayLine.process(context);
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

void EffectsProcessor::setDelayParameters(float timeInSeconds, float feedback, float mix)
{
    juce::ignoreUnused(feedback, mix);

    delayTimeSeconds = timeInSeconds;
    delayFeedbackAmount = feedback;
    delayWetMix = mix;

    if (storedSampleRate > 0.0 && delayLine.getMaximumDelayInSamples() > 0)
        delayLine.setDelay(juce::jlimit(
            1.0f,
            juce::jmax(2.0f, (float) delayLine.getMaximumDelayInSamples() - 1.0f),
            delayTimeSeconds * (float) storedSampleRate));
}

void EffectsProcessor::setFilterParameters(float cutoff, float resonance)
{
    filter.setCutoffFrequency(cutoff);
    filter.setResonance(resonance);
}

void EffectsProcessor::setDistortionParameters(float drive, float mix)
{
    juce::ignoreUnused(mix);
    distortion.setGainDecibels(drive * 24.0f);
}
