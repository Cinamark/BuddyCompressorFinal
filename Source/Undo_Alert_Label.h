/*
  ==============================================================================

    Undo_Alert_Label.h
    Created: 24 Apr 2024 6:00:14pm
    Author:  Simeon

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Label_Attachment.h"

class UndoAlert : public juce::Label, public juce::ValueTree::Listener, public juce::ChangeListener
{
public:
    UndoAlert(juce::ValueTree v, juce::UndoManager* uMgr) :
    tree(v), undo(*uMgr){
        tree.addListener(this);
        undo.addChangeListener(this);
        this->setColour(juce::Label::ColourIds::backgroundColourId, juce::Colours::black);
    }
    ~UndoAlert() {
        tree.removeListener(this);
    }

    void changeListenerCallback(juce::ChangeBroadcaster* source) override {

    }
    void valueTreePropertyChanged(juce::ValueTree& Changedtree, const juce::Identifier& Changedparam) override {
        if (!updating && undo.isPerformingUndoRedo()) {
            updating = true;
            if (Changedtree == tree)
                this->setText("Undo: " + juce::String{ Changedparam.toString() }, juce::dontSendNotification);         
        }
        updating = false;
    }

    juce::ValueTree tree;
private:
    juce::UndoManager& undo;
    bool updating = false;

};