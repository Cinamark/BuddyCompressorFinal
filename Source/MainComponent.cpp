#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
    : menu_2(i_file), playback_bar(transportSource), state(Stopped), controlState(Display_Gate), multibandState(Display_LB),
    in_meterL([&]() {return get_in_RMSvalue(0); }), in_meterR([&]() {return get_in_RMSvalue(1); }),
    out_meterL([&]() {return get_out_RMSvalue(0); }), out_meterR([&]() {return get_out_RMSvalue(1); }),
    gateSliderComp{ paramTree, &undo },
    apCompressorSliderComp{ paramTree, &undo },
    lbCompressorSliderComp{ paramTree, &undo },
    mbCompressorSliderComp{ paramTree, &undo },
    hbCompressorSliderComp{ paramTree, &undo },
    freqComp{ paramTree, &undo },
    limSliderComp{ paramTree, &undo },
    inputGainAttach(paramTree, &input_gain, juce::Identifier{ juce::Identifier{ Params::GetParams().at(Params::Names::Gain_In)} }, &undo),
    outputGainAttach(paramTree, &output_gain, juce::Identifier{ juce::Identifier{ Params::GetParams().at(Params::Names::Gain_Out)} }, &undo),
    //silenceSamplesAttach(paramTree, &silence_samples, juce::Identifier{ juce::Identifier{ Params::GetParams().at(Params::Names::Gain_Out)} }, & undo),

    SS_Attach(paramTree, &SS, juce::Identifier{ juce::Identifier{ Params::GetParams().at(Params::Names::SS_Toggle)} }, & undo),
    gate_xAttach(paramTree, &gate_x, juce::Identifier{ juce::Identifier{ Params::GetParams().at(Params::Names::Bypass_Gate)} }, & undo),
    allband_xAttach(paramTree, &allband_x, juce::Identifier{ juce::Identifier{ Params::GetParams().at(Params::Names::Bypass_Allpass)} }, & undo),
    lowband_xAttach(paramTree, &lowband_x, juce::Identifier{ juce::Identifier{ Params::GetParams().at(Params::Names::Bypass_Low)} }, & undo),
    midband_xAttach(paramTree, &midband_x, juce::Identifier{ juce::Identifier{ Params::GetParams().at(Params::Names::Bypass_Mid)} }, & undo),
    highband_xAttach(paramTree, &highband_x, juce::Identifier{ juce::Identifier{ Params::GetParams().at(Params::Names::Bypass_High)} }, & undo),
    limiter_xAttach(paramTree, &limiter_x, juce::Identifier{ juce::Identifier{ Params::GetParams().at(Params::Names::Bypass_Limiter)} }, & undo),

    ss_samplesAttach(paramTree, &ss_samples, juce::Identifier{ juce::Identifier{ Params::GetParams().at(Params::Names::Save_Samples)}}, & undo),
    save_thread{ paramTree },
    pManage{ paramTree },
    undo_label(paramTree, &undo),
    redo_label(paramTree, &undo)
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (1280, 720);

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }
    
    
    menu_2.Undo = [this]() {
        undo.undo();
        
        addAndMakeVisible(&undo_label);
        redo_label.setVisible(false);
        undo_label.setVisible(true);
        };

    menu_2.Redo = [this]() {
        undo.redo();

        addAndMakeVisible(&redo_label);
        undo_label.setVisible(false);
        redo_label.setVisible(true);
        };

    menu_2.setDark = [this]() {
        in_waveform.setAsDark();
        out_waveform.setAsDark();
        inAnalyse.setAsDark();
        outAnalyse.setAsDark();
        in_meterL.setAsDark();
        out_meterL.setAsDark();
        in_meterR.setAsDark();
        out_meterR.setAsDark();
        selected_button = juce::Colour::fromRGB(31, 88, 221);
        controlButtonTheme(controlState);
        setSliderlnf(sliderlnf_two);
        current_theme = DARK;
        repaint();
        };

    menu_2.setLight = [this]() {
        in_waveform.setAsLight();
        out_waveform.setAsLight();
        inAnalyse.setAsLight();
        outAnalyse.setAsLight();
        in_meterL.setAsLight();
        out_meterL.setAsLight();
        in_meterR.setAsLight();
        out_meterR.setAsLight();
        selected_button = juce::Colours::darkcyan;
        controlButtonTheme(controlState);
        setSliderlnf(sliderlnf);
        current_theme = LIGHT;
        repaint();
        };

    menu_2.openPreset = [this]() {
        pManage.loadPreset(menu_2.presetFile);
        };

    menu_2.savePreset = [this]() {
        pManage.savePreset(menu_2.savedPresetFile);
    };
    //startTimerHz(24);
    dspfunc.setTree(paramTree);
    formatManager.registerBasicFormats();
    transportSource.addChangeListener(this);
    in_waveform.addListener(this);
    c_manager.registerAllCommandsForTarget(this);
    c_manager.setFirstCommandTarget(this);
    addKeyListener(c_manager.getKeyMappings());
    setWantsKeyboardFocus(true);


    addAndMakeVisible(output);
    addAndMakeVisible(buttons);
    addAndMakeVisible(input);
    addAndMakeVisible(preset);
    addAndMakeVisible(controls);
    addAndMakeVisible(slida);
    

    //Header Region
    addAndMakeVisible(&menu_2);
    addAndMakeVisible(&in_waveform);
    addAndMakeVisible(&playback_bar);
    addAndMakeVisible(&in_meterL);
    addAndMakeVisible(&in_meterR);
    addAndMakeVisible(&inAnalyse);
    auto gain_range = juce::NormalisableRange<double>(-60, 6, .5);
    input_gain.setNormalisableRange(gain_range);
    input_gain.setColour(juce::Slider::ColourIds::trackColourId, juce::Colours::white);
    output_gain.setColour(juce::Slider::ColourIds::trackColourId, juce::Colours::white);
    addAndMakeVisible(&input_gain);
    inAnalyse.setVisible(false);


    addAndMakeVisible(&header_Waveform_Btn);
    header_Waveform_Btn.onClick = [this]() {in_waveform.setVisible(true); out_waveform.setVisible(true); inAnalyse.setVisible(false); outAnalyse.setVisible(false); };
    addAndMakeVisible(&header_Frequency_Btn);
    header_Frequency_Btn.onClick = [this]() {in_waveform.setVisible(false); out_waveform.setVisible(false); inAnalyse.setVisible(true); outAnalyse.setVisible(true); };

    //Controls
    setSliderlnf(sliderlnf);
    input_gain_label.setLookAndFeel(&labellnf);
    output_gain_label.setLookAndFeel(&labellnf);
    
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

    input_gain_label.setText("Input Gain", juce::dontSendNotification);
    output_gain_label.setText("Output Gain", juce::dontSendNotification);

    
    addAndMakeVisible(&gateSliderComp);
    addAndMakeVisible(&apCompressorSliderComp);
    addAndMakeVisible(&lbCompressorSliderComp);
    addAndMakeVisible(&mbCompressorSliderComp);
    addAndMakeVisible(&hbCompressorSliderComp);
    addAndMakeVisible(&freqComp);
    addAndMakeVisible(&limSliderComp);
    addAndMakeVisible(&base_labels);
    addAndMakeVisible(&lim_labels);
    addAndMakeVisible(&freq_labels);
    addAndMakeVisible(&input_gain_label);
    addAndMakeVisible(&output_gain_label);

    base_labels.setVisible(false);
    lim_labels.setVisible(false);
    freq_labels.setVisible(false);
    addAndMakeVisible(&LB);

    addAndMakeVisible(&preset_bg);
    yt_button.setLookAndFeel(&presetlnf);
    pod_button.setLookAndFeel(&presetlnf);
    custom.setLookAndFeel(&presetlnf);

    yt_button.setRadioGroupId(PresetButtons);
    pod_button.setRadioGroupId(PresetButtons);
    custom.setRadioGroupId(PresetButtons);

    yt_button.onClick = [this]() { loadYoutube(); };
    yt_button.setEnabled(false);
    addAndMakeVisible(&yt_button);

    pod_button.onClick = [this]() {loadYoutube(); };
    pod_button.setEnabled(false);
        addAndMakeVisible(&pod_button);
        custom.onClick = [this]() {setCustom(); };
        custom.setEnabled(false);
        addAndMakeVisible(&custom);
        


    input_gain.onDragEnd = [this]() {undo.beginNewTransaction(); };
    output_gain.onDragEnd = [this]() {undo.beginNewTransaction(); };
    LB.onClick = [this]() {multiStateChange(Display_LB); };
    MB.onClick = [this]() {multiStateChange(Display_MB); };
    HB.onClick = [this]() {multiStateChange(Display_HB); };
    addAndMakeVisible(&MB);
    addAndMakeVisible(&HB);

    gate_x.setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    gate_x.setClickingTogglesState(true);
    gate_x.setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    allband_x.setClickingTogglesState(true);
    allband_x.setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    lowband_x.setClickingTogglesState(true);
    lowband_x.setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    midband_x.setClickingTogglesState(true);
    midband_x.setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    highband_x.setClickingTogglesState(true);
    highband_x.setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    limiter_x.setClickingTogglesState(true);
    limiter_x.setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    gate_x.onClick = [this]() {bypassToggle(&gate_x); undo.beginNewTransaction(); };
    allband_x.onClick = [this]() {bypassToggle(&allband_x); undo.beginNewTransaction(); };
    lowband_x.onClick = [this]() {bypassToggle(&lowband_x); undo.beginNewTransaction(); };
    midband_x.onClick = [this]() {bypassToggle(&midband_x); undo.beginNewTransaction(); };
    highband_x.onClick = [this]() {bypassToggle(&highband_x); undo.beginNewTransaction(); };
    limiter_x.onClick = [this]() {bypassToggle(&limiter_x); undo.beginNewTransaction(); };
    addAndMakeVisible(&gate_x);
    addAndMakeVisible(&allband_x);
    addAndMakeVisible(&lowband_x);
    addAndMakeVisible(&midband_x);
    addAndMakeVisible(&highband_x);
    addAndMakeVisible(&limiter_x);
    
    //Default Controls State
    selected_button = juce::Colours::darkcyan;
    Gate.setColour(juce::TextButton::buttonColourId, selected_button);
    APComp.setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    MBandComp.setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    Limiter.setColour(juce::TextButton::buttonColourId, juce::Colours::black);

    LB.setColour(juce::TextButton::buttonColourId, juce::Colours::midnightblue);
    MB.setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    HB.setColour(juce::TextButton::buttonColourId, juce::Colours::black);

    gateSliderComp.setVisible(false);
    apCompressorSliderComp.setVisible(false);
    lbCompressorSliderComp.setVisible(false);
    mbCompressorSliderComp.setVisible(false);
    hbCompressorSliderComp.setVisible(false);
    limSliderComp.setVisible(false);
    freqComp.setVisible(false);
    lim_labels.setVisible(false);
    multibuttonsInVisible();

    allband_x.setVisible(false);
    gate_x.setVisible(false);
    lowband_x.setVisible(false);
    midband_x.setVisible(false);
    highband_x.setVisible(false);
    limiter_x.setVisible(false);

    input_gain.setTextValueSuffix("db");
    output_gain.setTextValueSuffix("db");
    addAndMakeVisible(&Gate);
    Gate.setVisible(false);
    Gate.onClick = [this]() {changeControls(Display_Gate); };
    addAndMakeVisible(&APComp);
    APComp.onClick = [this]() {changeControls(Display_Allpass); };
    APComp.setVisible(false);
    addAndMakeVisible(&MBandComp);
    MBandComp.onClick = [this]() {changeControls(Display_MultiBand); };
    MBandComp.setVisible(false);
    addAndMakeVisible(&Limiter);
    Limiter.onClick = [this]() {changeControls(Display_Limiter); };
    Limiter.setVisible(false);

    input_gain_label.setVisible(false);
    output_gain_label.setVisible(false);

    
    out_waveform.set_as_output();

    //Footer Region
    addAndMakeVisible(&out_meterL);
    addAndMakeVisible(&out_meterR);
    addAndMakeVisible(&outAnalyse);
    addAndMakeVisible(&out_waveform);
    output_gain.setNormalisableRange(gain_range);
    //silence_samples.setRange(64, 44100, std::pow(silence_samples.getValue));
    //addAndMakeVisible(&silence_samples)
    addAndMakeVisible(&output_gain);
    outAnalyse.setVisible(false);

    addAndMakeVisible(&footer_Waveform_Btn);
    footer_Waveform_Btn.onClick = [this]() {in_waveform.setVisible(true); out_waveform.setVisible(true); inAnalyse.setVisible(false); outAnalyse.setVisible(false); };
    addAndMakeVisible(&footer_Frequency_Btn);
    footer_Frequency_Btn.onClick = [this]() {in_waveform.setVisible(false); out_waveform.setVisible(false); inAnalyse.setVisible(true); outAnalyse.setVisible(true); };

    input_gain.setVisible(false);
    output_gain.setVisible(false);
    //Footer Buttons
    addAndMakeVisible(&PLAY);
    PLAY.onClick = [this]() {playButtonClicked(); };
    PLAY.setEnabled(false);
    addAndMakeVisible(&STOP);
    STOP.onClick = [this]() {stopButtonClicked(); };
    STOP.setEnabled(false);
    addAndMakeVisible(&SAVE);
    SAVE.onClick = [this]() {saveButtonClicked(); };
    SAVE.setEnabled(false);
    SS.onClick = [this] { updateToggleState(&SS);   };
    addAndMakeVisible(&SS);
    SS.setEnabled(false);

    addAndMakeVisible(&ss_samples);
    ss_samples.setVisible(false);
    for (int i = 6; i <= std::log2(44100); ++i) {
        ss_samples.addItem(juce::String(std::pow(2, i)), std::pow(2, i));
    }

    ss_samples.setSelectedId(64);

    ss_samples.onChange = [this]() {undo.beginNewTransaction(); };

    ytpreset_save.onClick = [this] { pManage.savePreset("Youtube");   };
    //addAndMakeVisible(&ytpreset_save);

    podpreset_save.onClick = [this] { pManage.savePreset("PodCast");   };
    //addAndMakeVisible(&podpreset_save);




    menu_2.FileUpdate = [this]() {
        i_file = menu_2.input_file;
        in_waveform.local_file = menu_2.input_file;
        openOperation(i_file);
        };
    menu_2.SaveFile = [this]() {
        saveButtonClicked();
        };

    in_waveform.openFile = [this]() {
        i_file = in_waveform.local_file;
        menu_2.input_file = in_waveform.local_file;
        openOperation(i_file);
        };
}

