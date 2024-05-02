/*
  ==============================================================================

    FrequencySliders.h
    Created: 12 Apr 2024 8:16:46am
    Author:  Simeon

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "ParamsMap.h"
#include "Slider_Attachment.h"

class FrequencySliders : public juce::Component
{
public:
    FrequencySliders(const juce::ValueTree& v, juce::UndoManager* uMgr)
        : ltree(v), undo_Mgr(*uMgr),

        low_midAttach(ltree, &low_mid, juce::Identifier{ juce::Identifier{ Params::GetParams().at(Params::Names::Low_Mid_Crossover_Freq)} }, & undo_Mgr),
        mid_highAttach(ltree, &mid_high, juce::Identifier{ juce::Identifier{ Params::GetParams().at(Params::Names::Mid_High_Crossover_Freq)} }, & undo_Mgr)
    {
        low_mid.setSliderStyle(juce::Slider::SliderStyle::Rotary);
        low_mid.setValue(400);
        low_mid.setRange(20, 900, 1);
        low_mid.setTextValueSuffix("Hz");
        mid_high.setSliderStyle(juce::Slider::SliderStyle::Rotary);
        mid_high.setRange(1000, 20000, 1);
        mid_high.setValue(2000);
        mid_high.setTextValueSuffix("Hz");

        mid_high.onDragEnd = [this]() {undo_Mgr.beginNewTransaction(); };
        low_mid.onDragEnd = [this]() {undo_Mgr.beginNewTransaction(); };

        addAndMakeVisible(&low_mid);
        addAndMakeVisible(&mid_high);
    }

    void resized() override
    {
        auto bounds = getLocalBounds();
        juce::FlexBox flex;
        flex.flexDirection = juce::FlexBox::Direction::row;
        flex.flexWrap = juce::FlexBox::Wrap::noWrap;
        auto slider_size = bounds.getWidth() * 0.25f;


        flex.items.add(juce::FlexItem(low_mid).withFlex(1.f));
        flex.items.add(juce::FlexItem(mid_high).withFlex(1.f));
        flex.performLayout(bounds);
    }
    juce::UndoManager& undo_Mgr;
    juce::ValueTree ltree;

private:
    juce::Slider low_mid, mid_high;
    TreeSliderAttachment low_midAttach;
    TreeSliderAttachment mid_highAttach;


    
};