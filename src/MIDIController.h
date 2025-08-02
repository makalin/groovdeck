#pragma once

#include <JuceHeader.h>

struct MIDIMapping
{
    int channel;
    int note;
    int cc;
    juce::String function;
    float minValue;
    float maxValue;
    
    MIDIMapping() : channel(0), note(0), cc(0), function(""), minValue(0.0f), maxValue(1.0f) {}
};

class MIDIController : public juce::MidiInputCallback
{
public:
    MIDIController();
    ~MIDIController() override;

    // MIDI device management
    void scanForDevices();
    bool connectToDevice(const juce::String& deviceName);
    void disconnectDevice();
    juce::StringArray getAvailableDevices() const;

    // MIDI callback
    void handleIncomingMidiMessage(juce::MidiInput* source, const juce::MidiMessage& message) override;

    // MIDI mapping
    void addNoteMapping(int channel, int note, const juce::String& function, float minVal = 0.0f, float maxVal = 1.0f);
    void addCCMapping(int channel, int cc, const juce::String& function, float minVal = 0.0f, float maxVal = 1.0f);
    void removeMapping(int channel, int note, int cc);
    void clearMappings();

    // MIDI output
    void sendNoteOn(int channel, int note, int velocity);
    void sendNoteOff(int channel, int note);
    void sendCC(int channel, int cc, int value);
    void sendProgramChange(int channel, int program);

    // MIDI clock
    void startClock();
    void stopClock();
    void setClockTempo(double bpm);
    double getClockTempo() const { return clockTempo; }

    // MIDI learn
    void enableLearnMode(bool enable) { learnMode = enable; }
    bool isLearnModeEnabled() const { return learnMode; }
    void setLearnFunction(const juce::String& function) { learnFunction = function; }

    // MIDI state
    bool isConnected() const { return midiInput != nullptr; }
    juce::String getConnectedDevice() const { return connectedDevice; }

    // MIDI message callback
    std::function<void(const juce::MidiMessage&)> onMIDIMessage;

private:
    std::unique_ptr<juce::MidiInput> midiInput;
    std::unique_ptr<juce::MidiOutput> midiOutput;
    juce::String connectedDevice;
    std::vector<MIDIMapping> mappings;
    bool learnMode;
    juce::String learnFunction;
    double clockTempo;
    juce::MidiMessageSequence clockSequence;

    void processMIDIMessage(const juce::MidiMessage& message);
    float normalizeValue(int value, float minVal, float maxVal);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MIDIController)
}; 