MainComponent::~MainComponent()
{
    shutdownAudio();
    gateSliderComp.setLookAndFeel(nullptr);
    apCompressorSliderComp.setLookAndFeel(nullptr);
    lbCompressorSliderComp.setLookAndFeel(nullptr);
    mbCompressorSliderComp.setLookAndFeel(nullptr);
    hbCompressorSliderComp.setLookAndFeel(nullptr);
    input_gain.setLookAndFeel(nullptr);
    output_gain.setLookAndFeel(nullptr);
    freqComp.setLookAndFeel(nullptr);
    limSliderComp.setLookAndFeel(nullptr);

    input_gain_label.setLookAndFeel(nullptr);
    output_gain_label.setLookAndFeel(nullptr);

    yt_button.setLookAndFeel(nullptr);
    pod_button.setLookAndFeel(nullptr);
    custom.setLookAndFeel(nullptr);

}

void MainComponent::openOperation(juce::File& file) {
    auto* reader = formatManager.createReaderFor(file);

    if (reader != nullptr)
    {
        auto duration = (float)reader->lengthInSamples / reader->sampleRate;               // [3]

        if (duration < 18000) {
            auto newSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);
            transportSource.stop();
            transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
            in_waveform.setFile(file);
            in_waveform.launchThread();
            //in_waveform.info(new juce::FileInputSource(i_file));

            /*out_waveform.setFile(i_file);
            out_waveform.set_as_output();
            out_waveform.startThread();*/

            PLAY.setEnabled(true);
            SAVE.setEnabled(true);
            STOP.setEnabled(false);
            SS.setEnabled(true);
            readerSource.reset(newSource.release());

            yt_button.setEnabled(true);
            pod_button.setEnabled(true);
            custom.setEnabled(true);
            yt_button.setToggleState(true, juce::dontSendNotification);
            paramTree.setProperty(juce::Identifier{ Params::GetParams().at(Params::Names::Gain_Out) }, 0, nullptr);
            loadYoutube();
            undo.clearUndoHistory();
        }

        else {
            lengthAlert = std::make_unique<juce::AlertWindow>("ERROR", "Maximum file length is 5 hours", juce::MessageBoxIconType::WarningIcon, nullptr);

            lengthAlert->showMessageBoxAsync(juce::MessageBoxIconType::WarningIcon, "ERROR", "Maximum file length is 5 hours", "Ok", nullptr, nullptr);
        }
    }
}
//==============================================================================
void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);

    
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlockExpected;
    spec.numChannels = 2;
    spec.sampleRate = sampleRate;

    in_rmsLevel_L.reset(sampleRate, 0.02);
    in_rmsLevel_L.setCurrentAndTargetValue(-100.f);
    in_rmsLevel_R.reset(sampleRate, 0.02);
    in_rmsLevel_R.setCurrentAndTargetValue(-100.f);

    out_rmsLevel_L.reset(sampleRate, 0.02);
    out_rmsLevel_L.setCurrentAndTargetValue(-100.f);
    out_rmsLevel_R.reset(sampleRate, 0.02);
    out_rmsLevel_R.setCurrentAndTargetValue(-100.f);

    dspfunc.prepare(spec, samplesPerBlockExpected);
}
void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    if (readerSource.get() == nullptr)
    {
        bufferToFill.clearActiveBufferRegion();
        return;
    }
    transportSource.getNextAudioBlock(bufferToFill);

    juce::AudioBuffer<float> procBuf(bufferToFill.buffer->getArrayOfWritePointers(),
        bufferToFill.buffer->getNumChannels(),
        bufferToFill.startSample,
        bufferToFill.numSamples);

    dspfunc.applyInputGain(procBuf);


    in_rmsLevel_L.skip(bufferToFill.buffer->getNumSamples());
    in_rmsLevel_R.skip(bufferToFill.buffer->getNumSamples());
    {
        const auto value = juce::Decibels::gainToDecibels(bufferToFill.buffer->getRMSLevel(0, 0, bufferToFill.buffer->getNumSamples()));
        if (value < in_rmsLevel_L.getCurrentValue())
            in_rmsLevel_L.setTargetValue(value);
        else
            in_rmsLevel_L.setCurrentAndTargetValue(value);
    }

    {
        const auto value = juce::Decibels::gainToDecibels(bufferToFill.buffer->getRMSLevel(1, 0, bufferToFill.buffer->getNumSamples()));
        if (value < in_rmsLevel_R.getCurrentValue())
            in_rmsLevel_R.setTargetValue(value);
        else
            in_rmsLevel_R.setCurrentAndTargetValue(value);
    }

    auto* in_channelData = bufferToFill.buffer->getReadPointer(0, bufferToFill.startSample);

    for (auto i = 0; i < bufferToFill.numSamples; ++i)
        inAnalyse.pushNextSampleIntoFifo(in_channelData[i]);



    processBlock(procBuf);
    

    out_rmsLevel_L.skip(bufferToFill.buffer->getNumSamples());
    out_rmsLevel_R.skip(bufferToFill.buffer->getNumSamples());
    {
        const auto value = juce::Decibels::gainToDecibels(bufferToFill.buffer->getRMSLevel(0, 0, bufferToFill.buffer->getNumSamples()));
        if (value < out_rmsLevel_L.getCurrentValue())
            out_rmsLevel_L.setTargetValue(value);
        else
            out_rmsLevel_L.setCurrentAndTargetValue(value);
    }

    {
        const auto value = juce::Decibels::gainToDecibels(bufferToFill.buffer->getRMSLevel(1, 0, bufferToFill.buffer->getNumSamples()));
        if (value < out_rmsLevel_R.getCurrentValue())
            out_rmsLevel_R.setTargetValue(value);
        else
            out_rmsLevel_R.setCurrentAndTargetValue(value);
    }

    auto* out_channelData = bufferToFill.buffer->getReadPointer(0, bufferToFill.startSample);

    for (auto i = 0; i < bufferToFill.numSamples; ++i)
        outAnalyse.pushNextSampleIntoFifo(out_channelData[i]);

}
void MainComponent::processBlock(juce::AudioBuffer<float>& buffer) {

    juce::ScopedNoDenormals noDenormals;
    dspfunc.NoiseGateProcess(buffer);
    dspfunc.APCompressorProcess(buffer);
    dspfunc.MultibandComp(buffer);
    dspfunc.applyLimiter(buffer);
    dspfunc.applyOutputGain(buffer);

}
void MainComponent::releaseResources()
{
    transportSource.releaseResources();
    transportSource.setSource(nullptr);
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    

    switch(current_theme) {
        case LIGHT:
            g.setGradientFill(bg_gradient);
            g.fillAll();
            break;
        case DARK:
            g.setColour(juce::Colour::fromRGB(25, 25, 25));
            g.fillAll();
            break;
        default:
            break;
    }
    //
    
    
}

