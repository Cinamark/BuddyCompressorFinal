/*
  ==============================================================================

    Preset_Buttons.h
    Created: 14 Apr 2024 6:09:39pm
    Author:  Simeon

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "ParamsMap.h"
#include "Button_Attachment.h"
class PresetButtons : public juce::Component
{
public:
    PresetButtons(const juce::ValueTree& v, juce::UndoManager* uMgr)
        : ltree(v), undoMgr(*uMgr)

    {
        yt.setRadioGroupId(PresetButtonsID);
        podCast.setRadioGroupId(PresetButtonsID);
        custom.setRadioGroupId(PresetButtonsID);

        addAndMakeVisible(&yt);

        addAndMakeVisible(&podCast);
        addAndMakeVisible(&custom);
    }

    void resized() override
    {
        auto bounds = getLocalBounds();
        juce::FlexBox flex;
        flex.flexDirection = juce::FlexBox::Direction::row;
        flex.flexWrap = juce::FlexBox::Wrap::noWrap;
        auto slider_size = bounds.getWidth() * 0.25f;


        flex.items.add(juce::FlexItem(yt).withFlex(1.f));
        flex.items.add(juce::FlexItem(podCast).withFlex(1.f));
        flex.items.add(juce::FlexItem(custom).withFlex(1.f));
        flex.performLayout(bounds);
    }
    juce::ValueTree ltree;
private:
    juce::ToggleButton yt, podCast, custom;

    enum PresetRadioId
    {
        PresetButtonsID = 1011
    };
    juce::UndoManager& undoMgr;
};