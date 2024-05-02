/*
  ==============================================================================

    APCompressor_Sliders.h
    Created: 11 Apr 2024 6:04:25pm
    Author:  Simeon

  ==============================================================================
*/


#pragma once
#include <JuceHeader.h>
#include "ParamsMap.h"
#include "Slider_Attachment.h"
class APCompressorSliders : public juce::Component
{
public:
    APCompressorSliders(const juce::ValueTree& v, juce::UndoManager* uMgr)
        : ltree(v), undo_Mgr(*uMgr),

        threshAttach(ltree, &thresh, juce::Identifier{ juce::Identifier{ Params::GetParams().at(Params::Names::Threshold_AllPass)} }, &undo_Mgr),
        ratAttach(ltree, &ratio, juce::Identifier{ juce::Identifier{ Params::GetParams().at(Params::Names::Ratio_AllPass)} }, & undo_Mgr),
        attAttach(ltree, &att, juce::Identifier{ juce::Identifier{ Params::GetParams().at(Params::Names::Attack_AllPass)} }, & undo_Mgr),
        relAttach(ltree, &rel, juce::Identifier{ juce::Identifier{ Params::GetParams().at(Params::Names::Release_AllPass)} }, & undo_Mgr)
    {
        thresh.setSliderStyle(juce::Slider::SliderStyle::Rotary);
        thresh.setValue(0);
        thresh.setRange(-60, 6, .5);
        thresh.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
        thresh.setTextValueSuffix("db");
        ratio.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
        ratio.setTextValueSuffix(":1");
        att.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
        att.setTextValueSuffix("ms");
        rel.setTextValueSuffix("ms");
        rel.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
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

        thresh.onDragEnd = [this]() {undo_Mgr.beginNewTransaction(); };
        ratio.onDragEnd = [this]() {undo_Mgr.beginNewTransaction(); };
        att.onDragEnd = [this]() {undo_Mgr.beginNewTransaction(); };
        rel.onDragEnd = [this]() {undo_Mgr.beginNewTransaction(); };

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
    juce::UndoManager& undo_Mgr;
    juce::ValueTree ltree;
private:
    juce::Slider thresh, ratio, att, rel;
    juce::Label thresh_label{ "Threshold" }, ratio_label{ "Ratio" }, att_label{ "Attack" }, rel_label{ "Release" };
    TreeSliderAttachment threshAttach;
    TreeSliderAttachment ratAttach;
    TreeSliderAttachment attAttach;
    TreeSliderAttachment relAttach;


    
};