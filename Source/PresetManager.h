/*
  ==============================================================================

    PresetManager.h
    Created: 14 Apr 2024 6:35:57pm
    Author:  Simeon

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
class PresetManager {
public:

    const juce::File defaultDirectory{juce::File::getSpecialLocation(juce::File::SpecialLocationType::commonDocumentsDirectory).getChildFile(ProjectInfo::companyName).getChildFile(ProjectInfo::projectName)};
    const juce::String extension{"preset"};
    PresetManager(const juce::ValueTree& tree) :ltree(tree) {
        if (!defaultDirectory.exists())
        {
            const auto result = defaultDirectory.createDirectory();
            if (result.failed())
                jassertfalse;
        }
    }

    void savePreset(const juce::String& presetName) {
        if (presetName.isEmpty())
            return;

        const auto xml = ltree.createXml();
        const auto presetFile = defaultDirectory.getChildFile(presetName + "." + extension);
        if (!xml->writeTo(presetFile))
            jassertfalse;
    }

    void loadPreset(const juce::String& presetName) {
        if (presetName.isEmpty())
            return;

        const auto presetFile = defaultDirectory.getChildFile(presetName + "." + extension);
        if (!presetFile.existsAsFile()) {
            jassertfalse;
            return;
        }

        juce::XmlDocument xmldoc{ presetFile };
        const auto ValueTreeToLoad = juce::ValueTree::fromXml(*xmldoc.getDocumentElement());

        ltree.copyPropertiesFrom(ValueTreeToLoad, nullptr);

    }
private:
    juce::ValueTree ltree;
};