void MainComponent::resized()
{
    const auto bounds = getLocalBounds().toFloat();
    auto area = getLocalBounds();
    bg_gradient = juce::ColourGradient{ juce::Colours::silver, bounds.getTopLeft(), juce::Colours::grey, bounds.getBottomLeft(), false };

    input.setBounds(area.removeFromTop(200));
    buttons.setBounds(area.removeFromBottom(100));
    output.setBounds(area.removeFromBottom(200));
    preset.setBounds(area.removeFromLeft(300));
    controls.setBounds(area);

    undo_label.setBounds(menu_2.getLocalBounds().getCentreX(), menu_2.getLocalBounds().getCentreY() - 10, 150, 20);
    redo_label.setBounds(menu_2.getLocalBounds().getCentreX(), menu_2.getLocalBounds().getCentreY() - 10, 150, 20);

    //Input
    in_waveform.setBounds(input.getBounds().reduced(40));
    playback_bar.setBounds(in_waveform.getBounds());

    auto wav_bounds = in_waveform.getBounds();
    inAnalyse.setBounds(wav_bounds);
    in_meterL.setBounds(getLocalBounds().getX(), wav_bounds.getTopLeft().getY(), wav_bounds.getTopLeft().getX(), in_waveform.getHeight());
    in_meterR.setBounds(wav_bounds.getTopRight().getX(), wav_bounds.getTopRight().getY(), getLocalBounds().getRight()- wav_bounds.getTopRight().getX(), in_waveform.getHeight());

    juce::FlexBox input_view_flex;
    input_view_flex.flexDirection = juce::FlexBox::Direction::row;
    input_view_flex.flexWrap = juce::FlexBox::Wrap::noWrap;

    auto input_view_bounds = juce::Rectangle<float>(wav_bounds.getX(), wav_bounds.getBottom(), 200, 30);

    input_view_flex.items.add(juce::FlexItem(header_Waveform_Btn).withFlex(1.f));
    input_view_flex.items.add(juce::FlexItem(header_Frequency_Btn).withFlex(1.f));

    input_view_flex.performLayout(input_view_bounds);

    input_gain.setBounds(input_view_bounds.getRight(), input_view_bounds.getY(), 500, 50);
    input_gain_label.setBounds(input_gain.getRight(), input_gain.getY(), 80, input_gain.getHeight() - 10);

    //Presets
    juce::FlexBox preset_flex;
    preset_flex.flexDirection = juce::FlexBox::Direction::row;
    preset_flex.flexWrap = juce::FlexBox::Wrap::noWrap;

    preset_flex.items.add(juce::FlexItem(yt_button).withFlex(2.f));
    preset_flex.items.add(juce::FlexItem(pod_button).withFlex(2.f));
    preset_flex.items.add(juce::FlexItem(custom).withFlex(2.f));

    preset_bg.setBounds(preset.getBounds().reduced(10, 100));
    preset_flex.performLayout(preset.getBounds().reduced(10, 100));

    //Controls
    auto slider_bounds = controls.getBounds().removeFromRight(600).reduced(80);
    slider_bounds.expand(0, 10);
    slida.setBounds(slider_bounds);
    gateSliderComp.setBounds(slider_bounds);
    apCompressorSliderComp.setBounds(slider_bounds);
    hbCompressorSliderComp.setBounds(slider_bounds);
    mbCompressorSliderComp.setBounds(slider_bounds);
    lbCompressorSliderComp.setBounds(slider_bounds);
    limSliderComp.setBounds(slider_bounds);
    

    base_labels.setBounds(slider_bounds.getX(), slider_bounds.getBottom(), slider_bounds.getWidth(), 20);
    lim_labels.setBounds(base_labels.getBounds());
    freqComp.setBounds(slider_bounds.getX(), base_labels.getBottom(), slider_bounds.getWidth(), 50);
    freq_labels.setBounds(freqComp.getX(), freqComp.getBottom(), freqComp.getWidth(), 20);


    //Control_Controls XD
    auto controls_height = 50;
    auto control_bounds = juce::Rectangle<float>(slider_bounds.getX(), slider_bounds.getY()-controls_height, slider_bounds.getWidth(),controls_height);
    juce::FlexBox flex;
    flex.flexDirection = juce::FlexBox::Direction::row;
    flex.flexWrap = juce::FlexBox::Wrap::noWrap;


    flex.items.add(juce::FlexItem(Gate).withFlex(1.f));
    flex.items.add(juce::FlexItem(APComp).withFlex(1.f));
    flex.items.add(juce::FlexItem(MBandComp).withFlex(1.f));
    flex.items.add(juce::FlexItem(Limiter).withFlex(1.f));
    flex.performLayout(control_bounds);

    auto bypass_bounds = juce::Rectangle<int>(slider_bounds.getRight(), slider_bounds.getY(), 50, slider_bounds.getHeight());

    gate_x.setBounds(bypass_bounds);
    limiter_x.setBounds(bypass_bounds);
    allband_x.setBounds(bypass_bounds);
    //Multi Control Controls XD
    auto multi_controls_width = 50;
    auto multi_control_bounds = juce::Rectangle<float>(slider_bounds.getX() - multi_controls_width, slider_bounds.getY(), multi_controls_width, slider_bounds.getHeight());
    juce::FlexBox multi_flex;
    multi_flex.flexDirection = juce::FlexBox::Direction::column;
    multi_flex.flexWrap = juce::FlexBox::Wrap::noWrap;


    multi_flex.items.add(juce::FlexItem(LB).withFlex(1.f));
    multi_flex.items.add(juce::FlexItem(MB).withFlex(1.f));
    multi_flex.items.add(juce::FlexItem(HB).withFlex(1.f));
    multi_flex.performLayout(multi_control_bounds);

    juce::FlexBox multi_bypass_flex;
    multi_bypass_flex.flexDirection = juce::FlexBox::Direction::column;
    multi_bypass_flex.flexWrap = juce::FlexBox::Wrap::noWrap;

    multi_bypass_flex.items.add(juce::FlexItem(lowband_x).withFlex(1.f));
    multi_bypass_flex.items.add(juce::FlexItem(midband_x).withFlex(1.f));
    multi_bypass_flex.items.add(juce::FlexItem(highband_x).withFlex(1.f));
    multi_bypass_flex.performLayout(bypass_bounds);
    //Output
    outAnalyse.setBounds(output.getBounds().reduced(40));

    auto wav_out_bounds = outAnalyse.getBounds();
    out_waveform.setBounds(wav_out_bounds);

    out_meterL.setBounds(getLocalBounds().getX(), wav_out_bounds.getTopLeft().getY(), wav_out_bounds.getTopLeft().getX(), outAnalyse.getHeight());
    out_meterR.setBounds(wav_out_bounds.getTopRight().getX(), wav_out_bounds.getTopRight().getY(), getLocalBounds().getRight() - wav_out_bounds.getTopRight().getX(), outAnalyse.getHeight());

    juce::FlexBox output_view_flex;
    output_view_flex.flexDirection = juce::FlexBox::Direction::row;
    output_view_flex.flexWrap = juce::FlexBox::Wrap::noWrap;

    auto output_view_bounds = juce::Rectangle<float>(wav_out_bounds.getX(), wav_out_bounds.getBottom(), 200, 30);

    output_view_flex.items.add(juce::FlexItem(footer_Waveform_Btn).withFlex(1.f));
    output_view_flex.items.add(juce::FlexItem(footer_Frequency_Btn).withFlex(1.f));

    output_view_flex.performLayout(output_view_bounds);

    output_gain.setBounds(output_view_bounds.getRight(), output_view_bounds.getY(), 500, 50);
    output_gain_label.setBounds(output_gain.getRight(), output_gain.getY(), 80, output_gain.getHeight() - 10);

    //Final Buttons
    auto button_bounds = buttons.getBounds();
    PLAY.setBounds(button_bounds.getX(), button_bounds.getY(), button_bounds.getWidth()/2, button_bounds.getHeight() / 2);
    SAVE.setBounds(PLAY.getRight(), button_bounds.getY(), button_bounds.getWidth() / 2, button_bounds.getHeight() / 2);
    STOP.setBounds(PLAY.getX(),PLAY.getBottom(), button_bounds.getWidth() / 2, button_bounds.getHeight() / 2);
    SS.setBounds(STOP.getRight(),PLAY.getBottom(), SAVE.getWidth() / 2, button_bounds.getHeight() / 2);

    //ytpreset_save.setBounds(SS.getRight(), SS.getY(), SS.getWidth() / 2, SS.getHeight());
    //podpreset_save.setBounds(ytpreset_save.getRight(), ytpreset_save.getY(), SS.getWidth() / 2, ytpreset_save.getHeight());

    ss_samples.setBounds(SS.getRight(), SS.getY(), getLocalBounds().getRight() - SS.getRight(), SS.getHeight());
    //ss_samples.setBounds(ytpreset_save.getBounds());
}

