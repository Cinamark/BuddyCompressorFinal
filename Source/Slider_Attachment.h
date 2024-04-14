/*
  ==============================================================================

    Slider_Attachment.h
    Created: 11 Apr 2024 4:31:05am
    Author:  Simeon

  ==============================================================================
*/



#pragma once
#include <JuceHeader.h>
class TreeSliderAttachment : public juce::Slider::Listener,
    public juce::ValueTree::Listener
{
public:
    TreeSliderAttachment(juce::ValueTree v, juce::Slider* slida, juce::Identifier parameter)
        : tree(v), slide(slida), param(parameter)

    {
        jassert(tree.isValid());
        tree.addListener(this);
        slide->addListener(this);

        if (param == juce::Identifier{ "Gate Ratio" } || param == juce::Identifier{ "Ratio High Band" }) {
            tree.setProperty(param, 7, nullptr);
        }

        if (tree.hasProperty(param)) {
            slide->setValue(tree.getProperty(param));
        }
        else
            tree.setProperty(param, slide->getValue(), nullptr);


    }
    ~TreeSliderAttachment() {
        tree.removeListener(this);
        if (slide)
            slide->removeListener(this);
    }


    void valueTreePropertyChanged(juce::ValueTree& Changedtree, const juce::Identifier& Changedparam) override {
        if (!updating) {
            updating = true;
            if (Changedtree == tree && slide)
                if (Changedparam == param)
                    slide->setValue(tree.getProperty(param));
        }
        updating = false;
    }

    void sliderValueChanged(juce::Slider* slider) override {
        tree.setProperty(param, slider->getValue(),nullptr);
    }
    juce::ValueTree tree;
private:
    juce::Component::SafePointer<juce::Slider> slide;
    juce::Identifier param;
    bool updating = false;
};