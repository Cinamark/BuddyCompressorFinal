#pragma once

#include <JuceHeader.h>
#include "MenuBar.h"
#include "ParamsMap.h"
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
#include "Limiter_Sliders.h"
#include "Analyzer.h"
#include "Button_Attachment.h"
#include "PresetManager.h"
#include "Control_Labels.h"
#include "Label_LnF.h"
#include "Undo_Alert_Label.h"
#include "ComboBoxAttachment.h"


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
        g.setColour(juce::Colour::fromRGB(200, 200, 207));
        g.fillEllipse(rx, ry, rw, rw);

        // outline
        g.setColour(juce::Colours::black);
        g.drawEllipse(rx, ry, rw, rw, 1.0f);

        juce::Path p;
        auto pointerLength = radius * 0.33f;
        auto pointerThickness = 2.0f;
        p.addRectangle(-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
        p.applyTransform(juce::AffineTransform::rotation(angle).translated(centreX, centreY));

        g.setColour(juce::Colours::blue);
        g.fillPath(p);
    }

    juce::Label* createSliderTextBox(juce::Slider& slider) override
    {
        juce::Label* l = LookAndFeel_V4::createSliderTextBox(slider);

        // make sure editor text is black (so it shows on white background)
        l->setColour(juce::Label::textColourId, juce::Colours::white);
        l->setColour(juce::Label::backgroundColourId, juce::Colours::darkgrey);
        return l;
    }


};

class SliderLooknFeel_two : public juce::LookAndFeel_V4 {
public:
    SliderLooknFeel_two() {

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
        g.setColour(juce::Colour::fromRGB(42, 45, 51));
        g.fillEllipse(rx, ry, rw, rw);

        // outline
        g.setColour(juce::Colours::white);
        g.drawEllipse(rx, ry, rw, rw, 1.0f);

        juce::Path p;
        auto pointerLength = radius * 0.33f;
        auto pointerThickness = 2.0f;
        p.addRectangle(-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
        p.applyTransform(juce::AffineTransform::rotation(angle).translated(centreX, centreY));

        g.setColour(juce::Colours::blue);
        g.fillPath(p);
    }

    juce::Label* createSliderTextBox(juce::Slider& slider) override
    {
        juce::Label* l = LookAndFeel_V4::createSliderTextBox(slider);

        // make sure editor text is black (so it shows on white background)
        l->setColour(juce::Label::textColourId, juce::Colours::white);
        l->setColour(juce::Label::backgroundColourId, juce::Colours::darkgrey);
        return l;
    }


};

class ToggleButtonLooknFeel : public juce::LookAndFeel_V4 {
public:
    ToggleButtonLooknFeel() {
        setColour(juce::ToggleButton::ColourIds::textColourId, juce::Colours::white);
    }

    void drawTickBox(juce::Graphics& g, juce::Component& btn, float x, float y, float w, float h, bool ticked, bool isEnabled, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override {
        auto radius = (float)juce::jmin(w / 2, h / 2) - 4.0f;
        auto centreX = (float)x + (float)w * 0.5f;
        auto centreY = (float)y + (float)h * 0.5f;
        auto rx = centreX - radius;
        auto ry = centreY - radius;
        auto rw = radius * 2.0f;

        


        g.setColour(juce::Colours::red);
        g.drawEllipse(rx, ry, rw, rw, 1.0f);

        g.setColour(juce::Colours::bisque);
        g.fillEllipse(rx, ry, rw, rw);

        if (shouldDrawButtonAsHighlighted && !ticked) {

                auto shrink = 5;
                g.setColour(juce::Colours::red);
                g.fillEllipse(rx + shrink * 0.5f, ry + shrink * 0.5f, rw - shrink, rw - shrink);


        }
        if (ticked) {

            auto shrink = 5;
            g.setColour(juce::Colours::blue);
            g.fillEllipse(rx + shrink*0.5f, ry + shrink * 0.5f, rw - shrink, rw - shrink);


        }
    }

    juce::Label* createSliderTextBox(juce::Slider& slider) override
    {
        juce::Label* l = LookAndFeel_V4::createSliderTextBox(slider);

        // make sure editor text is black (so it shows on white background)
        l->setColour(juce::Label::textColourId, juce::Colours::white);
        l->setColour(juce::Label::backgroundColourId, juce::Colours::darkgrey);
        return l;
    }


};

class PresetButtonsBg : public juce::Component {
public:

