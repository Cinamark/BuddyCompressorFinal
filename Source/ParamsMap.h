/*
  ==============================================================================

    ParamsMap.h
    Created: 31 Mar 2024 10:54:26am
    Author:  Simeon

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
namespace Params
{
    enum Names
    {
        Low_Mid_Crossover_Freq,
        Mid_High_Crossover_Freq,

        Threshold_Low_Band,
        Threshold_Mid_Band,
        Threshold_High_Band,
        Threshold_AllPass,

        Attack_Low_Band,
        Attack_Mid_Band,
        Attack_High_Band,
        Attack_AllPass,

        Release_Low_Band,
        Release_Mid_Band,
        Release_High_Band,
        Release_AllPass,

        Ratio_Low_Band,
        Ratio_Mid_Band,
        Ratio_High_Band,
        Ratio_AllPass,

        Bypass_Low,
        Bypass_Mid,
        Bypass_High,
        Bypass_Allpass,
        Bypass_Gate,
        Bypass_Limiter,

        Solo_Low,
        Solo_Mid,
        Solo_High,

        Mute_Low,
        Mute_Mid,
        Mute_High,

        Gain_In,
        Gain_Out,

        Gate_Thresh,
        Gate_Ratio,
        Gate_Attack,
        Gate_Release,

        Limiter_Threshold,
        Limiter_Release,
        SS_Toggle,

        Save_Samples
    };

    inline const std::map<Names, juce::String>& GetParams() {
        static std::map<Names, juce::String> params =
        {
            {Low_Mid_Crossover_Freq, "Low-Mid_Crossover_Freq"},
            {Mid_High_Crossover_Freq, "Mid-High_Crossover_Freq"},
            {Threshold_Low_Band, "Threshold_Low_Band"},
            {Threshold_Mid_Band, "Threshold_Mid_Band"},
            {Threshold_High_Band, "Threshold_High_Band"},
            {Attack_Low_Band, "Attack_Low_Band"},
            {Attack_Mid_Band, "Attack_Mid_Band"},
            {Attack_High_Band, "Attack_High_Band"},
            {Ratio_Low_Band, "Ratio_Low_Band"},
            {Ratio_Mid_Band, "Ratio_Mid_Band"},
            {Ratio_High_Band, "Ratio_High_Band"},
            {Release_Low_Band, "Release_Low_Band"},
            {Release_Mid_Band, "Release_Mid_Band"},
            {Release_High_Band, "Release_High_Band"},
            {Bypass_Low, "Bypass_Low"},
            {Bypass_Mid, "Bypass_Mid"},
            {Bypass_High, "Bypass_High"},
            {Solo_Low, "Solo_Low"},
            {Solo_Mid, "Solo_Mid"},
            {Solo_High, "Solo_High"},
            {Mute_Low, "Mute_Low"},
            {Mute_Mid, "Mute_Mid"},
            {Mute_High, "Mute_High"},
            {Gain_In, "Gain_In"},
            {Gain_Out, "Gain_Out"},
            {Bypass_Allpass, "Bypass_Allpass"},
            {Bypass_Gate, "Bypass_Gate"},
            {Bypass_Limiter, "Bypass_Limiter"},
            {Threshold_AllPass, "Allpass_Threshold"},
            {Ratio_AllPass, "Allpass_Ratio"},
            {Attack_AllPass, "Allpass_Attack"},
            {Release_AllPass, "Allpass_Release"},
            {Gate_Thresh, "Gate_Threshold"},
            {Gate_Ratio, "Gate_Ratio"},
            {Gate_Attack, "Gate_Attack"},
            {Gate_Release, "Gate_Release"},
            {Limiter_Threshold, "Limiter_Threshold"},
            {Limiter_Release, "Limiter_Release"},
            {SS_Toggle, "Silence_Slicer_Toggle"},
            {Save_Samples, "Save_Sample_Count"}
        };

        return params;
    }
}