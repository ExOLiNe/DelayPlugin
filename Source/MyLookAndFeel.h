/*
  ==============================================================================

    MyLookAndFeel.h
    Created: 1 Feb 2023 12:14:03pm
    Author:  Aleksandr

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class MyLookAndFeel : public juce::LookAndFeel_V4
{
private:
    juce::Font font;
public:
    MyLookAndFeel(juce::String faceName);
    juce::String getFaceName();
    void drawComboBox(juce::Graphics& g, int width, int height, bool,
        int, int, int, int, juce::ComboBox& box) override;
    
    juce::Font getComboBoxFont(juce::ComboBox& box) override;

    juce::Font getPopupMenuFont() override;

    //juce::Typeface::Ptr getTypefaceForFont(const juce::Font& font) override;
    void drawGroupComponentOutline(juce::Graphics& g, int width, int height,
        const juce::String& text, const juce::Justification& position,
        juce::GroupComponent& group) override;

    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
        const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& slider) override;
};