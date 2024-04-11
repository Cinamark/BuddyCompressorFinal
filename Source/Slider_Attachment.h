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
        : tree(v), btn(slida), param(parameter)

    {
        jassert(tree.isValid());
        tree.addListener(this);
        btn->addListener(this);

        if (param == juce::Identifier{ "Gate Ratio" } || param == juce::Identifier{ "Ratio High Band" }) {
            tree.setProperty(param, 7, nullptr);
        }

        if (tree.hasProperty(param)) {
            btn->setValue(tree.getProperty(param));
        }
        else
            tree.setProperty(param, btn->getValue(), nullptr);


    }
    ~TreeSliderAttachment() {
        tree.removeListener(this);
        if (btn)
            btn->removeListener(this);
    }


    void valueTreePropertyChanged(juce::ValueTree& Changedtree, const juce::Identifier& Changedparam) override {
        if (!updating) {
            updating = true;
            if (Changedtree == tree && btn)
                if (Changedparam == param)
                    btn->setValue(tree.getProperty(param));
        }
        updating = false;
    }

    void sliderValueChanged(juce::Slider* slider) override {
        tree.setProperty(param, slider->getValue(),nullptr);
    }
    juce::ValueTree tree;
private:
    juce::Component::SafePointer<juce::Slider> btn;
    juce::Identifier param;
    bool updating = false;
};