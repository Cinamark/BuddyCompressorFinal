/*
  ==============================================================================

    Meter.h
    Created: 1 Apr 2024 3:44:07pm
    Author:  Simeon

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

namespace GUI {
    class Horiz_Meter : public juce::Component, public juce::Timer
    {
    public:
        Horiz_Meter(std::function<float()>&& valueFunction) : valueSupplier(std::move(valueFunction)) {
            startTimerHz(24);
        }
        void paint(juce::Graphics& g) override
        {
            const auto level = valueSupplier();
            auto bounds = getLocalBounds().toFloat();

            g.setColour(juce::Colours::darkgrey);
            g.fillRect(bounds);

            if (level < 0)
                g.setColour(juce::Colours::white);
            else if (level > 0)
                g.setColour(juce::Colours::firebrick);
            const auto scaledY = juce::jmap(level, -60.f, 6.f, 0.f, static_cast<float>(getHeight()));
            g.fillRect(bounds.removeFromBottom(scaledY));

            


        }

        void resized() override
        {
            const auto bounds = getLocalBounds().toFloat();
            gradient = juce::ColourGradient{
                juce::Colours::green,
                bounds.getBottomLeft(),
                juce::Colours::red,
                bounds.getTopLeft(),
                false
            };

            gradient.addColour(0.5, juce::Colours::yellow);
        }

        void timerCallback() override
        {
            repaint();
        }
    private:
        std::function<float()> valueSupplier;
        juce::ColourGradient gradient;
    };
}
