/*
  ==============================================================================

    Label_Attachment.h
    Created: 24 Apr 2024 6:02:56pm
    Author:  Simeon

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
class LabelAttachment :
    public juce::ValueTree::Listener
{
public:
    LabelAttachment(juce::ValueTree v, juce::Label* lbl, juce::Identifier parameter, juce::UndoManager* uMgr)
        : tree(v), undo_label(lbl), param(parameter), undo(*uMgr)

    {
        jassert(tree.isValid());
        tree.addListener(this);


    }
    ~LabelAttachment() {
        tree.removeListener(this);
    }


    void valueTreePropertyChanged(juce::ValueTree& Changedtree, const juce::Identifier& Changedparam) override {
        if (!updating) {
            updating = true;
            if (Changedtree == tree)
                if (Changedparam == param)
                    undo_label->setText("Undo: " + juce::String{Changedparam.toString()}, juce::dontSendNotification);
        }
        updating = false;
    }

    juce::ValueTree tree;
private:
    juce::Component::SafePointer<juce::Label> undo_label;
    juce::Identifier param;
    juce::UndoManager& undo;
    bool updating = false;
};