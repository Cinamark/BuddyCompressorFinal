/*
  ==============================================================================

    MenuBar.h
    Created: 29 Mar 2024 8:29:04pm
    Author:  Simeon

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Global_Operations.h"

class MenuBarComp : public juce::Component,
    public juce::MenuBarModel,
    public juce::ApplicationCommandTarget
{
public:

    enum CommandID {
        OPEN = 1,
        SAVE,
        SAVE_AS,
        EXIT,
        UNDO,
        REDO,
        DARK,
        LIGHT,
        SAVE_PRESET
    };
    MenuBarComp(juce::File& file);
    ~MenuBarComp() override;

    void paint(juce::Graphics& g) override;
    void resized() override;

    juce::StringArray getMenuBarNames() override;
    juce::PopupMenu getMenuForIndex(int topLevelMenuIndex, const juce::String& menuName) override;
    void menuItemSelected(int menuItemID, int topLevelMenuIndex) override;

    juce::ApplicationCommandTarget* getNextCommandTarget() override;
    void getAllCommands(juce::Array<juce::CommandID>&) override;
    void getCommandInfo(juce::CommandID, juce::ApplicationCommandInfo&) override;
    bool perform(const InvocationInfo&) override;

    juce::File input_file;
    std::function<void()> FileUpdate = []() {};
private:
    #if JUCE_DEMO_RUNNER
    juce::ApplicationCommandManager& commandManager = getGlobalCommandManager();
    #else
    juce::ApplicationCommandManager commandManager;
    #endif
    juce::Colour cola;
    std::unique_ptr<juce::MenuBarComponent> menuComponent;
    
    std::unique_ptr<juce::FileChooser> input_chooser;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MenuBarComp);
};