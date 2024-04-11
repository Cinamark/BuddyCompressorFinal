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
        Bypass_Global,

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
    };

    inline const std::map<Names, juce::String>& GetParams() {
        static std::map<Names, juce::String> params =
        {
            {Low_Mid_Crossover_Freq, "Low-Mid Crossover Freq"},
            {Mid_High_Crossover_Freq, "Mid-High Crossover Freq"},
            {Threshold_Low_Band, "Threshold Low Band"},
            {Threshold_Mid_Band, "Threshold Mid Band"},
            {Threshold_High_Band, "Threshold High Band"},
            {Attack_Low_Band, "Attack Low Band"},
            {Attack_Mid_Band, "Attack Mid Band"},
            {Attack_High_Band, "Attack High Band"},
            {Ratio_Low_Band, "Ratio Low Band"},
            {Ratio_Mid_Band, "Ratio Mid Band"},
            {Ratio_High_Band, "Ratio High Band"},
            {Release_Low_Band, "Release Low Band"},
            {Release_Mid_Band, "Release Mid Band"},
            {Release_High_Band, "Release High Band"},
            {Bypass_Low, "Bypass Low"},
            {Bypass_Mid, "Bypass Mid"},
            {Bypass_High, "Bypass High"},
            {Solo_Low, "Solo Low"},
            {Solo_Mid, "Solo Mid"},
            {Solo_High, "Solo High"},
            {Mute_Low, "Mute Low"},
            {Mute_Mid, "Mute Mid"},
            {Mute_High, "Mute High"},
            {Gain_In, "Gain In"},
            {Bypass_Global, "Bypass Global"},
            {Threshold_AllPass, "Allpass Threshold"},
            {Ratio_AllPass, "Allpass Ratio"},
            {Attack_AllPass, "Allpass Attack"},
            {Release_AllPass, "Allpass Release"},
            {Gate_Thresh, "Gate Threshold"},
            {Gate_Ratio, "Gate Ratio"},
            {Gate_Attack, "Gate Attack"},
            {Gate_Release, "Gate Release"},
        };

        return params;
    }
}