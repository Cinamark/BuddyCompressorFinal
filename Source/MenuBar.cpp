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
    return { "File", "Edit", "Theme", "Presets" };
}

juce::PopupMenu MenuBarComp::getMenuForIndex(int topLevelMenuIndex, const juce::String& menuName) {
    juce::PopupMenu menu;

    if (menuName == "File") {
        menu.addCommandItem(&commandManager, CommandID::OPEN);
        menu.addCommandItem(&commandManager, CommandID::SAVE);
        //menu.addItem(3, "Save as");
        menu.addItem(4, "Exit");
    }
    else if (menuName == "Theme") {
        menu.addCommandItem(&commandManager, CommandID::DARK);
        menu.addCommandItem(&commandManager, CommandID::LIGHT);
    }
    else if (menuName == "Edit") {
        menu.addCommandItem(&commandManager, CommandID::UNDO);
        menu.addCommandItem(&commandManager, CommandID::REDO);
    }

    else if (menuName == "Presets") {
        menu.addCommandItem(&commandManager, CommandID::OPEN_PRESET);
        menu.addCommandItem(&commandManager, CommandID::SAVE_PRESET);
    }

    return menu;
}

void MenuBarComp::menuItemSelected(int menuItemID, int topLevelMenuIndex) {
}


void MenuBarComp::getAllCommands(juce::Array<juce::CommandID>& c) 
{
    juce::Array<CommandID> commands{ CommandID::OPEN, CommandID::SAVE, CommandID::SAVE_PRESET, CommandID::OPEN_PRESET, CommandID::UNDO, CommandID::REDO, CommandID::DARK, CommandID::LIGHT };
    c.addArray(commands);
}

void MenuBarComp::getCommandInfo(juce::CommandID commandID, juce::ApplicationCommandInfo& result)
{
    switch (commandID)
    {
    case MenuBarComp::CommandID::OPEN:
        result.setInfo("Open", "You are opening a file", "Menu", 0);
        break;

    case MenuBarComp::CommandID::SAVE:
        result.setInfo("Save", "You are saving a file", "Menu", 0);
        break;

    case MenuBarComp::CommandID::SAVE_PRESET:
        result.setInfo("Save Preset", "You are saving a preset", "Menu", 0);
        break;

    case MenuBarComp::CommandID::OPEN_PRESET:
        result.setInfo("Open Preset", "You are opening a preset", "Menu", 0);
        break;
    case MenuBarComp::CommandID::UNDO:
        result.setInfo("Undo", "Goes back 1 step", "Menu", 0);
        break;
    case MenuBarComp::CommandID::REDO:
        result.setInfo("Redo", "Goes forward 1 step", "Menu", 0);
        break;

    case MenuBarComp::CommandID::DARK:
        result.setInfo("Dark", "Sets theme to dark", "Menu", 0);
        break;

    case MenuBarComp::CommandID::LIGHT:
        result.setInfo("Light", "Sets theme to light", "Menu", 0);
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
            input_chooser = std::make_unique<juce::FileChooser>("Select a file to play...",
                juce::File{},
                "*.wav;*.aiff;*.mp3");
            auto chooserFlags = juce::FileBrowserComponent::openMode
                | juce::FileBrowserComponent::canSelectFiles;

            input_chooser->launchAsync(chooserFlags, [this](const juce::FileChooser& fc)
                {
                    auto file = fc.getResult();
                    if (file == juce::File{})
                        return;
                    input_file = file;
                    FileUpdate();
                });
            break; }
    
    case MenuBarComp::CommandID::SAVE: {
        SaveFile();
        break; }

    case MenuBarComp::CommandID::SAVE_PRESET: {
        save_chooser = std::make_unique<juce::FileChooser>("Please name your file...",
            juce::File::getSpecialLocation(juce::File::SpecialLocationType::commonDocumentsDirectory).getChildFile(ProjectInfo::companyName).getChildFile(ProjectInfo::projectName),
            "*.PRESET");
        auto chooserFlags = juce::FileBrowserComponent::saveMode
            | juce::FileBrowserComponent::canSelectFiles | juce::FileBrowserComponent::warnAboutOverwriting;

        save_chooser->launchAsync(chooserFlags, [this](const juce::FileChooser& fc)
            {
                auto file = fc.getResult();
                savedPresetFile = file.getFileNameWithoutExtension();
                savePreset();
            });
        break; }
    
    case MenuBarComp::CommandID::OPEN_PRESET: {
        preset_chooser = std::make_unique<juce::FileChooser>("Select a preset file...",
            juce::File::getSpecialLocation(juce::File::SpecialLocationType::commonDocumentsDirectory).getChildFile(ProjectInfo::companyName).getChildFile(ProjectInfo::projectName),
            "*.PRESET");
        auto chooserFlags = juce::FileBrowserComponent::openMode
            | juce::FileBrowserComponent::canSelectFiles;

        preset_chooser->launchAsync(chooserFlags, [this](const juce::FileChooser& fc)
            {
                auto file = fc.getResult();
                if (file == juce::File{})
                    return;
                presetFile = file.getFileNameWithoutExtension();
                openPreset();
            });
        break; }

    case MenuBarComp::CommandID::UNDO: {
        Undo();
        break; }
    case MenuBarComp::CommandID::REDO: {
        Redo();
        break; }

    case MenuBarComp::CommandID::DARK: {
        setDark();
        break; }
    
    case MenuBarComp::CommandID::LIGHT: {
        setLight();
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