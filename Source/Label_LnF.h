/*
  ==============================================================================

    Label_LnF.h
    Created: 16 Apr 2024 7:46:48am
    Author:  Simeon

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
class DarkLabelLooknFeel : public juce::LookAndFeel_V4 {
public:
    DarkLabelLooknFeel() {
        setColour(juce::Label::ColourIds::backgroundColourId, juce::Colour::fromRGB(15, 15, 15));
        setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
        setColour(juce::Label::ColourIds::outlineColourId, juce::Colours::white);
    }

};

class LightLabelLooknFeel : public juce::LookAndFeel_V4 {
public:
    LightLabelLooknFeel() {
        setColour(juce::Label::ColourIds::backgroundColourId, juce::Colours::darkgrey);
        setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
        setColour(juce::Label::ColourIds::outlineColourId, juce::Colours::white);
    }
};