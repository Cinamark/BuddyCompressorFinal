/*
  ==============================================================================

    Label_LnF.h
    Created: 16 Apr 2024 7:46:48am
    Author:  Simeon

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
class LabelLooknFeel : public juce::LookAndFeel_V4 {
public:
    LabelLooknFeel() {
        setColour(juce::Label::ColourIds::backgroundColourId, juce::Colours::darkgrey);
        setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
        setColour(juce::Label::ColourIds::outlineColourId, juce::Colours::white);
    }

    




};