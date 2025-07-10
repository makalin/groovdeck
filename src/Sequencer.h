#pragma once

#include <JuceHeader.h>

struct SequencerStep
{
    bool active;
    float velocity;
    double startTime;
    double duration;
    
    SequencerStep() : active(false), velocity(1.0f), startTime(0.0), duration(0.25) {}
};

class Sequencer : public juce::AudioSource
{
public:
    Sequencer();
    ~Sequencer() override;

    // AudioSource methods
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    // Sequencer control
    void start();
    void stop();
    void reset();
    void setTempo(double bpm);
    void setSteps(int numSteps);
    
    // Step control
    void setStepActive(int step, bool active);
    void setStepVelocity(int step, float velocity);
    void setStepStartTime(int step, double startTime);
    void setStepDuration(int step, double duration);
    
    // Sequencer state
    bool isPlaying() const { return playing; }
    int getCurrentStep() const { return currentStep; }
    double getTempo() const { return tempo; }
    int getNumSteps() const { return numSteps; }
    
    // Pattern management
    void clearPattern();
    void randomizePattern();
    void shiftPattern(int steps);

private:
    std::vector<SequencerStep> steps;
    double sampleRate;
    double tempo;
    int numSteps;
    int currentStep;
    double stepTime;
    double currentTime;
    bool playing;

    void updateStepTime();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Sequencer)
}; 