void MainComponent::playButtonClicked()
{
    if ((state == Stopped) || (state == Paused))
        changeState(Starting);
    else if (state == Playing)
        changeState(Pausing);
    
}

void MainComponent::saveButtonClicked() {
    if (i_file != juce::File{}) {
        save_chooser = std::make_unique<juce::FileChooser>("Please name your file...",
            juce::File{},
            "*.wav");
        auto chooserFlags = juce::FileBrowserComponent::saveMode
            | juce::FileBrowserComponent::canSelectFiles | juce::FileBrowserComponent::warnAboutOverwriting;

        save_chooser->launchAsync(chooserFlags, [this](const juce::FileChooser& fc)
            {
                auto file = fc.getResult();
                save_thread.setFile(i_file, file);
                save_thread.launchThread();
            });
    }
    
}

void MainComponent::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    if (source == &transportSource)
    {
        if (transportSource.isPlaying())
            changeState(Playing);
        else if ((state == Stopping) || (state == Playing))
            changeState(Stopped);
        else if (Pausing == state)
            changeState(Paused);

        
    }

    
}

void MainComponent::changeState(TransportState newState)
{
    
    if (state != newState)
    {
        state = newState;

        switch (state)
        {
        case Stopped:                           
            STOP.setEnabled(false);
            PLAY.setEnabled(true);
            PLAY.setButtonText("PLAY");
            transportSource.setPosition(0.0);
            
            break;

        case Starting:                          
            transportSource.start();
            break;

        case Playing:                           
            STOP.setEnabled(true);
            PLAY.setButtonText("PAUSE");
            
            break;

        case Stopping:                          
            transportSource.stop();
            break;

        case Paused:
            PLAY.setButtonText("PLAY");;
            STOP.setEnabled(false);
            break;

        case Pausing:
            transportSource.stop();
            break;
        }
    }
}