    PresetButtonsBg() {

    }
    void paint(juce::Graphics& g) override {
        //g.setColour(juce::Colours::darkgrey);
        g.setColour(juce::Colour::fromRGB(48, 51, 60));
        g.fillAll();
        g.setColour(juce::Colours::whitesmoke);
        g.drawRect(getLocalBounds());
    }

};
class MainComponent  : public juce::AudioAppComponent,
    public juce::ChangeListener,
    public juce::ApplicationCommandTarget,
    public juce::Thread::Listener,
    public juce::FileDragAndDropTarget
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
    void loadYoutube();
    void loadPodcast();
    void setCustom();
    void exitSignalSent() {
        const juce::MessageManagerLock mmLock;
        paramTree.setProperty(juce::Identifier{ Params::GetParams().at(Params::Names::Gain_In) }, (std::abs(in_waveform.getPeak())), nullptr);
        input_peak = (std::abs(in_waveform.getPeak()));
        DBG("level" << (float)paramTree.getProperty(juce::Identifier{ Params::GetParams().at(Params::Names::Gain_In) }));
    }

    //void mouseUp(const juce::MouseEvent& event) override {
    //    undo.beginNewTransaction();
    //}
    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;
    void playButtonClicked();
    void stopButtonClicked();
    void processBlock(juce::AudioBuffer<float>& buffer);
    void saveButtonClicked();
    void updateToggleState(juce::Button* button);
    void bypassToggle(juce::Button* button);
    void openOperation(juce::File& file);
    bool isInterestedInFileDrag(const juce::StringArray& files) override {
        if (files.size() > 1) {
            return false;
        }
        else
            if (files[0].contains(".wav") || files[0].contains(".mp3") || files[0].contains(".aiff")) {
                return true;
            }
            
        return false;
        
    }

    void filesDropped(const juce::StringArray& files, int x, int y) override {
        if (files.size() > 1) {
            return;
        }
        openOperation(juce::File{ files[0] });
    }

    //====================================================

    juce::ApplicationCommandTarget* getNextCommandTarget() override {
        return nullptr;
    };
    void getAllCommands(juce::Array<juce::CommandID>& c) override{
        juce::Array<CommandID> commands{ CommandID::PAUSE, CommandID::SAVE, CommandID::UNDO, CommandID::REDO };
        c.addArray(commands);
    };


    void getCommandInfo(juce::CommandID commandID, juce::ApplicationCommandInfo& result) override {
        switch (commandID)
        {
        case CommandID::PAUSE:
            result.setInfo("Pause/Pay", "The audio is paused and/or played", "Audio Playback", 0);
            result.addDefaultKeypress(' ',0);
            break;

        case CommandID::SAVE:
            result.setInfo("Pause/Pay", "The audio is paused and/or played", "Audio Playback", 0);
            result.addDefaultKeypress('s', juce::ModifierKeys::commandModifier);
            break;

        case CommandID::UNDO:
            result.setInfo("Pause/Pay", "The audio is paused and/or played", "Audio Playback", 0);
            result.addDefaultKeypress('z', juce::ModifierKeys::commandModifier);
            break;

        case CommandID::REDO:
            result.setInfo("Pause/Pay", "The audio is paused and/or played", "Audio Playback", 0);
            result.addDefaultKeypress('y', juce::ModifierKeys::commandModifier);
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
        case CommandID::SAVE: {
            saveButtonClicked();
            break; }

        case CommandID::UNDO: {
            undo.undo();

            addAndMakeVisible(&undo_label);
            redo_label.setVisible(false);
            undo_label.setVisible(true);
            break; }

        case CommandID::REDO: {
            undo.redo();

            addAndMakeVisible(&redo_label);
            undo_label.setVisible(false);
            redo_label.setVisible(true);

            break; }
        default:
            return false;
        }

        return true;
    };


private:
    //==============================================================================
    
    juce::ColourGradient bg_gradient;
    juce::Colour selected_button;
    SliderLooknFeel_two sliderlnf_two;

