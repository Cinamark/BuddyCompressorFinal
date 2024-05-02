/*
  ==============================================================================

    ComboBoxAttachment.h
    Created: 1 May 2024 11:36:07am
    Author:  Simeon

  ==============================================================================
*/


#pragma once
#include <JuceHeader.h>
class ComboBoxAttachment : public juce::ComboBox::Listener,
    public juce::ValueTree::Listener
{
public:
    ComboBoxAttachment(juce::ValueTree v, juce::ComboBox* comb, juce::Identifier parameter, juce::UndoManager* uMgr)
        : tree(v), box(comb), param(parameter), undoMgr(*uMgr)

    {
        jassert(tree.isValid());
        tree.addListener(this);
        box->addListener(this);


        if (tree.hasProperty(param)) {
            box->setSelectedItemIndex(tree.getProperty(param));
        }
        else
            tree.setProperty(param, box->getSelectedItemIndex(), &undoMgr);


    }
    ~ComboBoxAttachment() {
        tree.removeListener(this);
        if (box)
            box->removeListener(this);
    }


    void valueTreePropertyChanged(juce::ValueTree& Changedtree, const juce::Identifier& Changedparam) override {
        if (!updating) {
            updating = true;
            if (Changedtree == tree && box)
                if (Changedparam == param)
                    box->setSelectedId(tree.getProperty(param));
        }
        updating = false;
    }

    void comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged) override {
        if (!updating) {
            updating = true;
            if (box == comboBoxThatHasChanged)
                tree.setProperty(param, box->getSelectedId(), &undoMgr);
        }
        updating = false;
        
    }

    juce::ValueTree tree;
private:
    juce::Component::SafePointer<juce::ComboBox> box;
    juce::Identifier param;
    juce::UndoManager& undoMgr;
    bool updating = false;
};