void MainComponent::changeControls(Controls newState) {
    if (controlState != newState)
    {
        controlState = newState;

        switch (controlState)
        {
        case Display_Gate:
            controlButtonTheme(controlState);

            gateSliderComp.setVisible(true);
            apCompressorSliderComp.setVisible(false);
            hbCompressorSliderComp.setVisible(false);
            mbCompressorSliderComp.setVisible(false);
            lbCompressorSliderComp.setVisible(false);
            freqComp.setVisible(false);
            limSliderComp.setVisible(false);
            lim_labels.setVisible(false);
            multibuttonsInVisible();

            gate_x.setVisible(true);
            allband_x.setVisible(false);
            multiBypassInVisible();
            freq_labels.setVisible(false);
            limiter_x.setVisible(false);
            break;

        case Display_Allpass:
            controlButtonTheme(controlState);

            gateSliderComp.setVisible(false);
            apCompressorSliderComp.setVisible(true);
            hbCompressorSliderComp.setVisible(false);
            mbCompressorSliderComp.setVisible(false);
            lbCompressorSliderComp.setVisible(false);
            freqComp.setVisible(false);
            limSliderComp.setVisible(false);

            gate_x.setVisible(false);
            allband_x.setVisible(true);
            multiBypassInVisible();
            limiter_x.setVisible(false);
            multibuttonsInVisible();

            freq_labels.setVisible(false);
            lim_labels.setVisible(false);
            break;

        case Display_MultiBand:
            controlButtonTheme(controlState);

            gateSliderComp.setVisible(false);
            apCompressorSliderComp.setVisible(false);
            limSliderComp.setVisible(false);
            multibuttonsVisible();

            multiBypassVisible();
            gate_x.setVisible(false);
            allband_x.setVisible(false);
            limiter_x.setVisible(false);

            freqComp.setVisible(true);
            freq_labels.setVisible(true);
            multiStateChange(multibandState);

            lim_labels.setVisible(false);
            break;


        case Display_Limiter:
            controlButtonTheme(controlState);

            gateSliderComp.setVisible(false);
            apCompressorSliderComp.setVisible(false);
            hbCompressorSliderComp.setVisible(false);
            mbCompressorSliderComp.setVisible(false);
            lbCompressorSliderComp.setVisible(false);
            freq_labels.setVisible(false);
            freqComp.setVisible(false);
            limSliderComp.setVisible(true);
            multibuttonsInVisible();


            gate_x.setVisible(true);
            allband_x.setVisible(false);
            multiBypassInVisible();
            limiter_x.setVisible(true);

            lim_labels.setVisible(true);
            break;
        }

        
    }
}

