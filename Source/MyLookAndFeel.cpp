/*
  ==============================================================================

    MyLookAndFeel.cpp
    Created: 1 Feb 2023 12:14:03pm
    Author:  Aleksandr

  ==============================================================================
*/

#include "MyLookAndFeel.h"

MyLookAndFeel::MyLookAndFeel(juce::String faceName)
{
    using PopupMenu = juce::PopupMenu;
    using ComboBox = juce::ComboBox;
    using Colour = juce::Colour;
    using Label = juce::Label;
    using TextButton = juce::TextButton;

    setDefaultSansSerifTypefaceName(faceName);
    font.setTypefaceName(faceName);

    setColour(ComboBox::backgroundColourId, Colour::fromRGB(217, 217, 217));
    setColour(ComboBox::textColourId, Colour::fromRGB(57, 57, 57));
    setColour(ComboBox::outlineColourId, Colour::fromRGB(0, 0, 0));
    setColour(ComboBox::arrowColourId, Colour::fromRGB(57, 57, 57));
    setColour(PopupMenu::highlightedBackgroundColourId, Colour::fromRGB(195, 195, 195));
    setColour(PopupMenu::backgroundColourId, Colour::fromRGB(217, 217, 217));
    setColour(PopupMenu::textColourId, Colour::fromRGB(57, 57, 57));
    setColour(Label::textColourId, Colour::fromRGB(217, 217, 217));
    setColour(TextButton::textColourOffId, Colour::fromRGB(57, 57, 57));
    setColour(TextButton::textColourOnId, Colour::fromRGB(57, 57, 57));
    setColour(TextButton::buttonColourId, Colour::fromRGB(217, 217, 217));
    setColour(TextButton::buttonOnColourId, Colour::fromRGB(255, 239, 99));
}
void MyLookAndFeel::drawComboBox(juce::Graphics& g, int width, int height, bool,
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

juce::Font MyLookAndFeel::getComboBoxFont(juce::ComboBox& box)
{
    font.setHeight(juce::jmin(16.0f, (float)box.getHeight() * 0.85f));
    return font;
}

juce::Font MyLookAndFeel::getPopupMenuFont()
{
    font.setHeight(17.0f);
    return font;
}

void MyLookAndFeel::drawGroupComponentOutline(juce::Graphics& g, int width, int height,
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

juce::String MyLookAndFeel::getFaceName()
{
    return font.getTypefaceName();
}

void MyLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
    const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& slider)
{
    using namespace juce;
    auto svg = XmlDocument::parse(BinaryData::bigknob_svg);
    auto knob = Drawable::createFromSVG(*svg);
    float diameter = static_cast<float>(width > height ? height : width) * 0.75f;
    auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle) - MathConstants<float>::pi;
    auto factorX = diameter / knob->getWidth();
    auto factorY = diameter / knob->getHeight();
    AffineTransform transform;
    transform = transform
        .scaled(factorX, factorY)
        .translated(
            (float) width / 2 - diameter / 2,
            (float) height / 2 - diameter / 2)
        .rotated(angle, 
            width / 2, 
            height / 2);
    knob->draw(g, 1.f, transform);

    /*auto ledsQuantity = 18;
    auto centerX = (float)width / 2;
    auto centerY = (float)height / 2;
    auto ledsRadius = jmin(centerX, centerY) * 0.9f;
    auto eachLedAngleOffset = (rotaryEndAngle - rotaryStartAngle) / ledsQuantity * 0.98f;
    for (int i = 0; i <= ledsQuantity; ++i)
    {
        auto ledAngle = rotaryStartAngle - MathConstants<float>::pi + eachLedAngleOffset * i;
        auto nextLedAngle = ledAngle + eachLedAngleOffset;
        auto svgChars = (angle > ledAngle) 
            ? BinaryData::ledon_svg : BinaryData::ledoff_svg;
        auto ledSvg = XmlDocument::parse(svgChars);
        auto led = Drawable::createFromSVG(*ledSvg);
        AffineTransform ledTransform;
        led->draw(g, 1.f, ledTransform.translated(
            centerX - sin(ledAngle) * ledsRadius,
            centerY + cos(ledAngle) * ledsRadius
        ));
    }*/

    /*auto outline = slider.findColour(Slider::rotarySliderOutlineColourId);
    auto fill = slider.findColour(Slider::rotarySliderFillColourId);

    auto bounds = Rectangle<int>(x, y, width, height).toFloat().reduced(10);

    auto radius = jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f;
    auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    auto lineW = jmin(8.0f, radius * 0.5f);
    auto arcRadius = radius - lineW * 0.5f;

    Path backgroundArc;
    backgroundArc.addCentredArc(bounds.getCentreX(),
        bounds.getCentreY(),
        arcRadius,
        arcRadius,
        0.0f,
        rotaryStartAngle,
        rotaryEndAngle,
        true);

    g.setColour(outline);
    g.strokePath(backgroundArc, PathStrokeType(lineW, PathStrokeType::curved, PathStrokeType::rounded));

    if (slider.isEnabled())
    {
        Path valueArc;
        valueArc.addCentredArc(bounds.getCentreX(),
            bounds.getCentreY(),
            arcRadius,
            arcRadius,
            0.0f,
            rotaryStartAngle,
            toAngle,
            true);

        g.setColour(fill);
        g.strokePath(valueArc, PathStrokeType(lineW, PathStrokeType::curved, PathStrokeType::rounded));
    }

    auto thumbWidth = lineW * 2.0f;
    Point<float> thumbPoint(bounds.getCentreX() + arcRadius * std::cos(toAngle - MathConstants<float>::halfPi),
        bounds.getCentreY() + arcRadius * std::sin(toAngle - MathConstants<float>::halfPi));

    g.setColour(slider.findColour(Slider::thumbColourId));
    g.fillEllipse(Rectangle<float>(thumbWidth, thumbWidth).withCentre(thumbPoint));*/
}