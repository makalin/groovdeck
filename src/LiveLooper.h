#pragma once

#include <JuceHeader.h>

class LiveLooper : public juce::AudioSource
{
public:
    LiveLooper();
    ~LiveLooper() override;

    // AudioSource methods
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    // Loop control
    void startRecording();
    void stopRecording();
    void startPlayback();
    void stopPlayback();
    void clearLoop();
    void setLoopLength(double lengthInSeconds);
    
    // Loop state
    bool isRecording() const { return recording; }
    bool isPlaying() const { return playing; }
    bool hasLoop() const { return loopBuffer.getNumSamples() > 0; }
    double getCurrentPosition() const { return currentPosition; }
    double getLoopLength() const { return loopLength; }

    // Loop manipulation
    void setLoopStart(double startTime);
    void setLoopEnd(double endTime);
    void reverseLoop();
    void setLoopGain(float gain);

private:
    juce::AudioBuffer<float> loopBuffer;
    juce::AudioBuffer<float> recordingBuffer;
    double sampleRate;
    double loopLength;
    double currentPosition;
    int loopStartSample;
    int loopEndSample;
    bool recording;
    bool playing;
    float loopGain;

    void updateLoopBounds();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LiveLooper)
}; 