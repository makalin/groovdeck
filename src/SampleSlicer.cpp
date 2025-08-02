#include "SampleSlicer.h"
#include <random>

SampleSlicer::SampleSlicer()
    : sampleRate(44100.0), sampleLength(0.0), currentSlice(-1), playing(false), globalGain(1.0f)
{
}

SampleSlicer::~SampleSlicer()
{
}

void SampleSlicer::prepareToPlay(int samplesPerBlockExpected, double newSampleRate)
{
    sampleRate = newSampleRate;
}

void SampleSlicer::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    if (!playing || currentSlice < 0 || currentSlice >= slices.size())
        return;

    const Slice& slice = slices[currentSlice];
    if (!slice.active)
        return;

    int startSample = static_cast<int>(slice.startTime * sampleRate);
    int endSample = static_cast<int>(slice.endTime * sampleRate);
    int sliceLength = endSample - startSample;

    if (startSample >= sampleBuffer.getNumSamples())
        return;

    auto numSamples = bufferToFill.numSamples;
    auto* leftChannel = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
    auto* rightChannel = bufferToFill.buffer->getWritePointer(1, bufferToFill.startSample);

    for (int i = 0; i < numSamples; ++i)
    {
        int sampleIndex = startSample + i;
        if (sampleIndex < endSample && sampleIndex < sampleBuffer.getNumSamples())
        {
            leftChannel[i] = sampleBuffer.getSample(0, sampleIndex) * globalGain;
            rightChannel[i] = sampleBuffer.getSample(1, sampleIndex) * globalGain;
        }
        else
        {
            leftChannel[i] = 0.0f;
            rightChannel[i] = 0.0f;
        }
    }
}

void SampleSlicer::releaseResources()
{
    sampleBuffer.clear();
}

bool SampleSlicer::loadSample(const juce::File& file)
{
    juce::AudioFormatManager formatManager;
    formatManager.registerBasicFormats();
    
    std::unique_ptr<juce::AudioFormatReader> reader(formatManager.createReaderFor(file));
    if (reader != nullptr)
    {
        sampleLength = reader->lengthInSamples / reader->sampleRate;
        sampleBuffer.setSize(2, reader->lengthInSamples);
        reader->read(&sampleBuffer, 0, reader->lengthInSamples, 0, true, true);
        return true;
    }
    return false;
}

void SampleSlicer::unloadSample()
{
    sampleBuffer.clear();
    clearSlices();
    sampleLength = 0.0;
}

void SampleSlicer::autoSlice(double sliceLength)
{
    clearSlices();
    if (sampleLength <= 0.0) return;

    int numSlices = static_cast<int>(sampleLength / sliceLength);
    for (int i = 0; i < numSlices; ++i)
    {
        double startTime = i * sliceLength;
        double endTime = (i + 1) * sliceLength;
        if (endTime > sampleLength) endTime = sampleLength;
        
        addSlice(startTime, endTime, "Slice " + juce::String(i + 1));
    }
}

void SampleSlicer::sliceAtBeats(double bpm)
{
    clearSlices();
    if (sampleLength <= 0.0 || bpm <= 0.0) return;

    double beatLength = 60.0 / bpm;
    int numBeats = static_cast<int>(sampleLength / beatLength);
    
    for (int i = 0; i < numBeats; ++i)
    {
        double startTime = i * beatLength;
        double endTime = (i + 1) * beatLength;
        if (endTime > sampleLength) endTime = sampleLength;
        
        addSlice(startTime, endTime, "Beat " + juce::String(i + 1));
    }
}

void SampleSlicer::sliceAtTransients(double sensitivity)
{
    clearSlices();
    if (sampleBuffer.getNumSamples() == 0) return;

    std::vector<int> transientPoints;
    int windowSize = 1024;
    float threshold = sensitivity * 0.1f;

    for (int i = windowSize; i < sampleBuffer.getNumSamples() - windowSize; i += windowSize)
    {
        float energy = 0.0f;
        for (int j = 0; j < windowSize; ++j)
        {
            float sample = sampleBuffer.getSample(0, i + j);
            energy += sample * sample;
        }
        energy /= windowSize;

        if (energy > threshold)
        {
            transientPoints.push_back(i);
        }
    }

    // Create slices from transient points
    for (size_t i = 0; i < transientPoints.size(); ++i)
    {
        double startTime = transientPoints[i] / sampleRate;
        double endTime = (i + 1 < transientPoints.size()) ? 
                        transientPoints[i + 1] / sampleRate : sampleLength;
        
        addSlice(startTime, endTime, "Transient " + juce::String(i + 1));
    }
}

void SampleSlicer::addSlice(double startTime, double endTime, const juce::String& name)
{
    Slice slice;
    slice.startTime = startTime;
    slice.endTime = endTime;
    slice.name = name.isEmpty() ? "Slice " + juce::String(slices.size() + 1) : name;
    slice.active = true;
    slices.push_back(slice);
}

void SampleSlicer::removeSlice(int index)
{
    if (index >= 0 && index < slices.size())
    {
        slices.erase(slices.begin() + index);
    }
}

void SampleSlicer::clearSlices()
{
    slices.clear();
    currentSlice = -1;
    playing = false;
}

void SampleSlicer::playSlice(int index)
{
    if (index >= 0 && index < slices.size())
    {
        currentSlice = index;
        playing = true;
    }
}

void SampleSlicer::stopSlice()
{
    playing = false;
}

void SampleSlicer::setSliceGain(int index, float gain)
{
    if (index >= 0 && index < slices.size())
    {
        globalGain = gain;
    }
}

void SampleSlicer::setSlicePitch(int index, float pitch)
{
    // TODO: Implement pitch shifting
}

void SampleSlicer::setSliceSpeed(int index, float speed)
{
    // TODO: Implement speed control
}

const Slice& SampleSlicer::getSlice(int index) const
{
    static Slice emptySlice;
    if (index >= 0 && index < slices.size())
    {
        return slices[index];
    }
    return emptySlice;
}

void SampleSlicer::setSliceActive(int index, bool active)
{
    if (index >= 0 && index < slices.size())
    {
        slices[index].active = active;
    }
}

void SampleSlicer::randomizeSliceOrder()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(slices.begin(), slices.end(), gen);
}

void SampleSlicer::reverseSliceOrder()
{
    std::reverse(slices.begin(), slices.end());
}

void SampleSlicer::updateSliceTimes()
{
    // Update slice timing if needed
} 