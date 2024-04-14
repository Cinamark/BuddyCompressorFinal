/*
  ==============================================================================

    Button_Attachment.h
    Created: 13 Apr 2024 9:32:00pm
    Author:  Simeon

  ==============================================================================
*/

#pragma once
#pragma once
#include <JuceHeader.h>
class ToggleButtonAttachment : public juce::Button::Listener,
    public juce::ValueTree::Listener
{
public:
    ToggleButtonAttachment(juce::ValueTree v, juce::Button* button, juce::Identifier parameter)
        : tree(v), btn(button), param(parameter)

    {
        jassert(tree.isValid());
        tree.addListener(this);
        btn->addListener(this);


        if (tree.hasProperty(param)) {
            btn->setToggleState(tree.getProperty(param), juce::dontSendNotification);
        }
        else
            tree.setProperty(param, btn->getToggleState(),nullptr);


    }
    ~ToggleButtonAttachment() {
        tree.removeListener(this);
        if (btn)
            btn->removeListener(this);
    }

    void buttonClicked(juce::Button* button) override {
        btn->setClickingTogglesState(true);
    }
    void valueTreePropertyChanged(juce::ValueTree& Changedtree, const juce::Identifier& Changedparam) override {
        if (!updating) {
            updating = true;
            if (Changedtree == tree && btn)
                if (Changedparam == param)
                    btn->setToggleState(tree.getProperty(param), juce::dontSendNotification);
        }
        updating = false;
    }

    void buttonStateChanged(juce::Button* button) override {
        tree.setProperty(param, button->getToggleState(),nullptr);
    }
    juce::ValueTree tree;
private:
    juce::Button::SafePointer<juce::Button> btn;
    juce::Identifier param;
    bool updating = false;
};
