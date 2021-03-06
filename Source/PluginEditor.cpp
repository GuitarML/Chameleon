/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <stdio.h>
#include <fstream>
#include <iostream>


//==============================================================================
ChameleonAudioProcessorEditor::ChameleonAudioProcessorEditor (ChameleonAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to

    // Set Widget Graphics
    ampSilverKnobLAF.setLookAndFeel(ImageCache::getFromMemory(BinaryData::knob_70_black_png, BinaryData::knob_70_black_pngSize));

    addAndMakeVisible(colorSelectButton);
    colorSelectButton.setImages(true, true, true,
        ImageCache::getFromMemory(BinaryData::power_switch_up_png, BinaryData::power_switch_up_pngSize), 1.0, Colours::transparentWhite,
        Image(), 1.0, Colours::transparentWhite,
        ImageCache::getFromMemory(BinaryData::power_switch_up_png, BinaryData::power_switch_up_pngSize), 1.0, Colours::transparentWhite,
        0.0);
    colorSelectButton.addListener(this);
    

    addAndMakeVisible(ampLED);
    ampLED.setImages(true, true, true,
        ImageCache::getFromMemory(BinaryData::led_red_on_png, BinaryData::led_red_on_pngSize), 1.0, Colours::transparentWhite,
        Image(), 1.0, Colours::transparentWhite,
        ImageCache::getFromMemory(BinaryData::led_red_on_png, BinaryData::led_red_on_pngSize), 1.0, Colours::transparentWhite,
        0.0);
    ampLED.addListener(this);

    bassSliderAttach = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.treeState, BASS_ID, ampBassKnob);    	    
    addAndMakeVisible(ampBassKnob);
    ampBassKnob.setLookAndFeel(&ampSilverKnobLAF);
    ampBassKnob.addListener(this);
    ampBassKnob.setRange(-8.0, 8.0);
    ampBassKnob.setValue(processor.ampBassKnobState);
    ampBassKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    ampBassKnob.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, false, 50, 20);
    ampBassKnob.setNumDecimalPlacesToDisplay(1);
    ampBassKnob.setDoubleClickReturnValue(true, 0.0);

    midSliderAttach = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.treeState, MID_ID, ampMidKnob);    
    addAndMakeVisible(ampMidKnob);
    ampMidKnob.setLookAndFeel(&ampSilverKnobLAF);
    ampMidKnob.addListener(this);
    ampMidKnob.setRange(-8.0, 8.0);
    ampMidKnob.setValue(processor.ampMidKnobState);
    ampMidKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    ampMidKnob.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, false, 50, 20);
    ampMidKnob.setNumDecimalPlacesToDisplay(1);
    ampMidKnob.setDoubleClickReturnValue(true, 0.0);

    trebleSliderAttach = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.treeState, TREBLE_ID, ampTrebleKnob);
    addAndMakeVisible(ampTrebleKnob);
    ampTrebleKnob.setLookAndFeel(&ampSilverKnobLAF);
    ampTrebleKnob.addListener(this);
    ampTrebleKnob.setRange(-8.0, 8.0);
    ampTrebleKnob.setValue(processor.ampTrebleKnobState);
    ampTrebleKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    ampTrebleKnob.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, false, 50, 20);
    ampTrebleKnob.setNumDecimalPlacesToDisplay(1);
    ampTrebleKnob.setDoubleClickReturnValue(true, 0.0);

    gainSliderAttach = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.treeState, GAIN_ID, ampGainKnob);
    addAndMakeVisible(ampGainKnob);
    ampGainKnob.setLookAndFeel(&ampSilverKnobLAF);
    ampGainKnob.addListener(this);
    ampGainKnob.setRange(-10.0, 10.0);
    ampGainKnob.setValue(processor.ampGainKnobState);
    ampGainKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    ampGainKnob.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, false, 50, 20);
    ampGainKnob.setNumDecimalPlacesToDisplay(1);
    ampGainKnob.setDoubleClickReturnValue(true, 0.0);
	
    presenceSliderAttach = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.treeState, PRESENCE_ID, ampPresenceKnob);
    addAndMakeVisible(ampPresenceKnob);
    ampPresenceKnob.setLookAndFeel(&ampSilverKnobLAF);
    ampPresenceKnob.addListener(this);
    ampPresenceKnob.setRange(-8.0, 8.0);
    ampPresenceKnob.setValue(processor.ampPresenceKnobState);
    ampPresenceKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    ampPresenceKnob.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, false, 50, 20 );
    ampPresenceKnob.setNumDecimalPlacesToDisplay(1);
    ampPresenceKnob.setDoubleClickReturnValue(true, 0.0);

    masterSliderAttach = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.treeState, MASTER_ID, ampMasterKnob);
    addAndMakeVisible(ampMasterKnob);
    ampMasterKnob.setLookAndFeel(&ampSilverKnobLAF);
    ampMasterKnob.addListener(this);
    ampMasterKnob.setRange(-36.0, 0.0);
    ampMasterKnob.setValue(processor.ampMasterKnobState);
    ampMasterKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    ampMasterKnob.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, false, 50, 20 );
    ampMasterKnob.setNumDecimalPlacesToDisplay(1);
    ampMasterKnob.setDoubleClickReturnValue(true, -18.0);

    // Size of plugin GUI
    setSize(774, 293);

    resetImages(); // Resets the Toggle Switch and LED image based on current settings

}

