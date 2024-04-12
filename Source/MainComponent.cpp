#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
    : menu_2(i_file), playback_bar(transportSource), state(Stopped), controlState(Display_Gate), multibandState(Display_LB),
    in_meterL([&]() {return get_in_RMSvalue(0); }), in_meterR([&]() {return get_in_RMSvalue(1); }),
    gateSliderComp{ paramTree, &undo },
    apCompressorSliderComp { paramTree, &undo },
    lbCompressorSliderComp{ paramTree, &undo },
    mbCompressorSliderComp{ paramTree, &undo },
    hbCompressorSliderComp{ paramTree, &undo },
    freqComp{ paramTree, &undo }
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

    //startTimerHz(24);
    dspfunc.setTree(paramTree);
    formatManager.registerBasicFormats();
    transportSource.addChangeListener(this);
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
    addAndMakeVisible(&waveform);
    addAndMakeVisible(&playback_bar);
    addAndMakeVisible(&in_meterL);
    addAndMakeVisible(&in_meterR);

    //Controls
    addAndMakeVisible(&gateSliderComp);
    addAndMakeVisible(&apCompressorSliderComp);
    addAndMakeVisible(&lbCompressorSliderComp);
    addAndMakeVisible(&mbCompressorSliderComp);
    addAndMakeVisible(&hbCompressorSliderComp);
    addAndMakeVisible(&freqComp);
    addAndMakeVisible(&LB);
    LB.onClick = [this]() {multiStateChange(Display_LB); };
    MB.onClick = [this]() {multiStateChange(Display_MB); };
    HB.onClick = [this]() {multiStateChange(Display_HB); };
    addAndMakeVisible(&MB);
    addAndMakeVisible(&HB);
    
    //Default Controls State
    Gate.setColour(juce::TextButton::buttonColourId, juce::Colours::chocolate);
    APComp.setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    MBandComp.setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    Limiter.setColour(juce::TextButton::buttonColourId, juce::Colours::black);

    gateSliderComp.setVisible(true);
    apCompressorSliderComp.setVisible(false);
    lbCompressorSliderComp.setVisible(false);
    mbCompressorSliderComp.setVisible(false);
    hbCompressorSliderComp.setVisible(false);
    freqComp.setVisible(false);
    multibuttonsInVisible();


    addAndMakeVisible(&Gate);
    Gate.onClick = [this]() {changeControls(Display_Gate); };
    addAndMakeVisible(&APComp);
    APComp.onClick = [this]() {changeControls(Display_Allpass); };
    addAndMakeVisible(&MBandComp);
    MBandComp.onClick = [this]() {changeControls(Display_MultiBand); };
    addAndMakeVisible(&Limiter);
    //Footer Region
    addAndMakeVisible(&PLAY);
    PLAY.onClick = [this]() {playButtonClicked(); };
    PLAY.setEnabled(false);
    addAndMakeVisible(&STOP);
    STOP.onClick = [this]() {stopButtonClicked(); };
    STOP.setEnabled(false);

    menu_2.FileUpdate = [this]() {
        i_file = menu_2.input_file;
        auto* reader = formatManager.createReaderFor(i_file);

        if (reader != nullptr)
        {
            auto newSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);
            transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
            waveform.setFile(i_file);
            waveform.startThread();
            PLAY.setEnabled(true);
            STOP.setEnabled(false);
            readerSource.reset(newSource.release());
        }
        };
}

