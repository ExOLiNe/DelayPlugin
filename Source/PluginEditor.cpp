/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
//==============================================================================
AudioFifoTestAudioProcessorEditor::AudioFifoTestAudioProcessorEditor (AudioFifoTestAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), myLookAndFeel("FuturisExtra Bold Cyrillic@"),
    feedbackKnob("FEEDBACK", "%", 0, 100),
    mixKnob("MIX", "%", 0, 100),
    hiPassKnob("HI", "%", 0, 100),
    lowPassKnob("LOW", "%", 0, 100),
    delayKnob("DELAY", "ms", 5, 1000)
{
    customizeLookAndFeel(myLookAndFeel);
    LookAndFeel_V4::setDefaultLookAndFeel(&myLookAndFeel);

    background = juce::Drawable::createFromSVG(*juce::XmlDocument::parse(BinaryData::background_svg));

    bpmMappings.push_back(32);
    bpmMappings.push_back(24);
    bpmMappings.push_back(16);
    bpmMappings.push_back(12);
    bpmMappings.push_back(8);
    bpmMappings.push_back(6);
    bpmMappings.push_back(4);
    bpmMappings.push_back(3);
    bpmMappings.push_back(2);
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (675, 515);

    /*dryWetSliderValue = make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, DRY_WET_ID, dryWetSlider);
    dryWetSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    dryWetSlider.setTextValueSuffix(" %");
    dryWetSlider.setRange(0, 100, 1);
    dryWetSlider.addListener(this);
    //dryWetSlider.setValue(audioProcessor.getDryWet());
    addAndMakeVisible(dryWetSlider);

    delaySlider.setSliderStyle(Slider::SliderStyle::Rotary);
    delaySlider.setTextValueSuffix(" ms");
    delaySlider.setRange(50, 1200, 1);
    delaySlider.addListener(this);
    delaySlider.setValue(audioProcessor.getDelayMs());
    addAndMakeVisible(delaySlider);

    feedbackSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    feedbackSlider.setTextValueSuffix(" times");
    feedbackSlider.setRange(0.4, 1, 0.01f);
    feedbackSlider.setValue(audioProcessor.getFeedback());
    feedbackSlider.addListener(this);
    addAndMakeVisible(feedbackSlider);

    modeBox.addItem("stereo", 1);
    modeBox.addItem("mono", 2);
    modeBox.addItem("ping pong", 3);
    modeBox.setSize(100, 20);
    modeBox.addListener(this);
    addAndMakeVisible(modeBox);

    lowpassSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    lowpassSlider.setTextValueSuffix(" hz");
    lowpassSlider.setRange(2000.0f, 20000.0f, 1.0f);
    lowpassSlider.setValue(audioProcessor.getLowpassFrequency());
    lowpassSlider.addListener(this);
    lowpassSlider.setSkewFactor(0.4f);
    addAndMakeVisible(lowpassSlider);

    highpassSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    highpassSlider.setTextValueSuffix(" hz");
    highpassSlider.setRange(20.0f, 2000.0f, 1.0f);
    highpassSlider.setValue(audioProcessor.getHighpassFrequency());
    highpassSlider.addListener(this);
    addAndMakeVisible(highpassSlider);

    tiedToBpmButton.addListener(this);
    addAndMakeVisible(tiedToBpmButton);*/
    titleLabel.setText("DELAY NAME", juce::NotificationType::dontSendNotification);
    titleLabel.setJustificationType(juce::Justification::topLeft);
    titleLabel.setFont(juce::Font(48));
    addAndMakeVisible(titleLabel);

    versionLabel.setText("ver.666", juce::NotificationType::dontSendNotification);
    versionLabel.setJustificationType(juce::Justification::topRight);
    versionLabel.setFont(juce::Font(20));
    addAndMakeVisible(versionLabel);

    siteLabel.setText("https://shiteater.com", juce::NotificationType::dontSendNotification);
    siteLabel.setJustificationType(juce::Justification::topRight);
    siteLabel.setFont(juce::Font(20));
    addAndMakeVisible(siteLabel);

    presetsLabel.setText("PRESETS:", juce::NotificationType::dontSendNotification);
    presetsLabel.setJustificationType(juce::Justification::topLeft);
    presetsLabel.setFont(juce::Font(20));
    //presetsLabel.setLookAndFeel(myLookAndFeel);
    addAndMakeVisible(presetsLabel);

    //presets.setLookAndFeel(myLookAndFeel);
    presets.addItem("GAY DELAY", 1);
    presets.addItem("NATURAL DELAY", 2);
    //presets.onChange = [this] {  };
    presets.setSelectedId(1);
    addAndMakeVisible(presets);

    analogButton.setButtonText("ANALOG");
    analogButton.setToggleable(true);
    analogButton.setClickingTogglesState(true);
    addAndMakeVisible(analogButton);

    delayModeLabel.setText("DELAY MODE:", juce::NotificationType::dontSendNotification);
    delayModeLabel.setJustificationType(juce::Justification::centred);
    delayModeLabel.setFont(juce::Font(20));
    addAndMakeVisible(delayModeLabel);

    addAndMakeVisible(feedbackKnob);
    addAndMakeVisible(mixKnob);
    addAndMakeVisible(hiPassKnob);
    addAndMakeVisible(lowPassKnob);
    addAndMakeVisible(delayKnob);

    //auto image = juce::ImageFileFormat::loadFrom(pug::pug_png, pug::pug_pngSize);
    //logo.setImage(image);
    //addAndMakeVisible(logo);

    stereoModeButton.setButtonText("STEREO");
    stereoModeButton.setToggleable(true);
    stereoModeButton.setClickingTogglesState(true);
    stereoModeButton.addListener(this);
    stereoModeButton.setComponentID(settings::stereoModeID);
    stereoModeButton.setToggleState(true, juce::NotificationType::dontSendNotification);
    addAndMakeVisible(stereoModeButton);

    monoModeButton.setButtonText("MONO");
    monoModeButton.setToggleable(true);
    monoModeButton.setClickingTogglesState(true);
    monoModeButton.addListener(this);
    monoModeButton.setComponentID(settings::monoModeID);
    addAndMakeVisible(monoModeButton);

    pingPongModeButton.setButtonText("PING PONG");
    pingPongModeButton.setToggleable(true);
    pingPongModeButton.setClickingTogglesState(true);
    pingPongModeButton.addListener(this);
    pingPongModeButton.setComponentID(settings::pingPongModeID);
    addAndMakeVisible(pingPongModeButton);
}