ChameleonAudioProcessorEditor::~ChameleonAudioProcessorEditor()
{
    ampBassKnob.setLookAndFeel(nullptr);
    ampMidKnob.setLookAndFeel(nullptr);
    ampTrebleKnob.setLookAndFeel(nullptr);
    ampGainKnob.setLookAndFeel(nullptr);
    ampPresenceKnob.setLookAndFeel(nullptr);
    ampMasterKnob.setLookAndFeel(nullptr);
}

//==============================================================================
void ChameleonAudioProcessorEditor::paint (Graphics& g)
{
    // Workaround for graphics on Windows builds (clipping code doesn't work correctly on Windows)
    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    g.drawImageAt(background, 0, 0);  // Debug Line: Redraw entire background image
    #else
    // Redraw only the clipped part of the background image
    juce::Rectangle<int> ClipRect = g.getClipBounds(); 
    g.drawImage(background, ClipRect.getX(), ClipRect.getY(), ClipRect.getWidth(), ClipRect.getHeight(), ClipRect.getX(), ClipRect.getY(), ClipRect.getWidth(), ClipRect.getHeight());
    #endif
}

void ChameleonAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
	
    // Amp Widgets
    ampBassKnob.setBounds(188, 40, 50, 70);
    ampMidKnob.setBounds(249, 40, 50, 70);
    ampTrebleKnob.setBounds(308, 40, 50, 70);
    ampGainKnob.setBounds(120, 40, 50, 70);
    ampMasterKnob.setBounds(455, 40, 50, 70);
    ampPresenceKnob.setBounds(380, 40, 50, 70);

    colorSelectButton.setBounds(58, 41, 70, 70);
    ampLED.setBounds(694, 89, 34, 34);
}


void ChameleonAudioProcessorEditor::buttonClicked(juce::Button* button)
{
    if (button == &colorSelectButton) {
        colorSelectClicked();
    }
}


void ChameleonAudioProcessorEditor::colorSelectClicked() {
    if (processor.current_model_index == 0) {
        processor.current_model_index = 1;
        processor.fromUpDown = 0;
        processor.loadConfig(processor.gold_tone);
    }
    else if (processor.current_model_index == 1) {
        if (processor.fromUpDown == 0) {
            processor.current_model_index = 2;
            processor.loadConfig(processor.green_tone);
        } else {
            processor.current_model_index = 0;
            processor.loadConfig(processor.red_tone);
        }
    }
    else if (processor.current_model_index == 2) {
        processor.current_model_index = 1;
        processor.fromUpDown = 1;
        processor.loadConfig(processor.gold_tone);
    }
    resetImages(); // Resets the Toggle Switch and LED image based on current settings
    repaint();
}


