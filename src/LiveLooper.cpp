#include "LiveLooper.h"

LiveLooper::LiveLooper()
    : sampleRate(44100.0), loopLength(4.0), currentPosition(0.0),
      loopStartSample(0), loopEndSample(0), recording(false), playing(false), loopGain(1.0f)
{
}

LiveLooper::~LiveLooper()
{
}

void LiveLooper::prepareToPlay(int samplesPerBlockExpected, double newSampleRate)
{
    sampleRate = newSampleRate;
    loopLength = 4.0; // Default 4 second loop
    updateLoopBounds();
    
    // Prepare buffers
    int maxSamples = static_cast<int>(sampleRate * 30.0); // 30 second max
    loopBuffer.setSize(2, maxSamples);
    recordingBuffer.setSize(2, maxSamples);
    loopBuffer.clear();
    recordingBuffer.clear();
}

void LiveLooper::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    if (!playing || !hasLoop())
        return;

    auto numSamples = bufferToFill.numSamples;
    auto* leftChannel = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
    auto* rightChannel = bufferToFill.buffer->getWritePointer(1, bufferToFill.startSample);

    for (int i = 0; i < numSamples; ++i)
    {
        int sampleIndex = static_cast<int>(currentPosition * sampleRate);
        
        if (sampleIndex >= loopEndSample)
        {
            sampleIndex = loopStartSample;
            currentPosition = loopStartSample / sampleRate;
        }

        if (sampleIndex < loopBuffer.getNumSamples())
        {
            leftChannel[i] = loopBuffer.getSample(0, sampleIndex) * loopGain;
            rightChannel[i] = loopBuffer.getSample(1, sampleIndex) * loopGain;
        }

        currentPosition += 1.0 / sampleRate;
    }
}

void LiveLooper::releaseResources()
{
    loopBuffer.clear();
    recordingBuffer.clear();
}

void LiveLooper::startRecording()
{
    if (!recording)
    {
        recording = true;
        currentPosition = 0.0;
        recordingBuffer.clear();
    }
}

void LiveLooper::stopRecording()
{
    if (recording)
    {
        recording = false;
        // Copy recorded audio to loop buffer
        int recordedSamples = static_cast<int>(currentPosition * sampleRate);
        if (recordedSamples > 0)
        {
            loopBuffer.clear();
            loopBuffer.setSize(2, recordedSamples);
            for (int ch = 0; ch < 2; ++ch)
            {
                loopBuffer.copyFrom(ch, 0, recordingBuffer, ch, 0, recordedSamples);
            }
            loopLength = currentPosition;
            updateLoopBounds();
        }
    }
}

void LiveLooper::startPlayback()
{
    if (hasLoop())
    {
        playing = true;
        currentPosition = loopStartSample / sampleRate;
    }
}

void LiveLooper::stopPlayback()
{
    playing = false;
}

void LiveLooper::clearLoop()
{
    playing = false;
    recording = false;
    loopBuffer.clear();
    currentPosition = 0.0;
}

void LiveLooper::setLoopLength(double lengthInSeconds)
{
    loopLength = lengthInSeconds;
    updateLoopBounds();
}

void LiveLooper::setLoopStart(double startTime)
{
    loopStartSample = static_cast<int>(startTime * sampleRate);
    updateLoopBounds();
}

void LiveLooper::setLoopEnd(double endTime)
{
    loopEndSample = static_cast<int>(endTime * sampleRate);
    updateLoopBounds();
}

void LiveLooper::reverseLoop()
{
    if (hasLoop())
    {
        juce::AudioBuffer<float> reversedBuffer(2, loopBuffer.getNumSamples());
        for (int ch = 0; ch < 2; ++ch)
        {
            for (int i = 0; i < loopBuffer.getNumSamples(); ++i)
            {
                reversedBuffer.setSample(ch, i, loopBuffer.getSample(ch, loopBuffer.getNumSamples() - 1 - i));
            }
        }
        loopBuffer = std::move(reversedBuffer);
    }
}

void LiveLooper::setLoopGain(float gain)
{
    loopGain = gain;
}

void LiveLooper::updateLoopBounds()
{
    loopStartSample = 0;
    loopEndSample = static_cast<int>(loopLength * sampleRate);
} 