void AudioFifoTestAudioProcessorEditor::customizeLookAndFeel(LookAndFeel& lookAndFeel)
{
    lookAndFeel.setColour(presets.backgroundColourId, Colour::fromRGB(217, 217, 217));
    lookAndFeel.setColour(presets.textColourId, Colour::fromRGB(57, 57, 57));
    lookAndFeel.setColour(presets.outlineColourId, Colour::fromRGB(0, 0, 0));
    lookAndFeel.setColour(presets.arrowColourId, Colour::fromRGB(57, 57, 57));
    lookAndFeel.setColour(PopupMenu::highlightedBackgroundColourId, Colour::fromRGB(195, 195, 195));
    lookAndFeel.setColour(PopupMenu::backgroundColourId, Colour::fromRGB(217, 217, 217));
    lookAndFeel.setColour(PopupMenu::textColourId, Colour::fromRGB(57, 57, 57));
    lookAndFeel.setColour(Label::textColourId, Colour::fromRGB(217, 217, 217));
    lookAndFeel.setColour(juce::TextButton::textColourOffId, Colour::fromRGB(57, 57, 57));
    lookAndFeel.setColour(juce::TextButton::textColourOnId, Colour::fromRGB(57, 57, 57));
    lookAndFeel.setColour(juce::TextButton::buttonColourId, Colour::fromRGB(217, 217, 217));
    lookAndFeel.setColour(juce::TextButton::buttonOnColourId, Colour::fromRGB(255, 239, 99));
}

