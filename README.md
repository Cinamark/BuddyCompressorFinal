# Buddy Compressor

Buddy Compressor is a multiband compressor application made using C++ and JUCE. The goal was to make a simple audio touch up tool that can run on low-end devices, having multiple parameters concerning the dynamics of the original audio file, allowing for an in-depth dynamic processing experience

## Technical Implementation

- **Language:** C++17
- **Framework:** JUCE
- **Architecture:** Normalisation -> Noise Gate -> Multiband Compressor -> Limiter
- **Audio Processing:** Real-time digital signal processing
- **Plugin Format:** VST3
