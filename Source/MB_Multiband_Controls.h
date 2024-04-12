/*
  ==============================================================================

    MB_Multiband_Controls.h
    Created: 12 Apr 2024 5:40:36am
    Author:  Simeon

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "ParamsMap.h"
#include "Slider_Attachment.h"
class MidBandCompSliders : public juce::Component
{
public:
    MidBandCompSliders(const juce::ValueTree& v, juce::UndoManager* uMgr)
        : ltree(v), undoMgr(*uMgr),

        threshAttach(ltree, &thresh, juce::Identifier{ juce::Identifier{ Params::GetParams().at(Params::Names::Threshold_Mid_Band)} }),
        ratAttach(ltree, &ratio, juce::Identifier{ juce::Identifier{ Params::GetParams().at(Params::Names::Ratio_Mid_Band)} }),
        attAttach(ltree, &att, juce::Identifier{ juce::Identifier{ Params::GetParams().at(Params::Names::Attack_Mid_Band)} }),
        relAttach(ltree, &rel, juce::Identifier{ juce::Identifier{ Params::GetParams().at(Params::Names::Release_Mid_Band)} })
    {
        thresh.setSliderStyle(juce::Slider::SliderStyle::Rotary);
        thresh.setValue(0);
        thresh.setRange(-60, 6, .5);
        ratio.setSliderStyle(juce::Slider::SliderStyle::Rotary);
        ratio.setRange(1, 100, 1);
        ratio.setValue(2);
        auto range = juce::NormalisableRange<double>(5, 500, 1);
        att.setSliderStyle(juce::Slider::SliderStyle::Rotary);
        att.setValue(5);
        att.setNormalisableRange(range);
        rel.setSliderStyle(juce::Slider::SliderStyle::Rotary);
        rel.setValue(5);
        rel.setNormalisableRange(range);

        addAndMakeVisible(&thresh);
        addAndMakeVisible(&ratio);
        addAndMakeVisible(&att);
        addAndMakeVisible(&rel);
    }

    void resized() override
    {
        auto bounds = getLocalBounds();
        juce::FlexBox flex;
        flex.flexDirection = juce::FlexBox::Direction::row;
        flex.flexWrap = juce::FlexBox::Wrap::noWrap;
        auto slider_size = bounds.getWidth() * 0.25f;


        flex.items.add(juce::FlexItem(thresh).withFlex(1.f));
        flex.items.add(juce::FlexItem(ratio).withFlex(1.f));
        flex.items.add(juce::FlexItem(att).withFlex(1.f));
        flex.items.add(juce::FlexItem(rel).withFlex(1.f));
        flex.performLayout(bounds);
    }
    juce::ValueTree ltree;
private:
    juce::Slider thresh, ratio, att, rel;
    TreeSliderAttachment threshAttach;
    TreeSliderAttachment ratAttach;
    TreeSliderAttachment attAttach;
    TreeSliderAttachment relAttach;


    juce::UndoManager& undoMgr;
};