    PresetButtonsBg preset_bg;
    juce::MenuBarComponent menu;
    juce::File i_file;
    MenuBarComp menu_2;
    juce::Identifier paramId{ "Parameter_Tree" };
    juce::ValueTree paramTree{ paramId };
    juce::UndoManager undo;
    
    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    std::unique_ptr<juce::AlertWindow> lengthAlert;
    juce::AudioTransportSource transportSource;
    bool file_loaded = false;

    float input_peak = 0.0;
    GUI::Waveform in_waveform, out_waveform;
    GUI::Wav_Bar playback_bar;
    GUI::customLnF LNF;

    juce::Slider input_gain, output_gain, silence_samples;
    TreeSliderAttachment inputGainAttach, outputGainAttach;
    juce::Label input_gain_label, output_gain_label;
    GateSliders gateSliderComp;
    APCompressorSliders apCompressorSliderComp;
    HighBandCompSliders hbCompressorSliderComp;
    MidBandCompSliders  mbCompressorSliderComp;
    LowBandCompSliders  lbCompressorSliderComp;
    FrequencySliders freqComp;
    LimiterSliders limSliderComp;
    DynamicRangeLabels base_labels;
    LimiterLabels lim_labels;
    FreqLabels freq_labels;
    Placeholder input, preset, controls, output, buttons, slida;

    SliderLooknFeel sliderlnf;
    DarkLabelLooknFeel labellnf;

    void setSliderlnf(juce::LookAndFeel_V4& lnf) {
        gateSliderComp.setLookAndFeel(&lnf);
        apCompressorSliderComp.setLookAndFeel(&lnf);
        lbCompressorSliderComp.setLookAndFeel(&lnf);
        mbCompressorSliderComp.setLookAndFeel(&lnf);
        hbCompressorSliderComp.setLookAndFeel(&lnf);
        input_gain.setLookAndFeel(&lnf);
        output_gain.setLookAndFeel(&lnf);
        freqComp.setLookAndFeel(&lnf);
        limSliderComp.setLookAndFeel(&lnf);

    }

    enum Theme {
        DARK,
        LIGHT
    };

    Theme current_theme = LIGHT;

    juce::ComboBox ss_samples;
    ComboBoxAttachment ss_samplesAttach;
    juce::TextButton PLAY{ "PLAY" };
    juce::TextButton STOP{ "STOP" };
    juce::TextButton SAVE{ "SAVE" };
    juce::ToggleButton SS{ "Silence Slicer" };

    ToggleButtonLooknFeel presetlnf;
    juce::ToggleButton yt_button{ "Youtube" }, pod_button{ "Podcast" }, custom{ "Custom" };

    juce::TextButton ytpreset_save{ "yt preset Save" }, podpreset_save{ "podSave" };

    juce::TextButton gate_x{ "X" }, allband_x{ "X" }, lowband_x{ "X" }, midband_x{ "X" }, highband_x{ "X" }, limiter_x{ "X" };

    ToggleButtonAttachment gate_xAttach, allband_xAttach, lowband_xAttach, midband_xAttach, highband_xAttach, limiter_xAttach, SS_Attach;
    juce::TextButton Gate{ "Gate" };
    juce::TextButton APComp{ "Comp" };
    juce::TextButton MBandComp{ "3-Comp" };
    juce::TextButton Limiter{ "Limit" };

    juce::TextButton LB{ "L" };
    juce::TextButton MB{ "M" };
    juce::TextButton HB{ "H" };


    juce::TextButton header_Waveform_Btn{ "Waveform" };
    juce::TextButton header_Frequency_Btn{ "Freq" };

    juce::TextButton footer_Waveform_Btn{ "Waveform" };
    juce::TextButton footer_Frequency_Btn{ "Freq" };
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
    
    void multiBypassVisible() {
        
        lowband_x.setVisible(true);
        midband_x.setVisible(true);
        highband_x.setVisible(true);
    }