void ChameleonAudioProcessorEditor::sliderValueChanged(Slider* slider)
{
    // Amp
    if (slider == &ampGainKnob)
        processor.set_ampDrive(slider->getValue());
    else if (slider == &ampMasterKnob)
        processor.set_ampMaster(slider->getValue());
    else if (slider == &ampBassKnob || slider == &ampMidKnob || slider == &ampTrebleKnob) {
        processor.set_ampEQ(ampBassKnob.getValue(), ampMidKnob.getValue(), ampTrebleKnob.getValue(), ampPresenceKnob.getValue());
        // Set knob states for saving positions when closing/reopening GUI
        processor.ampBassKnobState = ampBassKnob.getValue();
        processor.ampMidKnobState = ampMidKnob.getValue();
        processor.ampTrebleKnobState = ampTrebleKnob.getValue();
    }
    else if (slider == &ampPresenceKnob) {
        processor.set_ampEQ(ampBassKnob.getValue(), ampMidKnob.getValue(), ampTrebleKnob.getValue(), ampPresenceKnob.getValue());
    }
}

void ChameleonAudioProcessorEditor::resetImages()
{
    if (processor.current_model_index == 0) {
        colorSelectButton.setImages(true, true, true,
            ImageCache::getFromMemory(BinaryData::power_switch_up_png, BinaryData::power_switch_up_pngSize), 1.0, Colours::transparentWhite,
            Image(), 1.0, Colours::transparentWhite,
            ImageCache::getFromMemory(BinaryData::power_switch_up_png, BinaryData::power_switch_up_pngSize), 1.0, Colours::transparentWhite,
            0.0);
        ampLED.setImages(true, true, true,
            ImageCache::getFromMemory(BinaryData::led_red_on_png, BinaryData::led_red_on_pngSize), 1.0, Colours::transparentWhite,
            Image(), 1.0, Colours::transparentWhite,
            ImageCache::getFromMemory(BinaryData::led_red_on_png, BinaryData::led_red_on_pngSize), 1.0, Colours::transparentWhite,
            0.0);
    }
    else if (processor.current_model_index == 1) {
        colorSelectButton.setImages(true, true, true,
            ImageCache::getFromMemory(BinaryData::power_switch_mid_png, BinaryData::power_switch_mid_pngSize), 1.0, Colours::transparentWhite,
            Image(), 1.0, Colours::transparentWhite,
            ImageCache::getFromMemory(BinaryData::power_switch_mid_png, BinaryData::power_switch_mid_pngSize), 1.0, Colours::transparentWhite,
            0.0);
        ampLED.setImages(true, true, true,
            ImageCache::getFromMemory(BinaryData::led_gold_on_png, BinaryData::led_gold_on_pngSize), 1.0, Colours::transparentWhite,
            Image(), 1.0, Colours::transparentWhite,
            ImageCache::getFromMemory(BinaryData::led_gold_on_png, BinaryData::led_gold_on_pngSize), 1.0, Colours::transparentWhite,
            0.0);
    }
    else {
        colorSelectButton.setImages(true, true, true,
            ImageCache::getFromMemory(BinaryData::power_switch_down_png, BinaryData::power_switch_down_pngSize), 1.0, Colours::transparentWhite,
            Image(), 1.0, Colours::transparentWhite,
            ImageCache::getFromMemory(BinaryData::power_switch_down_png, BinaryData::power_switch_down_pngSize), 1.0, Colours::transparentWhite,
            0.0);
        ampLED.setImages(true, true, true,
            ImageCache::getFromMemory(BinaryData::led_green_on_png, BinaryData::led_green_on_pngSize), 1.0, Colours::transparentWhite,
            Image(), 1.0, Colours::transparentWhite,
            ImageCache::getFromMemory(BinaryData::led_green_on_png, BinaryData::led_green_on_pngSize), 1.0, Colours::transparentWhite,
            0.0);
    }
}