MainComponent::~MainComponent()
{
    shutdownAudio();
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

    processBlock(procBuf);
    


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
}
void MainComponent::processBlock(juce::AudioBuffer<float>& buffer) {
    auto block = juce::dsp::AudioBlock<float>(buffer);
    //dspfunc.NoiseGateProcess(block);
    //dspfunc.APCompressorProcess(block);
    dspfunc.MultibandComp(buffer);
}
void MainComponent::releaseResources()
{
    transportSource.releaseResources();
    transportSource.setSource(nullptr);
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    
    /*background = juce::ImageCache::getFromMemory(BinaryData::Metal009_4KJPG_Color_jpg, BinaryData::Metal009_4KJPG_Color_jpgSize);
    g.drawImageWithin(background, 0, 0, getWidth(), getHeight(), juce::RectanglePlacement::doNotResize);*/

    g.setGradientFill(bg_gradient);
    g.fillAll();
    
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

    //Input
    waveform.setBounds(input.getBounds().reduced(40));
    playback_bar.setBounds(waveform.getBounds());

    auto wav_bounds = waveform.getBounds();
    
    in_meterL.setBounds(getLocalBounds().getX(), wav_bounds.getTopLeft().getY(), wav_bounds.getTopLeft().getX(), waveform.getHeight());
    in_meterR.setBounds(wav_bounds.getTopRight().getX(), wav_bounds.getTopRight().getY(), getLocalBounds().getRight()- wav_bounds.getTopRight().getX(), waveform.getHeight());

    //Controls
    auto slider_bounds = controls.getBounds().removeFromRight(600).reduced(80);
    slida.setBounds(slider_bounds);
    gateSliderComp.setBounds(slider_bounds);
    apCompressorSliderComp.setBounds(slider_bounds);
    hbCompressorSliderComp.setBounds(slider_bounds);
    mbCompressorSliderComp.setBounds(slider_bounds);
    lbCompressorSliderComp.setBounds(slider_bounds);
    freqComp.setBounds(slider_bounds.getX(), slider_bounds.getBottom(), slider_bounds.getWidth(), 50);


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


    //Output

    auto button_bounds = buttons.getBounds();
    PLAY.setBounds(button_bounds.removeFromTop(40));
    STOP.setBounds(button_bounds.removeFromTop(40));
}

void MainComponent::playButtonClicked()
{
    if ((state == Stopped) || (state == Paused))
        changeState(Starting);
    else if (state == Playing)
        changeState(Pausing);
    
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
            //saveButton.setEnabled(true);
            break;

        case Starting:                          
            transportSource.start();
            break;

        case Playing:                           
            STOP.setEnabled(true);
            PLAY.setButtonText("PAUSE");
            //audio_level.setEnabled(true);
            break;

        case Stopping:                          
            transportSource.stop();
            break;

        case Paused:
            PLAY.setButtonText("PLAY");;
            STOP.setEnabled(false);

        case Pausing:
            transportSource.stop();
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
            Gate.setColour(juce::TextButton::buttonColourId, juce::Colours::chocolate);
            APComp.setColour(juce::TextButton::buttonColourId, juce::Colours::black);
            MBandComp.setColour(juce::TextButton::buttonColourId, juce::Colours::black);
            Limiter.setColour(juce::TextButton::buttonColourId, juce::Colours::black);

            gateSliderComp.setVisible(true);
            apCompressorSliderComp.setVisible(false);
            hbCompressorSliderComp.setVisible(false);
            mbCompressorSliderComp.setVisible(false);
            lbCompressorSliderComp.setVisible(false);
            freqComp.setVisible(false);
            multibuttonsInVisible();
            break;

        case Display_Allpass:
            Gate.setColour(juce::TextButton::buttonColourId, juce::Colours::black);
            APComp.setColour(juce::TextButton::buttonColourId, juce::Colours::chocolate);
            MBandComp.setColour(juce::TextButton::buttonColourId, juce::Colours::black);
            Limiter.setColour(juce::TextButton::buttonColourId, juce::Colours::black);

            gateSliderComp.setVisible(false);
            apCompressorSliderComp.setVisible(true);
            hbCompressorSliderComp.setVisible(false);
            mbCompressorSliderComp.setVisible(false);
            lbCompressorSliderComp.setVisible(false);
            freqComp.setVisible(false);
            multibuttonsInVisible();
            break;

        case Display_MultiBand:
            Gate.setColour(juce::TextButton::buttonColourId, juce::Colours::black);
            APComp.setColour(juce::TextButton::buttonColourId, juce::Colours::black);
            MBandComp.setColour(juce::TextButton::buttonColourId, juce::Colours::chocolate);
            Limiter.setColour(juce::TextButton::buttonColourId, juce::Colours::black);

            gateSliderComp.setVisible(false);
            apCompressorSliderComp.setVisible(false);
            multibuttonsVisible();
            freqComp.setVisible(true);
            multiStateChange(multibandState);
            break;
        }
    }
}

void MainComponent::multiStateChange(MB_Controls newState) {
    multibandState = newState;

    switch (multibandState)
    {
    case Display_LB:
        hbCompressorSliderComp.setVisible(false);
        mbCompressorSliderComp.setVisible(false);
        lbCompressorSliderComp.setVisible(true);
        break;

    case Display_MB:
        hbCompressorSliderComp.setVisible(false);
        mbCompressorSliderComp.setVisible(true);
        lbCompressorSliderComp.setVisible(false);
        break;

    case Display_HB:
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