AudioFifoTestAudioProcessorEditor::~AudioFifoTestAudioProcessorEditor()
{
    /*presetsLabel.setLookAndFeel(nullptr);
    presets.setLookAndFeel(nullptr);
    setLookAndFeel(nullptr);*/
}

void AudioFifoTestAudioProcessorEditor::comboBoxChanged(ComboBox* comboBoxThatHasChanged)
{
    switch (comboBoxThatHasChanged->getSelectedId())
    {
    case 1:
        audioProcessor.setMode(Mode::STEREO);
        break;
    case 2:
        audioProcessor.setMode(Mode::MONO);
        break;
    case 3:
        audioProcessor.setMode(Mode::PING_PONG);
        break;
    }
}

void AudioFifoTestAudioProcessorEditor::sliderValueChanged(Slider* slider)
{
    /*if (slider == &delaySlider)
    {
        if (delayTimeMode == DelayTimeMode::BPM)
        {
            audioProcessor.setDuration(bpmMappings[slider->getValue()]);
        }
        else
        {
            audioProcessor.setDelayMs(slider->getValue());
        }
    }
    else if (slider == &feedbackSlider)
    {
        audioProcessor.setFeedback(slider->getValue());
    }
    else if (slider == &dryWetSlider)
    {
        //audioProcessor.setDryWet(slider->getValue()) ;
    }
    else if (slider == &lowpassSlider)
    {
        audioProcessor.setLowpassFrequency(slider->getValue());
    }
    else if (slider == &highpassSlider)
    {
        audioProcessor.setHighpassFrequency(slider->getValue());
    }*/
}

void AudioFifoTestAudioProcessorEditor::setDelayTimeMode(DelayTimeMode mode)
{
    this->delayTimeMode = mode;
}

void AudioFifoTestAudioProcessorEditor::buttonStateChanged(Button* button)
{
    
    /*auto state = button->getToggleState();
    if (state)
    {
        delaySlider.setTextValueSuffix("");
        delaySlider.setRange(0, bpmMappings.size() - 1, 1.0f);
        delayTimeMode = DelayTimeMode::BPM;
        audioProcessor.setBpmTied(state);
    }
    else
    {
        delaySlider.setTextValueSuffix(" ms");
        delaySlider.setRange(50, 1200, 1);
        delayTimeMode = DelayTimeMode::MS;
    }*/
}

void AudioFifoTestAudioProcessorEditor::buttonClicked(Button* button)
{
    if (!button->getToggleState())
    {
        button->setToggleState(true, juce::NotificationType::dontSendNotification);
    }

    auto id = button->getComponentID();
    if (id == settings::stereoModeID) 
    {
        monoModeButton.setToggleState(false, juce::NotificationType::dontSendNotification);
        pingPongModeButton.setToggleState(false, juce::NotificationType::dontSendNotification);
        audioProcessor.setMode(Mode::STEREO);
    }
    else if (id == settings::monoModeID) 
    {
        stereoModeButton.setToggleState(false, juce::NotificationType::dontSendNotification);
        pingPongModeButton.setToggleState(false, juce::NotificationType::dontSendNotification);
        audioProcessor.setMode(Mode::MONO);
    }
    else if (id == settings::pingPongModeID)
    {
        stereoModeButton.setToggleState(false, juce::NotificationType::dontSendNotification);
        monoModeButton.setToggleState(false, juce::NotificationType::dontSendNotification);
        audioProcessor.setMode(Mode::PING_PONG);
    }
}

//==============================================================================
void AudioFifoTestAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    //g.fillAll (Colour::fromRGB(42, 113, 108));

    juce::AffineTransform transform;

    auto bounds = getLocalBounds();

    background->draw(g, 1.f, transform.scaled(
        static_cast<double>(bounds.getWidth()) / background->getWidth(), static_cast<double>(bounds.getHeight()) / background->getHeight()
    ));

    drawGroup(g, {
        (float)border,
        (float)presets.getY() + border * 5,
        bounds.getWidth() / 3.5f,
        320
        }, GroupStyle::SOLID);
    auto rightGroupWidth = bounds.getWidth() / 3.5f;
    drawGroup(g, {
        (float)bounds.getWidth() - rightGroupWidth - border,
        (float)presets.getY() + border * 5,
        rightGroupWidth,
        320
        }, GroupStyle::BORDERED);
}

