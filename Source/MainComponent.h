#pragma once

#include <JuceHeader.h>
#include "MenuBar.h"
#include "Placeholder_Comp.h"
#include "dsp_fucntions.h"
#include "Waveform.h"
#include "Meter.h"
#include "Gate_Sliders.h"
#include "APCompressor_Sliders.h"
#include "LB_Multiband_Controls.h"
#include "MB_Multiband_Controls.h"
#include "HP_Multiband_Controls.h"
#include "FrequencySliders.h"
#include "Analyzer.h"

//=============================================================================

enum CommandID {
    PAUSE = 1,
    SAVE,
    SAVE_AS,
    EXIT,
    UNDO,
    REDO,
    DARK,
    LIGHT,
    SAVE_PRESET
};


class SliderLooknFeel : public juce::LookAndFeel_V4 {
public:
    SliderLooknFeel() {

    }

    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
        const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider&) override {
        auto radius = (float)juce::jmin(width / 2, height / 2) - 4.0f;
        auto centreX = (float)x + (float)width * 0.5f;
        auto centreY = (float)y + (float)height * 0.5f;
        auto rx = centreX - radius;
        auto ry = centreY - radius;
        auto rw = radius * 2.0f;
        auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

        // fill
        g.setColour(juce::Colours::orange);
        g.fillEllipse(rx, ry, rw, rw);

        // outline
        g.setColour(juce::Colours::red);
        g.drawEllipse(rx, ry, rw, rw, 1.0f);

        juce::Path p;
        auto pointerLength = radius * 0.33f;
        auto pointerThickness = 2.0f;
        p.addRectangle(-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
        p.applyTransform(juce::AffineTransform::rotation(angle).translated(centreX, centreY));

        g.setColour(juce::Colours::blue);
        g.fillPath(p);
    }
};
class MainComponent  : public juce::AudioAppComponent,
    public juce::ChangeListener,
    public juce::ApplicationCommandTarget
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;
    bool keyPressed(const juce::KeyPress& key) override;

    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;
    void playButtonClicked();
    void stopButtonClicked();
    void processBlock(juce::AudioBuffer<float>& buffer);


    //====================================================

    juce::ApplicationCommandTarget* getNextCommandTarget() override {
        return nullptr;
    };
    void getAllCommands(juce::Array<juce::CommandID>& c) override{
        juce::Array<CommandID> commands{ CommandID::PAUSE };
        c.addArray(commands);
    };


    void getCommandInfo(juce::CommandID commandID, juce::ApplicationCommandInfo& result) override {
        switch (commandID)
        {
        case CommandID::PAUSE:
            result.setInfo("Pause/Pay", "The audio is paused and/or played", "Audio Playback", 0);
            result.addDefaultKeypress(' ',0);
            break;
        default:
            break;
        }
    };
    bool perform(const InvocationInfo& info) override
    {
        switch (info.commandID)
        {
        case CommandID::PAUSE: {
            if (state == Playing)
                changeState(Pausing);
            else if (state == Stopped || state == Paused)
                changeState(Starting);

            break; }
        default:
            return false;
        }

        return true;
    };


private:
    //==============================================================================
    
    juce::ColourGradient bg_gradient;

    juce::MenuBarComponent menu;
    juce::File i_file;
    MenuBarComp menu_2;
    juce::Identifier paramId{ "Parameter Tree" };
    juce::ValueTree paramTree{ paramId };
    juce::UndoManager undo;
    
    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;
    bool file_loaded = false;

    GUI::Waveform waveform;
    GUI::Wav_Bar playback_bar;
    GUI::customLnF LNF;

    GateSliders gateSliderComp;
    APCompressorSliders apCompressorSliderComp;
    HighBandCompSliders hbCompressorSliderComp;
    MidBandCompSliders  mbCompressorSliderComp;
    LowBandCompSliders  lbCompressorSliderComp;
    FrequencySliders freqComp;
    Placeholder input, preset, controls, output, buttons, slida;

    //SliderLooknFeel sliderlnf;

    juce::TextButton PLAY{ "PLAY" };
    juce::TextButton STOP{ "STOP" };

    juce::TextButton Gate{ "Gate" };
    juce::TextButton APComp{ "Comp" };
    juce::TextButton MBandComp{ "3-Comp" };
    juce::TextButton Limiter{ "Limit" };

    juce::TextButton LB{ "L" };
    juce::TextButton MB{ "M" };
    juce::TextButton HB{ "H" };


    juce::TextButton Waveform_Btn{ "Waveform" };
    juce::TextButton Frequency{ "Freq" };
    void multibuttonsInVisible() {
        LB.setVisible(false);
        MB.setVisible(false);
        HB.setVisible(false);
    }

    void multibuttonsVisible() {
        LB.setVisible(true);
        MB.setVisible(true);
        HB.setVisible(true);
    }
    
    enum TransportState
    {
        Stopped,
        Starting,
        Playing,
        Stopping,
        Saving,
        Paused,
        Pausing
    };

    enum Controls
    {
        Display_Gate,
        Display_Allpass,
        Display_MultiBand,
        Display_Limiter
    };

    enum MB_Controls
    {
        Display_LB,
        Display_MB,
        Display_HB
    };
    TransportState state;
    Controls controlState;
    MB_Controls multibandState;
    enum RadioButttonIds {

        LimiterButtons = 1001
    };
    void changeState(TransportState newState);
    void changeControls(Controls newState);
    void multiStateChange(MB_Controls newState);
    juce::LinearSmoothedValue<float> in_rmsLevel_L, in_rmsLevel_R, out_rmsLevel_L, out_rmsLevel_R;
    float get_in_RMSvalue(const int channel) const;
    float get_out_RMSvalue(const int channel) const;

    GUI::Horiz_Meter in_meterL;
    GUI::Horiz_Meter in_meterR;
    GUI::Analyser inAnalyse, outAnalyse;
    juce::ApplicationCommandManager c_manager;
    DSPFunctions dspfunc;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
