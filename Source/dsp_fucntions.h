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

    DSPFunctions(){
        LP1.setType(juce::dsp::LinkwitzRileyFilterType::lowpass);
        LP2.setType(juce::dsp::LinkwitzRileyFilterType::lowpass);
        HP1.setType(juce::dsp::LinkwitzRileyFilterType::highpass);
        HP2.setType(juce::dsp::LinkwitzRileyFilterType::highpass);
        AP2.setType(juce::dsp::LinkwitzRileyFilterType::allpass);
    }
    void prepare(const juce::dsp::ProcessSpec& spec, int samplesPerBlockExpected) {
        gate.prepare(spec);
        comp.prepare(spec);
        limiter.prepare(spec);
        for (auto& compressor : multi_compressors)
            compressor.prepare(spec);
        for (auto& buffer : filterBuffers) {
            buffer.setSize(spec.numChannels, samplesPerBlockExpected);
        }

        LP1.prepare(spec);
        LP2.prepare(spec);
        HP1.prepare(spec);
        HP2.prepare(spec);
        AP2.prepare(spec);

        inputGain.prepare(spec);
        outputGain.prepare(spec);

        inputGain.setRampDurationSeconds(0.05);
        outputGain.setRampDurationSeconds(0.05);

    }

    void setTree(juce::ValueTree tree) {
        this->tree = tree;
    }
    void NoiseGateProcess(juce::AudioBuffer<float>& buffer) {
        const auto& params = Params::GetParams();
        gate.setAttack(tree[juce::Identifier{ params.at(Params::Names::Gate_Attack) }]);
        gate.setRelease(tree[juce::Identifier{ params.at(Params::Names::Gate_Release) }]);
        gate.setThreshold(tree[juce::Identifier{ params.at(Params::Names::Gate_Thresh) }]);
        gate.setRatio((tree[juce::Identifier{ params.at(Params::Names::Gate_Ratio) }]));

        auto block = juce::dsp::AudioBlock<float>(buffer);
        auto ctx = juce::dsp::ProcessContextReplacing<float>(block);

        ctx.isBypassed = tree[juce::Identifier{ params.at(Params::Names::Bypass_Gate) }];
        gate.process(ctx);
    }

    void APCompressorProcess(juce::AudioBuffer<float>& buffer) {
            const auto& params = Params::GetParams();
            comp.setAttack(tree[juce::Identifier{ params.at(Params::Names::Attack_AllPass) }]);
            comp.setRelease(tree[juce::Identifier{ params.at(Params::Names::Release_AllPass) }]);
            comp.setThreshold(tree[juce::Identifier{ params.at(Params::Names::Threshold_AllPass) }]);
            comp.setRatio((tree[juce::Identifier{ params.at(Params::Names::Ratio_AllPass) }]));

            auto block = juce::dsp::AudioBlock<float>(buffer);
            auto ctx = juce::dsp::ProcessContextReplacing<float>(block);

            ctx.isBypassed = tree[juce::Identifier{ params.at(Params::Names::Bypass_Allpass) }];
            comp.process(ctx);

    }

    void MultibandComp(juce::AudioBuffer<float>& buffer) {
        for (auto& fb : filterBuffers)
            fb = buffer;

        const auto& params = Params::GetParams();
        auto lowMidCutoff = tree[juce::Identifier{ params.at(Params::Names::Low_Mid_Crossover_Freq) }];
        LP1.setCutoffFrequency(lowMidCutoff);
        HP1.setCutoffFrequency(lowMidCutoff);

        auto midHighCutoff = tree[juce::Identifier{ params.at(Params::Names::Mid_High_Crossover_Freq) }];
        HP2.setCutoffFrequency(midHighCutoff);
        LP2.setCutoffFrequency(midHighCutoff);
        AP2.setCutoffFrequency(midHighCutoff);

        auto fb0Block = juce::dsp::AudioBlock<float>(filterBuffers[0]);
        auto fb1Block = juce::dsp::AudioBlock<float>(filterBuffers[1]);
        auto fb2Block = juce::dsp::AudioBlock<float>(filterBuffers[2]);
        auto fb0Ctx = juce::dsp::ProcessContextReplacing<float>(fb0Block);
        auto fb1Ctx = juce::dsp::ProcessContextReplacing<float>(fb1Block);
        auto fb2Ctx = juce::dsp::ProcessContextReplacing<float>(fb2Block);

        LP1.process(fb0Ctx);
        AP2.process(fb0Ctx);

        

        HP1.process(fb1Ctx);
        filterBuffers[2] = filterBuffers[1];
        LP2.process(fb1Ctx);
        HP2.process(fb2Ctx);

        multi_compressors[0].setAttack(tree[juce::Identifier{ params.at(Params::Names::Attack_Low_Band) }]);
        multi_compressors[0].setRelease(tree[juce::Identifier{ params.at(Params::Names::Release_Low_Band) }]);
        multi_compressors[0].setThreshold(tree[juce::Identifier{ params.at(Params::Names::Threshold_Low_Band) }]);
        multi_compressors[0].setRatio(tree[juce::Identifier{ params.at(Params::Names::Ratio_Low_Band) }]);

        multi_compressors[1].setAttack(tree[juce::Identifier{ params.at(Params::Names::Attack_Mid_Band) }]);
        multi_compressors[1].setRelease(tree[juce::Identifier{ params.at(Params::Names::Release_Mid_Band) }]);
        multi_compressors[1].setThreshold(tree[juce::Identifier{ params.at(Params::Names::Threshold_Mid_Band) }]);
        multi_compressors[1].setRatio(tree[juce::Identifier{ params.at(Params::Names::Ratio_Mid_Band) }]);

        multi_compressors[2].setAttack(tree[juce::Identifier{ params.at(Params::Names::Attack_High_Band) }]);
        multi_compressors[2].setRelease(tree[juce::Identifier{ params.at(Params::Names::Release_High_Band) }]);
        multi_compressors[2].setThreshold(tree[juce::Identifier{ params.at(Params::Names::Threshold_High_Band) }]);
        multi_compressors[2].setRatio(tree[juce::Identifier{ params.at(Params::Names::Ratio_High_Band) }]);

        for (size_t i = 0; i < filterBuffers.size(); ++i) {
            auto block = juce::dsp::AudioBlock<float>(filterBuffers[i]);
            auto ctx = juce::dsp::ProcessContextReplacing<float>(block);

            switch (i)
            {
            case 0:
                ctx.isBypassed = tree[juce::Identifier{ params.at(Params::Names::Bypass_Low) }];
                break;
            
            case 1:
                ctx.isBypassed = tree[juce::Identifier{ params.at(Params::Names::Bypass_Mid) }];
                break;
        
            case 2:
            ctx.isBypassed = tree[juce::Identifier{ params.at(Params::Names::Bypass_High) }];
            break;
            }
            multi_compressors[i].process(ctx);
        }

        auto numSamples = buffer.getNumSamples();
        auto numChannels = buffer.getNumChannels();

        buffer.clear();

        auto addFilterBand = [nc = numChannels, ns = numSamples](auto& inputBuffer, const auto& source) {
            for (auto i = 0; i < nc; ++i)
                inputBuffer.addFrom(i, 0, source, i, 0, ns);
            };

        addFilterBand(buffer, filterBuffers[0]);
        addFilterBand(buffer, filterBuffers[1]);
        addFilterBand(buffer, filterBuffers[2]);
    }

    void applyInputGain(juce::AudioBuffer<float>& buffer) {
        juce::ScopedNoDenormals noDenormals;
        const auto& params = Params::GetParams();
        inputGain.setGainDecibels(tree[juce::Identifier{ params.at(Params::Names::Gain_In) }]);
        auto block = juce::dsp::AudioBlock<float>(buffer);
        auto ctx = juce::dsp::ProcessContextReplacing<float>(block);

        inputGain.process(ctx);
    }

    void applyOutputGain(juce::AudioBuffer<float>& buffer) {

        const auto& params = Params::GetParams();
        outputGain.setGainDecibels(tree[juce::Identifier{ params.at(Params::Names::Gain_Out) }]);
        auto block = juce::dsp::AudioBlock<float>(buffer);
        auto ctx = juce::dsp::ProcessContextReplacing<float>(block);

        outputGain.process(ctx);
    }

    void applyLimiter(juce::AudioBuffer<float>& buffer) {

        const auto& params = Params::GetParams();
        limiter.setAttack(30);
        limiter.setRelease(tree[juce::Identifier{ params.at(Params::Names::Limiter_Release) }]);
        limiter.setThreshold(tree[juce::Identifier{ params.at(Params::Names::Limiter_Threshold) }]);
        limiter.setRatio(100);

        auto block = juce::dsp::AudioBlock<float>(buffer);
        auto ctx = juce::dsp::ProcessContextReplacing<float>(block);

        ctx.isBypassed = tree[juce::Identifier{ params.at(Params::Names::Bypass_Limiter) }];

        limiter.process(ctx);
    }

    float SilenceSlicer(juce::AudioBuffer<float>& buffer) {

        juce::ScopedNoDenormals noDenormals;

        return juce::Decibels::gainToDecibels(buffer.getRMSLevel(0, 0, buffer.getNumSamples()));
    }
private:
    juce::dsp::Gain<float> inputGain, outputGain;
    juce::dsp::NoiseGate<float> gate;
    juce::dsp::Compressor<float> comp, limiter;
    std::array<juce::dsp::Compressor<float>, 3> multi_compressors;
    std::array<juce::AudioBuffer<float>, 3> filterBuffers;
    juce::dsp::LinkwitzRileyFilter<float> LP1, AP2, HP1, LP2, HP2;
    juce::ValueTree tree;
};