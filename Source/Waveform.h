/*
  ==============================================================================

    Waveform.h
    Created: 31 Mar 2024 8:29:03pm
    Author:  Simeon

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

namespace GUI {
    class Waveform :
        public juce::Component,
        public juce::ChangeListener,
        public juce::ThreadWithProgressWindow
    {
    public:
        Waveform() : thumbnailCache(5),
            thumbnail(512, formatManager, thumbnailCache),
            juce::ThreadWithProgressWindow("Loading File...", true, false)
        {
            thumbnail.addChangeListener(this);
            formatManager.registerBasicFormats();
        }
        ~Waveform() {

            stopThread(1000);
        }
        void setAsDark() {
            current_theme = DARK;
            repaint();
        }

        void setAsLight() {
            current_theme = LIGHT;
            repaint();
        }


        std::function<void()> openFile = [](){};

        void info(juce::FileInputSource* source) {
            thumbnail.setSource(source);
        }

        void setFile(juce::File opened_File) {
            input_file = opened_File;
        }

        void set_as_output() {
            is_output_wavform = true;

        }
        float getPeak() {
            return peak;
        }
        void run() override {
            loadData(input_file);
        }
        void loadData(juce::File file) {

            if (auto fileStream = std::unique_ptr<juce::FileInputStream>(file.createInputStream())) {

                juce::AudioBuffer<float> procBuf;

                

                auto* reader = formatManager.createReaderFor(file);



                juce::int64 counter = 0;
                int len = (int)reader->lengthInSamples;
                procBuf.setSize((int)reader->numChannels, 64);

                if (reader != nullptr) {
                    thumbnail.reset(procBuf.getNumChannels(), reader->sampleRate, procBuf.getNumSamples());
                    if (!is_output_wavform) {
                        while (counter < len) {
                            

                            reader->read(&procBuf, 0, procBuf.getNumSamples(), counter, true, true);
                            if (counter == 0) {
                                peak = juce::Decibels::gainToDecibels(procBuf.getMagnitude(0, 0, procBuf.getNumSamples()));
                                if (peak > 0)
                                    peak = 0;
                            }
                            else {
                                peak = juce::jmax(peak, juce::Decibels::gainToDecibels(procBuf.getMagnitude(0, 0, procBuf.getNumSamples())));
                                jassert(!peak == 0);
                                if (peak > 0.0)
                                    peak = 0;
                            }
                            thumbnail.addBlock(counter, procBuf, 0, procBuf.getNumSamples());
                            DBG(peak);
                            setProgress(counter / (double)reader->lengthInSamples);
                            counter += procBuf.getNumSamples();
                            procBuf.clear();
                        }
                    }
                    else
                    {
                        while (counter < len) {
                            dsp_prepare(reader->sampleRate, procBuf.getNumSamples());
                            reader->read(&procBuf, 0, procBuf.getNumSamples(), counter, true, true);
                            //dspfunc.NoiseGateProcess(procBuf);
                            //dspfunc.APCompressorProcess(procBuf);
                            //dspfunc.MultibandComp(procBuf);
                            thumbnail.addBlock(counter, procBuf, 0, procBuf.getNumSamples());
                            counter += procBuf.getNumSamples();
                            procBuf.clear();
                        }
                    }

                }
                delete reader;
            }
            thumbnail_loaded = true;
            DBG(peak);
            signalThreadShouldExit();

        }

        void dsp_prepare(double sample_rate, int samplesPerBlockExpected) {

            juce::dsp::ProcessSpec spec;
            spec.maximumBlockSize = samplesPerBlockExpected;
            spec.numChannels = 2;
            spec.sampleRate = sample_rate;

            //dspfunc.prepare(spec, samplesPerBlockExpected);
        }

        void paint(juce::Graphics& g) override {
            juce::Rectangle<int> thumbnailBounds = getLocalBounds();

            if (is_output_wavform) {
                paintIfOutput(g, thumbnailBounds);
                return;
            }

            if (thumbnail.getNumChannels() == 0)
                paintIfNoFileLoaded(g, thumbnailBounds);
            else
                paintIfFileLoaded(g, thumbnailBounds);



        }

        void paintIfNoFileLoaded(juce::Graphics& g, const juce::Rectangle<int>& thumbnailBounds)
        {
            switch (current_theme) {
            case DARK:
                g.setColour(juce::Colour::fromRGB(48, 51, 60));
                g.fillRect(thumbnailBounds);
                g.setColour(juce::Colours::white);
                g.drawRect(thumbnailBounds);
                g.drawFittedText("No File Loaded", thumbnailBounds, juce::Justification::centred, 1);
                break;

            case LIGHT:
                g.setColour(juce::Colours::darkgrey);
                g.fillRect(thumbnailBounds);
                g.setColour(juce::Colours::white);
                g.drawFittedText("No File Loaded", thumbnailBounds, juce::Justification::centred, 1);
                break;
            }

           
        }
            

        void paintIfFileLoaded(juce::Graphics& g, const juce::Rectangle<int>& thumbnailBounds)
        {
            switch(current_theme) {
                case DARK:
                    g.setColour(juce::Colours::dimgrey);
                    g.fillRect(thumbnailBounds);

                    g.setColour(juce::Colours::white);
                    g.drawRect(thumbnailBounds);


                    g.setColour(juce::Colour::fromRGB(57 + 100, 96 + 100, 187 + 50));

                    thumbnail.drawChannels(g,
                        thumbnailBounds.reduced(1),
                        0.0,
                        thumbnail.getTotalLength(),
                        1.0f);
                    break;

                case LIGHT:
                    g.setColour(juce::Colours::dimgrey);
                    g.fillRect(thumbnailBounds);

                    /*g.setColour(juce::Colours::white);
                    g.drawRect(thumbnailBounds);*/


                    g.setColour(juce::Colours::cornflowerblue);

                    thumbnail.drawChannels(g,
                        thumbnailBounds,
                        0.0,
                        thumbnail.getTotalLength(),
                        1.0f);
                    break;
            }
            
            //g.setColour(juce::Colours::cornflowerblue);

            
        }

        void paintIfOutput(juce::Graphics& g, const juce::Rectangle<int>& thumbnailBounds) {
            switch (current_theme) {
            case DARK:
                g.setColour(juce::Colour::fromRGB(48, 51, 60));
                g.fillRect(thumbnailBounds);
                g.setColour(juce::Colours::white);
                g.drawRect(thumbnailBounds);
                g.drawFittedText("COMING SOON", thumbnailBounds, juce::Justification::centred, 1);
                break;

            case LIGHT:
                g.setColour(juce::Colours::darkgrey);
                g.fillRect(thumbnailBounds);
                g.setColour(juce::Colours::white);
                g.drawFittedText("COMING SOON", thumbnailBounds, juce::Justification::centred, 1);
                break;
            }
        }
        void changeListenerCallback(juce::ChangeBroadcaster* source) override
        {
            if (source == &thumbnail)
                repaint();
        }

        juce::AudioThumbnail thumbnail;
    juce::File local_file;
    private:
        bool is_output_wavform = false;
        //DSPFunctions dspfunc;
        juce::AudioFormatManager formatManager;
        juce::AudioThumbnailCache thumbnailCache;
        juce::File input_file;
        float peak = 0.0;
        juce::Colour waveform, bg, no_file;
        enum Theme {
            DARK,
            LIGHT
        };

        Theme current_theme = LIGHT;
        bool thumbnail_loaded = false;
    };

    class Wav_Bar : public juce::Component,
        private juce::Timer
    {
    public:
        Wav_Bar(juce::AudioTransportSource& transportSourceToUse)
            : transportSource(transportSourceToUse)
        {
            startTimer(40);
        }

        void paint(juce::Graphics& g) override
        {
            auto duration = (float)transportSource.getLengthInSeconds();

            if (isMouseOver() && duration > 0.0) {
                auto clickPosition = getMouseXYRelative().getX();
                g.setColour(juce::Colours::black);
                g.drawLine(clickPosition, 0.0f, clickPosition, (float)getHeight(), 2.0f);
            }
            if (duration > 0.0)
            {
                auto audioPosition = (float)transportSource.getCurrentPosition();
                auto drawPosition = (audioPosition / duration) * (float)getWidth();

                g.setColour(juce::Colours::green);
                g.drawLine(drawPosition, 0.0f, drawPosition, (float)getHeight(), 2.0f);
            }
        }

        void mouseDown(const juce::MouseEvent& event) override
        {
            auto duration = transportSource.getLengthInSeconds();

            if (duration > 0.0)
            {
                auto clickPosition = event.position.x;
                auto audioPosition = (clickPosition / (float)getWidth()) * duration;

                transportSource.setPosition(audioPosition);
            }
        }

    private:
        void timerCallback() override
        {
            repaint();
        }

        juce::AudioTransportSource& transportSource;
        bool mouse_mov = false;
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Wav_Bar)
    };
    
    class SliderLnF : public::juce::Slider::LookAndFeelMethods {
    public:
        SliderLnF()
        {
            
        }

        
    };

    class customLnF : public::juce::LookAndFeel_V4 {

    public:

        customLnF() {
            
        }
        void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
            const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider&) override
        {
            auto radius = (float)juce::jmin(width / 2, height / 2);
            auto centreX = (float)x + (float)width * 0.5f;
            auto centreY = (float)y + (float)height * 0.5f;
            auto rx = centreX - radius;
            auto ry = centreY - radius;
            auto rw = radius * 2.0f;
            auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

            // fill
            g.setColour(juce::Colours::darkgrey);
            g.fillEllipse(rx, ry, rw, rw);

            // outline
            g.setColour(juce::Colours::midnightblue);
            g.drawEllipse(rx, ry, rw, rw, 1.0f);

            juce::Path p;
            auto pointerLength = radius * 0.33f;
            auto pointerThickness = 2.0f;
            p.addRectangle(-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
            p.applyTransform(juce::AffineTransform::rotation(angle).translated(centreX, centreY));

            // pointer
            g.setColour(juce::Colours::azure);
            g.fillPath(p);
        }

    };
}

