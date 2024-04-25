/*
  ==============================================================================

    Control_Labels.h
    Created: 15 Apr 2024 8:02:48am
    Author:  Simeon

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class DynamicRangeLabels : public juce::Component {
public:
    DynamicRangeLabels() {
        thresh_label.setColour(juce::Label::ColourIds::textColourId, juce::Colours::black);
        thresh_label.setText("Threshold", juce::dontSendNotification);
        thresh_label.setJustificationType(juce::Justification::centred);
        ratio_label.setColour(juce::Label::ColourIds::textColourId, juce::Colours::black);
        ratio_label.setText("Ratio", juce::dontSendNotification);
        ratio_label.setJustificationType(juce::Justification::centred);
        att_label.setColour(juce::Label::ColourIds::textColourId, juce::Colours::black);
        att_label.setText("Attack", juce::dontSendNotification);
        att_label.setJustificationType(juce::Justification::centred);
        rel_label.setColour(juce::Label::ColourIds::textColourId, juce::Colours::black);
        rel_label.setText("Release", juce::dontSendNotification);
        rel_label.setJustificationType(juce::Justification::centred);

        addAndMakeVisible(&thresh_label);
        addAndMakeVisible(&ratio_label);
        addAndMakeVisible(&att_label);
        addAndMakeVisible(&rel_label);
    }

    void paint(juce::Graphics& g) override{
        g.fillAll(juce::Colours::cornflowerblue);
        g.setColour(juce::Colours::black);
        g.drawRect(getLocalBounds());
    }
    void resized() override
    {
        auto bounds = getLocalBounds();
        juce::FlexBox flex;
        flex.flexDirection = juce::FlexBox::Direction::row;
        flex.flexWrap = juce::FlexBox::Wrap::noWrap;
        auto slider_size = bounds.getWidth() * 0.25f;


        flex.items.add(juce::FlexItem(thresh_label).withFlex(1.f));
        flex.items.add(juce::FlexItem(ratio_label).withFlex(1.f));
        flex.items.add(juce::FlexItem(att_label).withFlex(1.f));
        flex.items.add(juce::FlexItem(rel_label).withFlex(1.f));
        flex.performLayout(bounds);
    }
private:
    juce::Label thresh_label, ratio_label, att_label, rel_label;
};

class LimiterLabels : public juce::Component {
public:
    LimiterLabels() {
        thresh_label.setColour(juce::Label::ColourIds::textColourId, juce::Colours::black);
        thresh_label.setText("Threshold", juce::dontSendNotification);
        thresh_label.setJustificationType(juce::Justification::centred);
        rel_label.setColour(juce::Label::ColourIds::textColourId, juce::Colours::black);
        rel_label.setText("Release", juce::dontSendNotification);
        rel_label.setJustificationType(juce::Justification::centred);

        addAndMakeVisible(&thresh_label);
        addAndMakeVisible(&rel_label);
    }

    void paint(juce::Graphics& g) override {
        g.fillAll(juce::Colours::cornflowerblue);
        g.setColour(juce::Colours::black);
        g.drawRect(getLocalBounds());
    }
    void resized() override
    {
        auto bounds = getLocalBounds();
        juce::FlexBox flex;
        flex.flexDirection = juce::FlexBox::Direction::row;
        flex.flexWrap = juce::FlexBox::Wrap::noWrap;
        auto slider_size = bounds.getWidth() * 0.25f;


        flex.items.add(juce::FlexItem(thresh_label).withFlex(1.f));
        flex.items.add(juce::FlexItem(rel_label).withFlex(1.f));
        flex.performLayout(bounds);
    }
private:
    juce::Label thresh_label, rel_label;
};

class FreqLabels : public juce::Component {
public:
    FreqLabels() {
        low_mid_label.setColour(juce::Label::ColourIds::textColourId, juce::Colours::black);
        low_mid_label.setText("1st Cutoff Freq", juce::dontSendNotification);
        low_mid_label.setJustificationType(juce::Justification::centred);
        mid_high_label.setColour(juce::Label::ColourIds::textColourId, juce::Colours::black);
        mid_high_label.setText("2nd Cutoff Freq", juce::dontSendNotification);
        mid_high_label.setJustificationType(juce::Justification::centred);

        addAndMakeVisible(&low_mid_label);
        addAndMakeVisible(&mid_high_label);
    }

    void paint(juce::Graphics& g) override {
        /*g.fillAll(juce::Colours::darkcyan);
        g.setColour(juce::Colours::white);*/
        g.fillAll(juce::Colours::cornflowerblue);
        g.setColour(juce::Colours::black);
        g.drawRect(getLocalBounds());
    }
    void resized() override
    {
        auto bounds = getLocalBounds();
        juce::FlexBox flex;
        flex.flexDirection = juce::FlexBox::Direction::row;
        flex.flexWrap = juce::FlexBox::Wrap::noWrap;
        auto slider_size = bounds.getWidth() * 0.25f;


        flex.items.add(juce::FlexItem(low_mid_label).withFlex(1.f));
        flex.items.add(juce::FlexItem(mid_high_label).withFlex(1.f));
        flex.performLayout(bounds);
    }
private:
    juce::Label low_mid_label, mid_high_label;
};