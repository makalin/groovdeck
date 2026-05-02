#pragma once

#include <JuceHeader.h>

/** Identifies a floating tool panel (real modules + placeholder rack slots). */
enum class ModuleId : int
{
    Effects = 0,
    LiveLoop,
    Sequencer,
    SampleSlicer,
    RackMixer,
    RackRouting,
    numModules
};

inline int moduleIdToIndex (ModuleId id) { return static_cast<int> (id); }

inline juce::String moduleIdToKey (ModuleId id)
{
    switch (id)
    {
        case ModuleId::Effects:       return "effects";
        case ModuleId::LiveLoop:      return "liveLoop";
        case ModuleId::Sequencer:     return "sequencer";
        case ModuleId::SampleSlicer:  return "slicer";
        case ModuleId::RackMixer:     return "rackMixer";
        case ModuleId::RackRouting:   return "rackRouting";
        default:                      return {};
    }
}

inline juce::String moduleIdToTitle (ModuleId id)
{
    switch (id)
    {
        case ModuleId::Effects:       return "Mix & effects";
        case ModuleId::LiveLoop:      return "Live looper";
        case ModuleId::Sequencer:     return "Step sequencer";
        case ModuleId::SampleSlicer:  return "Sample slicer";
        case ModuleId::RackMixer:     return "Mixer rack";
        case ModuleId::RackRouting:   return "Routing / I/O";
        default:                      return "Panel";
    }
}
