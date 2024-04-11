/*
  ==============================================================================

    MenuBar.cpp
    Created: 29 Mar 2024 8:29:04pm
    Author:  Simeon

  ==============================================================================
*/

#include "MenuBar.h"

MenuBarComp::MenuBarComp(juce::File& file):
input_file(file){
    menuComponent.reset(new juce::MenuBarComponent(this));
    addAndMakeVisible(menuComponent.get());
    setApplicationCommandManagerToWatch(&commandManager);
    commandManager.registerAllCommandsForTarget(this);
    commandManager.setFirstCommandTarget(this);
    resized();
}

MenuBarComp::~MenuBarComp() {}

void MenuBarComp::paint(juce::Graphics& g) {
    g.fillAll(cola);

    g.setFont(juce::Font(20.0f));
    g.setColour(cola);
}

void MenuBarComp::resized() {
    //setting the size of our normal component.
    setSize(getParentWidth(), 30);
    // setting the size of the MenuBarComponent inside this component.
    menuComponent.get()->setSize(getParentWidth(), 30);
}

juce::StringArray MenuBarComp::getMenuBarNames() {
    return { "File", "Edit", "Theme", "Save Preset" };
}

juce::PopupMenu MenuBarComp::getMenuForIndex(int topLevelMenuIndex, const juce::String& menuName) {
    juce::PopupMenu menu;

    if (menuName == "File") {
        menu.addCommandItem(&commandManager, CommandID::OPEN);
        menu.addItem(2, "Save");
        menu.addItem(3, "Save as");
        menu.addItem(4, "Exit");
    }
    else if (menuName == "Theme") {
        menu.addItem(15, "Dark");
        menu.addItem(16, "Light");
    }
    else if (menuName == "Edit") {
        menu.addItem(21, "Undo");
        menu.addItem(21, "Redo");
    }

    return menu;
}

void MenuBarComp::menuItemSelected(int menuItemID, int topLevelMenuIndex) {
}


void MenuBarComp::getAllCommands(juce::Array<juce::CommandID>& c) 
{
    juce::Array<CommandID> commands{ CommandID::OPEN };
    c.addArray(commands);
}

void MenuBarComp::getCommandInfo(juce::CommandID commandID, juce::ApplicationCommandInfo& result)
{
    switch (commandID)
    {
    case MenuBarComp::CommandID::OPEN:
        result.setInfo("Open", "This a test", "Menu", 0);
        break;
    default:
        break;
    }
}

bool MenuBarComp::perform(const juce::ApplicationCommandTarget::InvocationInfo& info) 
{
    switch (info.commandID)
    {
    case MenuBarComp::CommandID::OPEN:{
            input_chooser = std::make_unique<juce::FileChooser>("Select a Wave file to play...",
                juce::File{},
                "*.wav");
            auto chooserFlags = juce::FileBrowserComponent::openMode
                | juce::FileBrowserComponent::canSelectFiles;

            input_chooser->launchAsync(chooserFlags, [this](const juce::FileChooser& fc)
                {
                    auto file = fc.getResult();
                    input_file = file;
                    FileUpdate();
                });
            break; }
    default:
        return false;
    }

    return true;
}

juce::ApplicationCommandTarget* MenuBarComp::getNextCommandTarget()
{
    return findFirstTargetParentComponent();
}