void MainComponent::multiStateChange(MB_Controls newState) {
    multibandState = newState;

    switch (multibandState)
    {
    case Display_LB:
        LB.setColour(juce::TextButton::buttonColourId, juce::Colours::midnightblue);
        MB.setColour(juce::TextButton::buttonColourId, juce::Colours::black);
        HB.setColour(juce::TextButton::buttonColourId, juce::Colours::black);

        hbCompressorSliderComp.setVisible(false);
        mbCompressorSliderComp.setVisible(false);
        lbCompressorSliderComp.setVisible(true);
        break;

    case Display_MB:
        LB.setColour(juce::TextButton::buttonColourId, juce::Colours::black);
        MB.setColour(juce::TextButton::buttonColourId, juce::Colours::midnightblue);
        HB.setColour(juce::TextButton::buttonColourId, juce::Colours::black);

        hbCompressorSliderComp.setVisible(false);
        mbCompressorSliderComp.setVisible(true);
        lbCompressorSliderComp.setVisible(false);
        break;

    case Display_HB:
        LB.setColour(juce::TextButton::buttonColourId, juce::Colours::black);
        MB.setColour(juce::TextButton::buttonColourId, juce::Colours::black);
        HB.setColour(juce::TextButton::buttonColourId, juce::Colours::midnightblue);

        hbCompressorSliderComp.setVisible(true);
        mbCompressorSliderComp.setVisible(false);
        lbCompressorSliderComp.setVisible(false);
        break;
    }
}

