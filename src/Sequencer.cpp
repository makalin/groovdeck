#include "Sequencer.h"
#include <random>

Sequencer::Sequencer()
    : sampleRate(44100.0), tempo(120.0), numSteps(16), currentStep(0),
      stepTime(0.0), currentTime(0.0), playing(false)
{
    steps.resize(numSteps);
    updateStepTime();
}

Sequencer::~Sequencer()
{
}

void Sequencer::prepareToPlay(int samplesPerBlockExpected, double newSampleRate)
{
    sampleRate = newSampleRate;
    updateStepTime();
}

void Sequencer::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    if (!playing)
        return;

    auto numSamples = bufferToFill.numSamples;
    auto* leftChannel = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
    auto* rightChannel = bufferToFill.buffer->getWritePointer(1, bufferToFill.startSample);

    for (int i = 0; i < numSamples; ++i)
    {
        // Check if we need to advance to next step
        if (currentTime >= stepTime)
        {
            currentStep = (currentStep + 1) % numSteps;
            currentTime = 0.0;
        }

        // Generate step trigger signal
        if (steps[currentStep].active && currentTime < 0.01) // Short trigger pulse
        {
            float velocity = steps[currentStep].velocity;
            leftChannel[i] = velocity * 0.5f;
            rightChannel[i] = velocity * 0.5f;
        }
        else
        {
            leftChannel[i] = 0.0f;
            rightChannel[i] = 0.0f;
        }

        currentTime += 1.0 / sampleRate;
    }
}

void Sequencer::releaseResources()
{
}

void Sequencer::start()
{
    playing = true;
    currentStep = 0;
    currentTime = 0.0;
}

void Sequencer::stop()
{
    playing = false;
}

void Sequencer::reset()
{
    currentStep = 0;
    currentTime = 0.0;
}

void Sequencer::setTempo(double bpm)
{
    tempo = bpm;
    updateStepTime();
}

void Sequencer::setSteps(int steps)
{
    numSteps = steps;
    this->steps.resize(numSteps);
    updateStepTime();
}

void Sequencer::setStepActive(int step, bool active)
{
    if (step >= 0 && step < numSteps)
        steps[step].active = active;
}

void Sequencer::setStepVelocity(int step, float velocity)
{
    if (step >= 0 && step < numSteps)
        steps[step].velocity = juce::jlimit(0.0f, 1.0f, velocity);
}

void Sequencer::setStepStartTime(int step, double startTime)
{
    if (step >= 0 && step < numSteps)
        steps[step].startTime = startTime;
}

void Sequencer::setStepDuration(int step, double duration)
{
    if (step >= 0 && step < numSteps)
        steps[step].duration = duration;
}

void Sequencer::clearPattern()
{
    for (auto& step : steps)
    {
        step.active = false;
        step.velocity = 1.0f;
        step.startTime = 0.0;
        step.duration = 0.25;
    }
}

void Sequencer::randomizePattern()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0f, 1.0f);

    for (auto& step : steps)
    {
        step.active = dis(gen) > 0.5f;
        step.velocity = dis(gen);
    }
}

void Sequencer::shiftPattern(int shiftSteps)
{
    std::vector<SequencerStep> tempSteps = steps;
    for (int i = 0; i < numSteps; ++i)
    {
        int newIndex = (i + shiftSteps + numSteps) % numSteps;
        steps[newIndex] = tempSteps[i];
    }
}

void Sequencer::updateStepTime()
{
    stepTime = 60.0 / tempo / 4.0; // 16th note duration
} 