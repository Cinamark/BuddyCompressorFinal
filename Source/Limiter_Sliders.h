/*
  ==============================================================================

    Limiter_Sliders.h
    Created: 13 Apr 2024 7:57:00am
    Author:  Simeon

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "ParamsMap.h"
#include "Slider_Attachment.h"
class LimiterSliders : public juce::Component
{
public:
    LimiterSliders(const juce::ValueTree& v, juce::UndoManager* uMgr)
        : ltree(v), undo_Mgr(*uMgr),

        threshAttach(ltree, &thresh, juce::Identifier{ juce::Identifier{ Params::GetParams().at(Params::Names::Limiter_Threshold)} }, & undo_Mgr),
        relAttach(ltree, &rel, juce::Identifier{ juce::Identifier{ Params::GetParams().at(Params::Names::Limiter_Release)} }, & undo_Mgr)
    {
        thresh.setSliderStyle(juce::Slider::SliderStyle::Rotary);
        thresh.setValue(0);
        thresh.setRange(-60, 6, .5);
        thresh.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
        thresh.setTextValueSuffix("db");
        rel.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
        rel.setTextValueSuffix("ms");
        auto range = juce::NormalisableRange<double>(5, 500, 1);
        rel.setSliderStyle(juce::Slider::SliderStyle::Rotary);
        rel.setValue(5);
        rel.setNormalisableRange(range);

        thresh.onDragEnd = [this]() {undo_Mgr.beginNewTransaction(); };
        rel.onDragEnd = [this]() {undo_Mgr.beginNewTransaction(); };

        addAndMakeVisible(&thresh);
        addAndMakeVisible(&rel);
        addAndMakeVisible(&thresh_label);
        addAndMakeVisible(&rel_label);
    }

    void resized() override
    {
        auto bounds = getLocalBounds();


        juce::FlexBox flex;
        flex.flexDirection = juce::FlexBox::Direction::row;
        flex.flexWrap = juce::FlexBox::Wrap::noWrap;
        auto slider_size = bounds.getWidth() * 0.25f;


        flex.items.add(juce::FlexItem(thresh).withFlex(1.f));
        flex.items.add(juce::FlexItem(rel).withFlex(1.f));
        flex.performLayout(bounds);
    }
    juce::ValueTree ltree;
    juce::UndoManager& undo_Mgr;
private:
    juce::Slider thresh, rel;
    juce::Label thresh_label, rel_label;
    TreeSliderAttachment threshAttach;
    TreeSliderAttachment relAttach;


    
};
