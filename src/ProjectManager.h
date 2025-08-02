#pragma once

#include <JuceHeader.h>
#include "AudioEngine.h"
#include "LiveLooper.h"
#include "Sequencer.h"
#include "SampleSlicer.h"

struct ProjectData
{
    juce::String projectName;
    juce::String projectPath;
    double tempo;
    double masterGain;
    
    // Audio file references
    juce::StringArray audioFiles;
    
    // Effects settings
    struct EffectsSettings
    {
        bool reverbEnabled;
        float reverbRoomSize;
        float reverbDamping;
        float reverbWetLevel;
        float reverbDryLevel;
        
        bool delayEnabled;
        float delayTime;
        float delayFeedback;
        float delayMix;
        
        bool filterEnabled;
        float filterCutoff;
        float filterResonance;
        
        bool distortionEnabled;
        float distortionDrive;
        float distortionMix;
    } effects;
    
    // Sequencer data
    struct SequencerData
    {
        int numSteps;
        double tempo;
        std::vector<bool> stepStates;
        std::vector<float> stepVelocities;
    } sequencer;
    
    // Live looper data
    struct LooperData
    {
        double loopLength;
        float loopGain;
        double loopStart;
        double loopEnd;
        bool hasLoop;
    } looper;
    
    // Sample slicer data
    struct SlicerData
    {
        juce::String sampleFile;
        std::vector<juce::String> sliceNames;
        std::vector<double> sliceStartTimes;
        std::vector<double> sliceEndTimes;
    } slicer;
    
    ProjectData() : tempo(120.0), masterGain(1.0)
    {
        effects.reverbEnabled = false;
        effects.reverbRoomSize = 0.5f;
        effects.reverbDamping = 0.5f;
        effects.reverbWetLevel = 0.33f;
        effects.reverbDryLevel = 0.67f;
        
        effects.delayEnabled = false;
        effects.delayTime = 0.5f;
        effects.delayFeedback = 0.3f;
        effects.delayMix = 0.3f;
        
        effects.filterEnabled = false;
        effects.filterCutoff = 1000.0f;
        effects.filterResonance = 0.7f;
        
        effects.distortionEnabled = false;
        effects.distortionDrive = 1.0f;
        effects.distortionMix = 0.5f;
        
        sequencer.numSteps = 16;
        sequencer.tempo = 120.0;
        sequencer.stepStates.resize(16, false);
        sequencer.stepVelocities.resize(16, 1.0f);
        
        looper.loopLength = 4.0;
        looper.loopGain = 1.0f;
        looper.loopStart = 0.0;
        looper.loopEnd = 4.0;
        looper.hasLoop = false;
    }
};

class ProjectManager
{
public:
    ProjectManager();
    ~ProjectManager();

    // Project management
    bool createProject(const juce::String& name, const juce::File& directory);
    bool saveProject(const juce::File& file);
    bool loadProject(const juce::File& file);
    bool closeProject();
    
    // Project state
    bool hasProject() const { return currentProject != nullptr; }
    juce::String getProjectName() const;
    juce::File getProjectFile() const;
    
    // Data export/import
    void exportProjectData(ProjectData& data);
    void importProjectData(const ProjectData& data);
    
    // Auto-save
    void enableAutoSave(bool enable);
    void setAutoSaveInterval(int minutes);
    void performAutoSave();

private:
    std::unique_ptr<ProjectData> currentProject;
    juce::File projectFile;
    bool autoSaveEnabled;
    int autoSaveInterval;
    juce::Timer autoSaveTimer;
    
    void saveToFile(const juce::File& file, const ProjectData& data);
    bool loadFromFile(const juce::File& file, ProjectData& data);
    juce::var projectDataToVar(const ProjectData& data);
    bool varToProjectData(const juce::var& var, ProjectData& data);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ProjectManager)
}; 