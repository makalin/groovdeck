#include "MIDIController.h"

MIDIController::MIDIController()
    : learnMode(false), clockTempo(120.0)
{
    scanForDevices();
}

MIDIController::~MIDIController()
{
    disconnectDevice();
}

void MIDIController::scanForDevices()
{
    // MIDI devices are automatically detected by JUCE
}

bool MIDIController::connectToDevice(const juce::String& deviceName)
{
    disconnectDevice();

    auto devices = juce::MidiInput::getAvailableDevices();
    for (const auto& device : devices)
    {
        if (device.name == deviceName)
        {
            midiInput = juce::MidiInput::openDevice(device.identifier, this);
            if (midiInput != nullptr)
            {
                midiInput->start();
                connectedDevice = deviceName;
                return true;
            }
        }
    }
    return false;
}

void MIDIController::disconnectDevice()
{
    if (midiInput != nullptr)
    {
        midiInput->stop();
        midiInput = nullptr;
    }
    connectedDevice = "";
}

juce::StringArray MIDIController::getAvailableDevices() const
{
    juce::StringArray deviceNames;
    auto devices = juce::MidiInput::getAvailableDevices();
    for (const auto& device : devices)
    {
        deviceNames.add(device.name);
    }
    return deviceNames;
}

void MIDIController::handleIncomingMidiMessage(juce::MidiInput* source, const juce::MidiMessage& message)
{
    processMIDIMessage(message);
    
    if (onMIDIMessage)
    {
        onMIDIMessage(message);
    }
}

void MIDIController::addNoteMapping(int channel, int note, const juce::String& function, float minVal, float maxVal)
{
    MIDIMapping mapping;
    mapping.channel = channel;
    mapping.note = note;
    mapping.function = function;
    mapping.minValue = minVal;
    mapping.maxValue = maxVal;
    mappings.push_back(mapping);
}

void MIDIController::addCCMapping(int channel, int cc, const juce::String& function, float minVal, float maxVal)
{
    MIDIMapping mapping;
    mapping.channel = channel;
    mapping.cc = cc;
    mapping.function = function;
    mapping.minValue = minVal;
    mapping.maxValue = maxVal;
    mappings.push_back(mapping);
}

void MIDIController::removeMapping(int channel, int note, int cc)
{
    mappings.erase(
        std::remove_if(mappings.begin(), mappings.end(),
            [channel, note, cc](const MIDIMapping& mapping) {
                return mapping.channel == channel && 
                       mapping.note == note && 
                       mapping.cc == cc;
            }),
        mappings.end()
    );
}

void MIDIController::clearMappings()
{
    mappings.clear();
}

void MIDIController::sendNoteOn(int channel, int note, int velocity)
{
    if (midiOutput != nullptr)
    {
        midiOutput->sendMessageNow(juce::MidiMessage::noteOn(channel, note, (juce::uint8)velocity));
    }
}

void MIDIController::sendNoteOff(int channel, int note)
{
    if (midiOutput != nullptr)
    {
        midiOutput->sendMessageNow(juce::MidiMessage::noteOff(channel, note));
    }
}

void MIDIController::sendCC(int channel, int cc, int value)
{
    if (midiOutput != nullptr)
    {
        midiOutput->sendMessageNow(juce::MidiMessage::controllerEvent(channel, cc, value));
    }
}

void MIDIController::sendProgramChange(int channel, int program)
{
    if (midiOutput != nullptr)
    {
        midiOutput->sendMessageNow(juce::MidiMessage::programChange(channel, program));
    }
}

void MIDIController::startClock()
{
    // Start MIDI clock
    if (midiOutput != nullptr)
    {
        midiOutput->sendMessageNow(juce::MidiMessage::midiClock());
    }
}

void MIDIController::stopClock()
{
    // Stop MIDI clock
    if (midiOutput != nullptr)
    {
        midiOutput->sendMessageNow(juce::MidiMessage::midiStop());
    }
}

void MIDIController::setClockTempo(double bpm)
{
    clockTempo = bpm;
}

void MIDIController::processMIDIMessage(const juce::MidiMessage& message)
{
    if (learnMode && !learnFunction.isEmpty())
    {
        // Handle MIDI learn mode
        if (message.isNoteOn())
        {
            addNoteMapping(message.getChannel(), message.getNoteNumber(), learnFunction);
            learnMode = false;
            learnFunction = "";
        }
        else if (message.isController())
        {
            addCCMapping(message.getChannel(), message.getControllerNumber(), learnFunction);
            learnMode = false;
            learnFunction = "";
        }
        return;
    }

    // Process mapped MIDI messages
    for (const auto& mapping : mappings)
    {
        if (message.isNoteOn() && mapping.note > 0)
        {
            if (message.getChannel() == mapping.channel && 
                message.getNoteNumber() == mapping.note)
            {
                float value = normalizeValue(message.getVelocity(), mapping.minValue, mapping.maxValue);
                // Handle the mapped function here
            }
        }
        else if (message.isController() && mapping.cc > 0)
        {
            if (message.getChannel() == mapping.channel && 
                message.getControllerNumber() == mapping.cc)
            {
                float value = normalizeValue(message.getControllerValue(), mapping.minValue, mapping.maxValue);
                // Handle the mapped function here
            }
        }
    }
}

float MIDIController::normalizeValue(int value, float minVal, float maxVal)
{
    return minVal + (maxVal - minVal) * (value / 127.0f);
} 