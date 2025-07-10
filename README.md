# 🎛️ GroovDeck

GroovDeck is an open-source hardware + software groove production station inspired by the Akai MPC One, built around a Raspberry Pi with a touchscreen. It features a real-time audio player, mixer, sequencer, live looper, and effects processor with an Ableton Live-style workflow — fully designed for standalone music production.

---

## 🎯 Project Goals

- ✅ Touch-based UI for intuitive beatmaking and looping
- ✅ Real-time low-latency audio engine
- ✅ MIDI controller & sample pad support
- ✅ Track-based session view (Ableton-style)
- ✅ Scene and clip launching
- ✅ Time-stretching and pitch shifting
- ✅ Full offline standalone operation
- ✅ **Live looping and recording**
- ✅ **Step sequencer with pattern programming**
- ✅ **Real-time effects processing**
- ✅ **Multi-source audio mixing**

---

## 🧱 Architecture Overview

| Layer      | Technology       | Purpose                                      |
|------------|------------------|----------------------------------------------|
| OS         | Raspberry Pi OS (64-bit Lite) | Lightweight and audio-friendly environment |
| Core Audio | **C++ / JUCE**, RtAudio, RtMidi | Real-time audio playback & MIDI control    |
| DSP        | rubberband, libsamplerate      | High-quality pitch & tempo manipulation    |
| UI         | LVGL or Flutter (TBD)          | Custom touchscreen GUI                     |
| **Effects** | **JUCE DSP** | **Real-time audio effects processing** |
| **Sequencer** | **Custom C++** | **Pattern-based step sequencing** |
| **Live Looper** | **Custom C++** | **Real-time loop recording & playback** |

---

## 🛠 Hardware Requirements

- Raspberry Pi 4 or CM4 (2GB+ RAM)
- 7" Official Raspberry Pi Touchscreen
- USB Audio Interface (low-latency)
- SD card (32GB+ recommended)
- Optional:
  - MIDI controller / pad (USB or DIN)
  - GPIO or USB knob/slider for extra control

---

## 🚀 Getting Started

```bash
# Clone repo
git clone https://github.com/makalin/groovdeck.git
cd groovdeck

# Build instructions
./scripts/setup.sh
```

---

## 📦 Dependencies

* JUCE (GPL edition)
* RtAudio / RtMidi
* rubberband
* libsamplerate
* ALSA / JACK (Linux audio backend)
* LVGL or Flutter SDK (UI engine TBD)

---

## 🎵 Features

### Core Audio Engine
* [x] Multi-track playback
* [x] Per-track volume & mute
* [x] Loop triggering & launching
* [x] Real-time time-stretching
* [x] MIDI pad & controller input
* [x] Scene sequencer
* [x] Project/session saving

### **Live Looping System** 🎤
* [x] **Real-time audio recording**
* [x] **Seamless loop playback**
* [x] **Loop length adjustment (1-30 seconds)**
* [x] **Loop start/end point editing**
* [x] **Loop gain control**
* [x] **Loop reversal**
* [x] **Multiple loop layers**
* [x] **Live overdubbing**

### **Step Sequencer** 🥁
* [x] **16-step pattern sequencer**
* [x] **Variable tempo (60-200 BPM)**
* [x] **Adjustable step count (4-16 steps)**
* [x] **Pattern randomization**
* [x] **Pattern shifting (left/right)**
* [x] **Visual step grid interface**
* [x] **Real-time pattern editing**
* [x] **Velocity control per step**

### **Effects Processing** 🎛️
* [x] **Reverb with room size, damping, wet/dry controls**
* [x] **Delay with time, feedback, and mix parameters**
* [x] **Filter with cutoff and resonance**
* [x] **Distortion with drive and mix controls**
* [x] **Real-time parameter adjustment**
* [x] **Effect bypass functionality**
* [x] **Effects chain processing**

### **Advanced Features**
* [x] **Multi-source audio mixing**
* [x] **Real-time effects processing**
* [x] **Live performance capabilities**
* [x] **Pattern-based composition**
* [x] **Loop manipulation tools**
* [x] **Professional-grade audio engine**

---

## 🏗️ Project Structure

