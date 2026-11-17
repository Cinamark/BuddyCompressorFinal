# Buddy Compressor

Buddy Compressor is a multiband compressor application made using C++ and JUCE. The goal was to make a simple audio touch up tool that can run on low-end devices, having multiple parameters concerning the dynamics of the original audio file, allowing for an in-depth dynamic processing experience

## Technical Implementation

- **Language:** C++17
- **Framework:** JUCE
- **Architecture:** Normalisation -> Noise Gate -> Multiband Compressor -> Limiter
- **Audio Processing:** Real-time digital signal processing
- **Plugin Format:** VST3

## Overview
<img width="1919" height="1080" alt="image" src="https://github.com/user-attachments/assets/f8ca96bb-d775-4c31-9d39-6b081b819fa1" />

<img width="844" height="636" alt="image" src="https://github.com/user-attachments/assets/1569699c-295e-4226-8788-f3d2e85f58d8" /><br>
Block Diagram of the audio processing

Files can be dragged and dropped into the top waveform section

<img width="1920" height="1080" alt="image" src="https://github.com/user-attachments/assets/00335d08-3e8b-4b4b-9837-e001893fa5f6" /><br>
App in use

NOTE: Currently the presets for Youtube and Podcast have not been uploaded to this version of the repo, which means the noise gate defaults to 0db on startup. If there are audible drop outs when first opening, that's probably why, so in just switching to "custom" and adjusting the Gate db/bypassing the Gate will fix this

## Building

### Requirements
- C++17 compatible compiler
- JUCE
- Projucer

### Instructions

1. Clone the repository.
2. Open the `.jucer` project file using <a href="https://juce.com/get-juce/">Projucer</a>.
3. Select your preferred IDE exporter or build configuration.
4. Set VST3 as the active plugin format.
5. Save the project and open the generated project in your IDE.
6. Build the project.

NOTE: Some modules act as dependencies, namely juce_dsp and juce_audio_processors. Can also easily be added via the Projucer

