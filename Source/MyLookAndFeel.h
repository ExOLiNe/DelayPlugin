/*
  ==============================================================================

    MyLookAndFeel.h
    Created: 1 Feb 2023 12:14:03pm
    Author:  Aleksandr

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class MyBoxLookAndFeel : public juce::LookAndFeel_V4
{
private:
    juce::Font font;
public:
    MyBoxLookAndFeel(juce::String faceName);
    void drawComboBox(juce::Graphics& g, int width, int height, bool,
        int, int, int, int, juce::ComboBox& box) override;
    
    juce::Font getComboBoxFont(juce::ComboBox& box) override;

    juce::Font getPopupMenuFont() override;

    //juce::Typeface::Ptr getTypefaceForFont(const juce::Font& font) override;
    void drawGroupComponentOutline(juce::Graphics& g, int width, int height,
        const juce::String& text, const juce::Justification& position,
        juce::GroupComponent& group) override;
};