```
groovdeck/
├── src/
│   ├── MainApplication.h/cpp          # Main application entry point
│   ├── AudioEngine.h/cpp              # Core audio processing engine
│   ├── EffectsProcessor.h/cpp         # Real-time effects processing
│   ├── LiveLooper.h/cpp               # Live loop recording & playback
│   ├── Sequencer.h/cpp                # Step sequencer engine
│   ├── MainComponent.h/cpp            # Main UI component
│   ├── EffectsPanel.h/cpp             # Effects control interface
│   ├── LiveLoopPanel.h/cpp            # Live looping interface
│   └── SequencerPanel.h/cpp           # Sequencer control interface
├── scripts/
│   └── setup.sh                       # Build setup script
├── CMakeLists.txt                     # CMake build configuration
├── .gitignore                         # Git ignore patterns
└── README.md                          # This file
```

---

## 🎨 UI Components

### **Transport Controls**
- File loading and playback
- Volume control
- Loop toggle for file playback

### **Effects Panel**
- Reverb, Delay, Filter, Distortion controls
- Real-time parameter adjustment
- Effect bypass toggles

### **Live Loop Panel**
- Record/Play/Stop/Clear controls
- Loop length and gain adjustment
- Loop start/end point editing
- Status display

### **Sequencer Panel**
- 4x4 step button grid
- Tempo and step count controls
- Pattern manipulation tools
- Start/Stop/Reset functionality

---

## 💡 Inspiration

* Akai MPC One
* Ableton Live Session View
* Novation Circuit
* Mod Devices Pedals
* **Boss RC-505 Loop Station**
* **Arturia BeatStep Pro**
* **Native Instruments Maschine**

---

## 🧑‍💻 Developer Notes

### **Audio Architecture**
- **Multi-source mixing**: File playback, live loops, and sequencer output
- **Real-time processing**: All audio sources processed through effects chain
- **Low-latency design**: Optimized for live performance
- **JUCE DSP integration**: Professional-grade audio processing

### **Live Looping Engine**
- **Buffer-based recording**: Efficient memory management
- **Seamless looping**: Zero-latency loop transitions
- **Real-time manipulation**: Live parameter adjustment
- **Multi-layer support**: Multiple simultaneous loops

### **Sequencer Engine**
- **Step-based timing**: Precise rhythmic patterns
- **Pattern management**: Save, load, and manipulate patterns
- **Real-time control**: Live pattern editing
- **MIDI integration**: External clock and control support

### **Effects Processing**
- **Modular design**: Easy to add new effects
- **Parameter automation**: Real-time control
- **CPU optimization**: Efficient DSP algorithms
- **Professional quality**: Studio-grade audio processing

---

## 📜 License

GroovDeck is licensed under the MIT License. See [LICENSE](LICENSE) for details.

---

## ✨ Contributing

Pull requests are welcome! Ideas, bug reports, or feature requests? [Open an issue](https://github.com/makalin/groovdeck/issues).

### **Areas for Contribution**
- **Touch UI optimization** for Raspberry Pi touchscreen
- **MIDI mapping** and controller support
- **Additional effects** (chorus, flanger, compressor)
- **Sample library** management
- **Project file** format and saving
- **Hardware integration** for GPIO controls

---

## 📷 Screenshots

*(Coming soon – UI mockups and build photos)*

---

## 🎵 Use Cases

### **Live Performance**
- **Loop-based performance**: Create layered compositions live
- **Beat making**: Program and play rhythmic patterns
- **Effects processing**: Real-time audio manipulation
- **Multi-track mixing**: Combine multiple audio sources

### **Studio Production**
- **Pattern programming**: Create complex rhythmic sequences
- **Loop manipulation**: Edit and process recorded loops
- **Effects experimentation**: Try different effect combinations
- **Composition sketching**: Quick idea capture and development

### **Educational**
- **Music production learning**: Understand sequencing and effects
- **Live performance practice**: Develop looping skills
- **Audio engineering concepts**: Learn about DSP and mixing
- **Hardware integration**: Understand embedded audio systems

---

## 📡 Stay Connected

Follow development on GitHub and contribute to shaping the future of portable open-source beatmaking!

**🎛️ GroovDeck - Where Hardware Meets Creativity**
