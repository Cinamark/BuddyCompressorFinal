/*
  ==============================================================================

    Placeholder_Comp.h
    Created: 31 Mar 2024 7:54:46am
    Author:  Simeon

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
struct Placeholder : juce::Component
{
public:
    Placeholder(){
        juce::Random r;
        customColour = juce::Colour(r.nextInt(255), r.nextInt(255), r.nextInt(255));
    }
    void paint(juce::Graphics& g) override
    {
        //g.fillAll(customColour);
    }

    juce::Colour customColour;
};
