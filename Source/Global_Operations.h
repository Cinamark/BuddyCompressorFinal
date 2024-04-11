/*
  ==============================================================================

    Global_Operations.h
    Created: 31 Mar 2024 11:10:06am
    Author:  Simeon

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

//inline void OpenFile(juce::File file) {
//	std::unique_ptr<juce::FileChooser> chooser;
//	chooser = std::make_unique<juce::FileChooser>("Select a Wave file to play...",
//		juce::File{},
//		"*.wav");                     
//	auto chooserFlags = juce::FileBrowserComponent::openMode
//		| juce::FileBrowserComponent::canSelectFiles;
//
//	chooser->launchAsync(chooserFlags, [file](const juce::FileChooser& fc)     
//		{
//			*file = fc.getResult();
//		});
//}