void AudioFifoTestAudioProcessorEditor::resized()
{
    /*dryWetSlider.setBounds(
        getLocalBounds().removeFromBottom(getHeight() / 2).removeFromLeft(getWidth() / 3));
    delaySlider.setBounds(
        getLocalBounds().removeFromTop(getHeight() / 2).removeFromLeft(getWidth() / 3));
    feedbackSlider.setBounds(
        getLocalBounds().removeFromTop(getHeight() / 2).removeFromRight(getWidth() / 3));
    modeBox.setBounds(
        getLocalBounds().removeFromBottom(getHeight() / 2).removeFromRight(getWidth() / 3));
    lowpassSlider.setBounds(
        getLocalBounds().removeFromBottom(getHeight() / 2).removeFromLeft(getWidth() / 3 * 2).removeFromRight(getWidth() / 7 * 2));
    highpassSlider.setBounds(
        getLocalBounds().removeFromTop(getHeight() / 2).removeFromLeft(getWidth() / 3 * 2).removeFromRight(getWidth() / 5 * 2));
    tiedToBpmButton.setBounds(
        getLocalBounds().removeFromBottom(getHeight() / 3).removeFromRight(getWidth() / 3));*/
    auto bounds = getLocalBounds();

    titleLabel.setBounds(border, border, bounds.getWidth() / 2, 48);

    versionLabel.setBounds(
        bounds.getWidth() / 2,
        border,
        bounds.getWidth() / 2,
        20
    );

    siteLabel.setBounds(
        bounds.getWidth() / 2,
        border + versionLabel.getHeight(),
        bounds.getWidth() / 2,
        20
    );

    presetsLabel.setBounds(
        border,
        border * 3 + titleLabel.getHeight() + 3,
        bounds.getWidth() / 5,
        25
    );

    presets.setBounds(
        presetsLabel.getWidth(),
        border * 3 + titleLabel.getHeight(),
        bounds.getWidth() / 3,
        25
    );

    analogButton.setBounds(
        bounds.getWidth() - 160,
        border * 3 + titleLabel.getHeight(),
        140,
        25
    );

    delayKnob.setBounds(
        188,
        142,
        290,
        290
    );


    feedbackKnob.setBounds(
        42,
        150,
        120,
        120
        );
    mixKnob.setBounds(
        42,
        310,
        120,
        120
    );
    hiPassKnob.setBounds(
        510,
        150,
        120,
        120
    );

    lowPassKnob.setBounds(
        510,
        310,
        120,
        120
    );

    delayModeLabel.setBounds(
        53,
        106,
        127,
        757
    );

    stereoModeButton.setBounds(
        213,
        469,
        125,
        30
    );

    monoModeButton.setBounds(
        360,
        469,
        125,
        30
    );

    pingPongModeButton.setBounds(
        506,
        469,
        125,
        30
    );
    /*drawGroup(g, {
        (float)border,
        (float)presets.getY() + border * 5,
        bounds.getWidth() / 3.5f,
        bounds.getHeight() / 1.5f
        }, GroupStyle::SOLID);*/

    /*logo.setBounds(
        JUCE_LIVE_CONSTANT(0),
        JUCE_LIVE_CONSTANT(0),
        JUCE_LIVE_CONSTANT(200),
        JUCE_LIVE_CONSTANT(200)
    );*/
}

void AudioFifoTestAudioProcessorEditor::drawGroup(juce::Graphics& g, juce::Rectangle<float> rect, GroupStyle style)
{
    juce::Path border;
    border.addRoundedRectangle(rect, 20.0f);
    g.setColour(juce::Colour::fromRGBA(217, 217, 217, 130));
    g.fillPath(border);
}