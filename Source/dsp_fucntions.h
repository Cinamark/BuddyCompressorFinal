/*
  ==============================================================================

    dsp_fucntions.h
    Created: 31 Mar 2024 10:47:34am
    Author:  Simeon

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "ParamsMap.h"

struct CompressorBand {
public:
    void prepare(const juce::dsp::ProcessSpec& spec) {
        compressor.prepare(spec);
        smoother.reset(spec.sampleRate, 0.5);
    }
    void setTree(juce::ValueTree tree_link) {
        tree = tree_link;
    }
    void l_parameterUpdate() {
        const auto& params = Params::GetParams();
        compressor.setAttack(tree[juce::Identifier{ params.at(Params::Names::Attack_Low_Band) }]);
        compressor.setRelease(tree[juce::Identifier{ params.at(Params::Names::Release_Low_Band) }]);
        compressor.setThreshold(tree[juce::Identifier{ params.at(Params::Names::Release_Low_Band) }]);
        //smoother.setTargetValue(tree[juce::Identifier{ params.at(Params::Names::Threshold_Low_Band) }]);
        compressor.setRatio(tree[juce::Identifier{ params.at(Params::Names::Ratio_Low_Band) }]);

        bypass = tree[juce::Identifier{ params.at(Params::Names::Bypass_Low) }];
    }

    void m_parameterUpdate() {
        const auto& params = Params::GetParams();
        compressor.setAttack(tree[juce::Identifier{ params.at(Params::Names::Attack_Mid_Band) }]);
        compressor.setRelease(tree[juce::Identifier{ params.at(Params::Names::Release_Mid_Band) }]);
        compressor.setThreshold(tree[juce::Identifier{ params.at(Params::Names::Threshold_Mid_Band) }]);
        //smoother.setTargetValue(tree[juce::Identifier{ params.at(Params::Names::Threshold_Mid_Band) }]);
        compressor.setRatio(tree[juce::Identifier{ params.at(Params::Names::Ratio_Mid_Band) }]);

        bypass = tree[juce::Identifier{ params.at(Params::Names::Bypass_Mid) }];
    }

    void h_parameterUpdate() {
        const auto& params = Params::GetParams();
        compressor.setAttack(tree[juce::Identifier{ params.at(Params::Names::Attack_High_Band) }]);
        compressor.setRelease(tree[juce::Identifier{ params.at(Params::Names::Release_High_Band) }]);
        compressor.setThreshold(tree[juce::Identifier{ params.at(Params::Names::Threshold_High_Band) }]);
        //smoother.setTargetValue(tree[juce::Identifier{ params.at(Params::Names::Threshold_High_Band) }]);
        compressor.setRatio(tree[juce::Identifier{ params.at(Params::Names::Ratio_High_Band) }]);

        bypass = tree[juce::Identifier{ params.at(Params::Names::Bypass_High) }];
    }

    void save_parameterUpdate() {
        const auto& params = Params::GetParams();
        compressor.setAttack(tree[juce::Identifier{ params.at(Params::Names::Attack_Low_Band) }]);
        compressor.setRelease(tree[juce::Identifier{ params.at(Params::Names::Release_Low_Band) }]);
        compressor.setThreshold(tree[juce::Identifier{ params.at(Params::Names::Threshold_Low_Band) }]);
        compressor.setRatio(tree[juce::Identifier{ params.at(Params::Names::Ratio_Low_Band) }]);
    }

    void save_process(juce::AudioBuffer<float>& buffer) {
        auto block = juce::dsp::AudioBlock<float>(buffer);
        auto context = juce::dsp::ProcessContextReplacing<float>(block);
        compressor.process(context);
    }

    void process(juce::AudioBuffer<float>& buffer) {
        auto block = juce::dsp::AudioBlock<float>(buffer);
        auto context = juce::dsp::ProcessContextReplacing<float>(block);

        //compressor.setThreshold(smoother.getNextValue());


        /*for (int s = 0; s < buffer.getNumSamples(); ++s)
        {
            compressor.setThreshold(smoother.getNextValue());


            for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
            {
                auto* channelData = buffer.getWritePointer(ch);
                channelData[s] = compressor.processSample(ch, channelData[s]);
            }

        }*/
        compressor.process(context);
    }
private:
    juce::dsp::Compressor<float> compressor;
    juce::ValueTree tree;
    juce::LinearSmoothedValue<float> smoother{ 0.0f };
    bool bypass;
};

class ReferenceCountedBuffer : public juce::ReferenceCountedObject
{
public:
    typedef juce::ReferenceCountedObjectPtr<ReferenceCountedBuffer> Ptr;

    ReferenceCountedBuffer(const juce::String& nameToUse,
        int numChannels,
        int numSamples)
        : name(nameToUse),
        buffer(numChannels, numSamples)
    {
        DBG(juce::String("Buffer named '") + name + "' constructed. numChannels = " + juce::String(numChannels) + ", numSamples = " + juce::String(numSamples));
    }

    ~ReferenceCountedBuffer()
    {
        DBG(juce::String("Buffer named '") + name + "' destroyed");
    }

    juce::AudioSampleBuffer* getAudioSampleBuffer()
    {
        return &buffer;
    }

    int position = 0;

private:
    juce::String name;
    juce::AudioSampleBuffer buffer;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ReferenceCountedBuffer)
};


template<typename T, typename U>
void applyGain(T& buffer, U& gain)
{
    auto block = juce::dsp::AudioBlock<float>(buffer);
    auto ctx = juce::dsp::ProcessContextReplacing<float>(block);
    gain.process(ctx);
}

class DSPFunctions {
public:

    DSPFunctions(){}
    void prepare(const juce::dsp::ProcessSpec& spec) {
        gate.prepare(spec);
    }
    void setTree(juce::ValueTree tree) {
        this->tree = tree;
    }
    void NoiseGateProcess( juce::dsp::AudioBlock<float>& block) {
        const auto& params = Params::GetParams();
        gate.setAttack(tree[juce::Identifier{ params.at(Params::Names::Gate_Attack) }]);
        gate.setRelease(tree[juce::Identifier{ params.at(Params::Names::Gate_Release) }]);
        gate.setThreshold(tree[juce::Identifier{ params.at(Params::Names::Gate_Thresh) }]);
        gate.setRatio((tree[juce::Identifier{ params.at(Params::Names::Gate_Ratio) }]));

        auto ctx = juce::dsp::ProcessContextReplacing<float>(block);
        gate.process(ctx);
    }

private:
    juce::dsp::Compressor<float> gate;
    juce::ValueTree tree;
};