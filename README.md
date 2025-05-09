# 🎛️ GroovDeck

GroovDeck is an open-source hardware + software groove production station inspired by the Akai MPC One, built around a Raspberry Pi with a touchscreen. It features a real-time audio player, mixer, and sequencer with an Ableton Live-style workflow — fully designed for standalone music production.

---

## 🎯 Project Goals

- ✅ Touch-based UI for intuitive beatmaking and looping
- ✅ Real-time low-latency audio engine
- ✅ MIDI controller & sample pad support
- ✅ Track-based session view (Ableton-style)
- ✅ Scene and clip launching
- ✅ Time-stretching and pitch shifting
- ✅ Full offline standalone operation

---

## 🧱 Architecture Overview

| Layer      | Technology       | Purpose                                      |
|------------|------------------|----------------------------------------------|
| OS         | Raspberry Pi OS (64-bit Lite) | Lightweight and audio-friendly environment |
| Core Audio | **C++ / JUCE**, RtAudio, RtMidi | Real-time audio playback & MIDI control    |
| DSP        | rubberband, libsamplerate      | High-quality pitch & tempo manipulation    |
| UI         | LVGL or Flutter (TBD)          | Custom touchscreen GUI                     |

---

## 🛠 Hardware Requirements

- Raspberry Pi 4 or CM4 (2GB+ RAM)
- 7” Official Raspberry Pi Touchscreen
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

# Build instructions (coming soon)
./scripts/setup.sh
````

---

## 📦 Dependencies

* JUCE (GPL edition)
* RtAudio / RtMidi
* rubberband
* libsamplerate
* ALSA / JACK (Linux audio backend)
* LVGL or Flutter SDK (UI engine TBD)

---

## 🧪 Features (MVP)

* [x] Multi-track playback
* [x] Per-track volume & mute
* [x] Loop triggering & launching
* [ ] Real-time time-stretching
* [ ] MIDI pad & controller input
* [ ] Scene sequencer
* [ ] Project/session saving

---

## 💡 Inspiration

* Akai MPC One
* Ableton Live Session View
* Novation Circuit
* Mod Devices Pedals

---

## 🧑‍💻 Developer Notes

Initial builds use a CMake-based project with JUCE 7. To keep things light on the Pi, audio backend uses ALSA or JACK, avoiding PulseAudio. UI performance is key; LVGL is being evaluated for ultra-fast touch response.

---

## 📜 License

GroovDeck is licensed under the MIT License. See [LICENSE](LICENSE) for details.

---

## ✨ Contributing

Pull requests are welcome! Ideas, bug reports, or feature requests? [Open an issue](https://github.com/makalin/groovdeck/issues).

---

## 📷 Screenshots

*(Coming soon – UI mockups and build photos)*

---

## 📡 Stay Connected

Follow development on GitHub and contribute to shaping the future of portable open-source beatmaking!
