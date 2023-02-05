/*
  ==============================================================================

    MyLookAndFeel.cpp
    Created: 1 Feb 2023 12:14:03pm
    Author:  Aleksandr

  ==============================================================================
*/

#include "MyLookAndFeel.h"

MyBoxLookAndFeel::MyBoxLookAndFeel(juce::String faceName)
{
    setDefaultSansSerifTypefaceName(faceName);
    font.setTypefaceName(faceName);
}
void MyBoxLookAndFeel::drawComboBox(juce::Graphics& g, int width, int height, bool,
    int, int, int, int, juce::ComboBox& box)
{
    auto cornerSize = 15.0f;
    juce::Rectangle<int> boxBounds(0, 0, width, height);

    g.setColour(box.findColour(juce::ComboBox::backgroundColourId));
    g.fillRoundedRectangle(boxBounds.toFloat(), cornerSize);

    g.setColour(box.findColour(juce::ComboBox::outlineColourId));
    g.drawRoundedRectangle(boxBounds.toFloat().reduced(0.5f, 0.5f), cornerSize, 0.0f);

    juce::Rectangle<int> arrowZone(width - 30, 0, 20, height);
    juce::Path path;

    path.addTriangle(
        { (float)arrowZone.getX() + 3.0f , (float)arrowZone.getCentreY() - 2.0f },
        { (float)arrowZone.getCentreX(), (float)arrowZone.getCentreY() + 3.0f },
        { (float)arrowZone.getRight() - 3.0f, (float)arrowZone.getCentreY() - 2.0f }
    );

    g.setColour(box.findColour(juce::ComboBox::arrowColourId).withAlpha((box.isEnabled() ? 0.9f : 0.2f)));
    g.fillPath(path);
}

juce::Font MyBoxLookAndFeel::getComboBoxFont(juce::ComboBox& box)
{
    font.setHeight(juce::jmin(16.0f, (float)box.getHeight() * 0.85f));
    return font;
}

juce::Font MyBoxLookAndFeel::getPopupMenuFont()
{
    font.setHeight(17.0f);
    return font;
}

void MyBoxLookAndFeel::drawGroupComponentOutline(juce::Graphics& g, int width, int height,
    const juce::String& text, const juce::Justification& position,
    juce::GroupComponent& group)
{

    using namespace juce;
    const float textH = 0.0f;
    const float indent = 0.0f;
    auto cs = 6.0f;

    Font f(textH);

    Path p;
    auto x = indent;
    auto y = f.getAscent() - 3.0f;
    auto w = jmax(0.0f, (float)width - x * 2.0f);
    auto h = jmax(0.0f, (float)height - y - indent);

    juce::Path border;
    border.addRoundedRectangle(juce::Rectangle<float>{
        x, y, w, h
    }, 20.0f);
    g.setColour(juce::Colour::fromRGBA(217, 217, 217, 130));
    g.fillPath(border);
}