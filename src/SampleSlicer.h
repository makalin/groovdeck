#pragma once

#include <JuceHeader.h>

struct Slice
{
    double startTime;
    double endTime;
    juce::String name;
    bool active;
    
    Slice() : startTime(0.0), endTime(1.0), name("Slice"), active(true) {}
};

class SampleSlicer : public juce::AudioSource
{
public:
    SampleSlicer();
    ~SampleSlicer() override;

    // AudioSource methods
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    // Sample loading
    bool loadSample(const juce::File& file);
    void unloadSample();

    // Slicing functions
    void autoSlice(double sliceLength);
    void sliceAtBeats(double bpm);
    void sliceAtTransients(double sensitivity);
    void addSlice(double startTime, double endTime, const juce::String& name = "");
    void removeSlice(int index);
    void clearSlices();

    // Slice playback
    void playSlice(int index);
    void stopSlice();
    void setSliceGain(int index, float gain);
    void setSlicePitch(int index, float pitch);
    void setSliceSpeed(int index, float speed);

    // Slice management
    int getNumSlices() const { return slices.size(); }
    const Slice& getSlice(int index) const;
    void setSliceActive(int index, bool active);
    void randomizeSliceOrder();
    void reverseSliceOrder();

    // Sample info
    double getSampleLength() const { return sampleLength; }
    double getSampleRate() const { return sampleRate; }
    bool hasSample() const { return sampleBuffer.getNumSamples() > 0; }

private:
    juce::AudioBuffer<float> sampleBuffer;
    std::vector<Slice> slices;
    double sampleRate;
    double sampleLength;
    int currentSlice;
    bool playing;
    float globalGain;

    void updateSliceTimes();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SampleSlicer)
}; 