    void multiBypassInVisible() {

        lowband_x.setVisible(false);
        midband_x.setVisible(false);
        highband_x.setVisible(false);
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

    void controlButtonTheme(Controls current_state) {
        switch (current_state) {
        case Display_Gate:
            Gate.setColour(juce::TextButton::buttonColourId, selected_button);
            APComp.setColour(juce::TextButton::buttonColourId, juce::Colours::black);
            MBandComp.setColour(juce::TextButton::buttonColourId, juce::Colours::black);
            Limiter.setColour(juce::TextButton::buttonColourId, juce::Colours::black);
            break;

        case Display_Allpass:
            Gate.setColour(juce::TextButton::buttonColourId, juce::Colours::black);
            APComp.setColour(juce::TextButton::buttonColourId, selected_button);
            MBandComp.setColour(juce::TextButton::buttonColourId, juce::Colours::black);
            Limiter.setColour(juce::TextButton::buttonColourId, juce::Colours::black);
            break;

        case Display_MultiBand:
            Gate.setColour(juce::TextButton::buttonColourId, juce::Colours::black);
            APComp.setColour(juce::TextButton::buttonColourId, juce::Colours::black);
            MBandComp.setColour(juce::TextButton::buttonColourId, selected_button);
            Limiter.setColour(juce::TextButton::buttonColourId, juce::Colours::black);
            break;

        case Display_Limiter:
            Gate.setColour(juce::TextButton::buttonColourId, juce::Colours::black);
            APComp.setColour(juce::TextButton::buttonColourId, juce::Colours::black);
            MBandComp.setColour(juce::TextButton::buttonColourId, juce::Colours::black);
            Limiter.setColour(juce::TextButton::buttonColourId, selected_button);
            break;

        default:
            break;
        }
    }

    MB_Controls multibandState;
    enum RadioButtonIds {

        PresetButtons = 1001
    };
    void changeState(TransportState newState);
    void changeControls(Controls newState);
    void multiStateChange(MB_Controls newState);
    juce::LinearSmoothedValue<float> in_rmsLevel_L, in_rmsLevel_R, out_rmsLevel_L, out_rmsLevel_R;
    float get_in_RMSvalue(const int channel) const;
    float get_out_RMSvalue(const int channel) const;

    GUI::Horiz_Meter in_meterL;
    GUI::Horiz_Meter in_meterR;
    GUI::Horiz_Meter out_meterL, out_meterR;
    GUI::Analyser inAnalyse, outAnalyse;
    juce::ApplicationCommandManager c_manager;
    DSPFunctions dspfunc;


    class SaveThread : public juce::ThreadWithProgressWindow {
    public:
        SaveThread(juce::ValueTree tree) : juce::ThreadWithProgressWindow("Saving...", true, true), ltree(tree) {
            dspfunc.setTree(ltree);
            formatManager.registerBasicFormats();
        }

        ~SaveThread() {
            stopThread(1000);
        }

        void prepareDSP(int sampleRate, int samplesPerBlockExpected, int numChannels) {
            juce::dsp::ProcessSpec spec;
            spec.maximumBlockSize = samplesPerBlockExpected;
            spec.numChannels = numChannels;
            spec.sampleRate = sampleRate;


            dspfunc.prepare(spec, samplesPerBlockExpected);
        }

        void setFile(juce::File opened_File, juce::File save_File) {
            input_file = opened_File;
            o_file = save_File;
        }
        void run() override {

            o_file.deleteFile();
            if (auto fileStream = std::unique_ptr<juce::FileInputStream>(input_file.createInputStream())) {

                juce::AudioBuffer<float> procBuf;


                auto* reader = formatManager.createReaderFor(input_file);



                juce::int64 counter = 0;
                int len = (int)reader->lengthInSamples;
                procBuf.setSize((int)reader->numChannels, ltree[juce::Identifier{ Params::GetParams().at(Params::Names::Save_Samples) }]);

                prepareDSP(reader->sampleRate, procBuf.getNumSamples(), procBuf.getNumChannels());

                juce::WavAudioFormat format;
                std::unique_ptr<juce::AudioFormatWriter> writer;
                writer.reset(format.createWriterFor(new juce::FileOutputStream(o_file), reader->sampleRate, procBuf.getNumChannels(), 16, {}, 0));

                if (reader != nullptr && writer != nullptr) {
                    while (counter < len) {
                        if (threadShouldExit())
                            break;
                        reader->read(&procBuf, 0, procBuf.getNumSamples(), counter, true, true);
                        savedprocessBlock(procBuf);
                        if (ltree[juce::Identifier{ Params::GetParams().at(Params::Names::SS_Toggle) }]) {
                            if (dspfunc.SilenceSlicer(procBuf) > -45) {
                                writer->writeFromAudioSampleBuffer(procBuf, 0, procBuf.getNumSamples());
                            }
                        }
                        else
                        {
                            writer->writeFromAudioSampleBuffer(procBuf, 0, procBuf.getNumSamples());
                        }
                        
                        setProgress(counter / (double)reader->lengthInSamples);
                        counter += procBuf.getNumSamples();
                        procBuf.clear();
                    }
                    writer.reset(writer.release());
                }

            }
            signalThreadShouldExit();
        }

        void savedprocessBlock(juce::AudioBuffer<float>& buffer) {

            juce::ScopedNoDenormals noDenormals;
            dspfunc.applyInputGain(buffer);
            dspfunc.NoiseGateProcess(buffer);
            dspfunc.APCompressorProcess(buffer);
            dspfunc.MultibandComp(buffer);
            dspfunc.applyLimiter(buffer);
            dspfunc.applyOutputGain(buffer);
        }
    private:
        juce::AudioBuffer<float> o_invAPBuff;
        std::array<juce::AudioBuffer<float>, 3> o_filterBuffers;
        juce::dsp::LinkwitzRileyFilter<float> LP1, AP2, HP1, LP2, HP2;
        juce::dsp::LinkwitzRileyFilter<float> invAP1, invAP2;
        juce::ValueTree ltree;
        juce::File input_file;
        juce::File o_file;
        DSPFunctions dspfunc;
        juce::AudioFormatManager formatManager;
    };

    
    std::unique_ptr<juce::FileChooser> save_chooser;
    SaveThread save_thread;
    PresetManager pManage;
    UndoAlert undo_label;
    RedoAlert redo_label;
    bool isDragging = false;
    void makeEverythingVisible() {

        input_gain_label.setVisible(true);
        output_gain_label.setVisible(true);

        input_gain.setVisible(true);
        output_gain.setVisible(true);

        gateSliderComp.setVisible(true);
        apCompressorSliderComp.setVisible(false);
        lbCompressorSliderComp.setVisible(false);
        mbCompressorSliderComp.setVisible(false);
        hbCompressorSliderComp.setVisible(false);
        limSliderComp.setVisible(false);
        freqComp.setVisible(false);
        lim_labels.setVisible(false);
        multibuttonsInVisible();

        gate_x.setVisible(true);
        allband_x.setVisible(false);
        lowband_x.setVisible(false);
        midband_x.setVisible(false);
        highband_x.setVisible(false);
        limiter_x.setVisible(false);

        base_labels.setVisible(true);
        //lim_labels.setVisible(true);

        Gate.setVisible(true);
        APComp.setVisible(true);
        MBandComp.setVisible(true);
        Limiter.setVisible(true);
        input_gain_label.setVisible(true);
        output_gain_label.setVisible(true);
    }

    void makeEverythingInVisible() {

        input_gain_label.setVisible(false);
        output_gain_label.setVisible(false);

        input_gain.setVisible(false);
        output_gain.setVisible(false);

        gateSliderComp.setVisible(false);
        apCompressorSliderComp.setVisible(false);
        lbCompressorSliderComp.setVisible(false);
        mbCompressorSliderComp.setVisible(false);
        hbCompressorSliderComp.setVisible(false);
        limSliderComp.setVisible(false);
        freqComp.setVisible(false);
        lim_labels.setVisible(false);
        multibuttonsInVisible();

        gate_x.setVisible(false);
        allband_x.setVisible(false);
        lowband_x.setVisible(false);
        midband_x.setVisible(false);
        highband_x.setVisible(false);
        limiter_x.setVisible(false);

        base_labels.setVisible(false);
        lim_labels.setVisible(false);

        Gate.setVisible(false);
        APComp.setVisible(false);
        MBandComp.setVisible(false);
        Limiter.setVisible(false);
        input_gain_label.setVisible(false);
        output_gain_label.setVisible(false);

        freq_labels.setVisible(false);
    }
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