void MainComponent::stopButtonClicked()
{
    changeState(Stopping);
}

bool MainComponent::keyPressed(const juce::KeyPress& key) {
    if (key == juce::KeyPress::spaceKey) {
        changeState(Stopping);
        return true;
    }
}

float MainComponent::get_in_RMSvalue(const int channel) const
{
    jassert(channel == 0 || channel == 1);
    if (channel == 0)
        return in_rmsLevel_L.getCurrentValue();
    if (channel == 1)
        return in_rmsLevel_R.getCurrentValue();
}

float MainComponent::get_out_RMSvalue(const int channel) const
{
    jassert(channel == 0 || channel == 1);
    if (channel == 0)
        return out_rmsLevel_L.getCurrentValue();
    if (channel == 1)
        return out_rmsLevel_R.getCurrentValue();
}

void MainComponent::updateToggleState(juce::Button* button)
{
    if (SS.getToggleState())
        ss_samples.setVisible(true);
    else if (!SS.getToggleState())
        ss_samples.setVisible(false);
}

void  MainComponent::bypassToggle(juce::Button* button) {
    if (button->getToggleState() == true){
        button->setColour(juce::TextButton::buttonOnColourId, juce::Colours::red);
        return;
}
    if (!button->getToggleState())
        button->setColour(juce::TextButton::buttonColourId, juce::Colours::black);
}

void MainComponent::loadYoutube() {
    if (yt_button.getToggleState()) {
        makeEverythingInVisible();
        
        pManage.loadPreset("Youtube");
        if (!in_waveform.isThreadRunning()) {
            paramTree.setProperty(juce::Identifier{ Params::GetParams().at(Params::Names::Gain_In) }, input_peak, nullptr);
            //auto gain = input_peak + (float)paramTree.getProperty(juce::Identifier{ Params::GetParams().at(Params::Names::Ratio_AllPass) }) * 1.0/((float)paramTree.getProperty(juce::Identifier{ Params::GetParams().at(Params::Names::Ratio_AllPass) }));
            //if (gain > 6)
            //    gain = 6;
            //paramTree.setProperty(juce::Identifier{ Params::GetParams().at(Params::Names::Gain_Out) }, gain , nullptr);
        }
            
    }
    
}

void MainComponent::loadPodcast() {
    makeEverythingInVisible();
    pManage.loadPreset("Podcast");
}

void MainComponent::setCustom() {
    makeEverythingVisible();
}