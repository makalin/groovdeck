#include "AudioEngine.h"

AudioEngine::AudioEngine()
{
    formatManager.registerBasicFormats();
    deviceManager.initialise(2, 2, nullptr, true);
    deviceManager.addAudioCallback(this);
}

AudioEngine::~AudioEngine()
{
    deviceManager.removeAudioCallback(this);
    unloadAudioFile();
}

void AudioEngine::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    effectsProcessor.prepareToPlay(sampleRate, samplesPerBlockExpected);
    liveLooper.prepareToPlay(samplesPerBlockExpected, sampleRate);
    sequencer.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void AudioEngine::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    // Get audio from transport source (file playback)
    transportSource.getNextAudioBlock(bufferToFill);
    
    // Mix in live looper
    liveLooper.getNextAudioBlock(bufferToFill);
    
    // Mix in sequencer
    sequencer.getNextAudioBlock(bufferToFill);
    
    // Apply effects if enabled
    if (effectsProcessor.isEffectEnabled())
    {
        juce::MidiBuffer midiBuffer;
        effectsProcessor.processBlock(*bufferToFill.buffer, midiBuffer);
    }
}

void AudioEngine::releaseResources()
{
    transportSource.releaseResources();
    effectsProcessor.releaseResources();
    liveLooper.releaseResources();
    sequencer.releaseResources();
}

bool AudioEngine::loadAudioFile(const juce::File& file)
{
    unloadAudioFile();

    audioFileReader.reset(formatManager.createReaderFor(file));
    if (audioFileReader != nullptr)
    {
        audioSource.reset(new juce::AudioFormatReaderSource(audioFileReader.get(), true));
        transportSource.setSource(audioSource.get(), 0, nullptr, audioFileReader->sampleRate);
        return true;
    }
    return false;
}

void AudioEngine::unloadAudioFile()
{
    transportSource.setSource(nullptr);
    audioSource = nullptr;
    audioFileReader = nullptr;
}

void AudioEngine::startPlayback()
{
    transportSource.start();
}

void AudioEngine::stopPlayback()
{
    transportSource.stop();
}

void AudioEngine::setLooping(bool shouldLoop)
{
    if (audioSource != nullptr)
        audioSource->setLooping(shouldLoop);
}

void AudioEngine::setGain(float newGain)
{
    transportSource.setGain(newGain);
} 