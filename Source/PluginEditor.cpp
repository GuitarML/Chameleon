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
SmartAmpProAudioProcessorEditor::SmartAmpProAudioProcessorEditor (SmartAmpProAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to

    // Set Widget Graphics
    ampSilverKnobLAF.setLookAndFeel(ImageCache::getFromMemory(BinaryData::Vintage_Knob_png, BinaryData::Vintage_Knob_pngSize));
    statusKnob.setLookAndFeel(ImageCache::getFromMemory(BinaryData::status_knob_png, BinaryData::status_knob_pngSize));

    ampOnButton.setImages(true, true, true,
        ImageCache::getFromMemory(BinaryData::Power_switch_on_png, BinaryData::Power_switch_on_pngSize), 1.0, Colours::transparentWhite,
        Image(), 1.0, Colours::transparentWhite,
        ImageCache::getFromMemory(BinaryData::Power_switch_on_png, BinaryData::Power_switch_on_pngSize), 1.0, Colours::transparentWhite,
        0.0);
    addAndMakeVisible(ampOnButton);
    ampOnButton.addListener(this);


    addAndMakeVisible(ampLED);
    ampLED.setImages(true, true, true,
        ImageCache::getFromMemory(BinaryData::led_blue_on_png, BinaryData::led_blue_on_pngSize), 1.0, Colours::transparentWhite,
        Image(), 1.0, Colours::transparentWhite,
        ImageCache::getFromMemory(BinaryData::led_blue_on_png, BinaryData::led_blue_on_pngSize), 1.0, Colours::transparentWhite,
        0.0);
    ampLED.addListener(this);


    addAndMakeVisible(ampBassKnob);
    ampBassKnob.setLookAndFeel(&ampSilverKnobLAF);
    ampBassKnob.addListener(this);
    ampBassKnob.setRange(-8.0, 8.0);
    ampBassKnob.setValue(processor.ampBassKnobState);
    ampBassKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    ampBassKnob.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, false, 50, 20);
    ampBassKnob.setNumDecimalPlacesToDisplay(1);
    ampBassKnob.setDoubleClickReturnValue(true, 0.0);

    addAndMakeVisible(ampMidKnob);
    ampMidKnob.setLookAndFeel(&ampSilverKnobLAF);
    ampMidKnob.addListener(this);
    ampMidKnob.setRange(-8.0, 8.0);
    ampMidKnob.setValue(processor.ampMidKnobState);
    ampMidKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    ampMidKnob.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, false, 50, 20);
    ampMidKnob.setNumDecimalPlacesToDisplay(1);
    ampMidKnob.setDoubleClickReturnValue(true, 0.0);

    addAndMakeVisible(ampTrebleKnob);
    ampTrebleKnob.setLookAndFeel(&ampSilverKnobLAF);
    ampTrebleKnob.addListener(this);
    ampTrebleKnob.setRange(-8.0, 8.0);
    ampTrebleKnob.setValue(processor.ampTrebleKnobState);
    ampTrebleKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    ampTrebleKnob.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, false, 50, 20);
    ampTrebleKnob.setNumDecimalPlacesToDisplay(1);
    ampTrebleKnob.setDoubleClickReturnValue(true, 0.0);

    addAndMakeVisible(ampGainKnob);
    ampGainKnob.setLookAndFeel(&ampSilverKnobLAF);
    ampGainKnob.addListener(this);
    ampGainKnob.setRange(-12.0, 12.0);
    ampGainKnob.setValue(processor.ampGainKnobState);
    ampGainKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    ampGainKnob.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, false, 50, 20);
    ampGainKnob.setNumDecimalPlacesToDisplay(1);
    ampGainKnob.setDoubleClickReturnValue(true, 0.0);

    addAndMakeVisible(ampMasterKnob);
    ampMasterKnob.setLookAndFeel(&ampSilverKnobLAF);
    ampMasterKnob.addListener(this);
    ampMasterKnob.setRange(-36.0, 12.0);
    ampMasterKnob.setValue(processor.ampMasterKnobState);
    ampMasterKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    ampMasterKnob.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, false, 50, 20 );
    ampMasterKnob.setNumDecimalPlacesToDisplay(1);
    ampMasterKnob.setDoubleClickReturnValue(true, -12.0);

    // Size of plugin GUI
    setSize(694, 376);

}

SmartAmpProAudioProcessorEditor::~SmartAmpProAudioProcessorEditor()
{
}

//==============================================================================
void SmartAmpProAudioProcessorEditor::paint (Graphics& g)
{
    if (processor.skin == 0) {
        if (current_background == 1 && processor.amp_state == 1) {
            background = ImageCache::getFromMemory(BinaryData::smp_on_png, BinaryData::smp_on_pngSize);
        }
        else if (current_background == 1 && processor.amp_state == 1) {
            background = ImageCache::getFromMemory(BinaryData::smp_on_png, BinaryData::smp_on_pngSize);
        }
        else {
            background = ImageCache::getFromMemory(BinaryData::smp_off_png, BinaryData::smp_off_pngSize);
        }
    } else if (processor.skin == 1) {
        if (current_background == 1 && processor.amp_state == 1) {
            background = ImageCache::getFromMemory(BinaryData::original_on_png, BinaryData::original_on_pngSize);
        }
        else if (current_background == 1 && processor.amp_state == 1) {
            background = ImageCache::getFromMemory(BinaryData::original_on_png, BinaryData::original_on_pngSize);
        }
        else {
            background = ImageCache::getFromMemory(BinaryData::original_off_png, BinaryData::original_off_pngSize);
        }
    }
    g.drawImageAt(background, 0, 0);

    g.setColour (Colours::white);
    g.setFont (15.0f);

    // Set On/Off amp graphic
    if (processor.skin == 0) {
        if (processor.amp_state == 0) {
            ampOnButton.setImages(true, true, true,
                ImageCache::getFromMemory(BinaryData::Power_switch_off_png, BinaryData::Power_switch_off_pngSize), 1.0, Colours::transparentWhite,
                Image(), 1.0, Colours::transparentWhite,
                ImageCache::getFromMemory(BinaryData::Power_switch_off_png, BinaryData::Power_switch_off_pngSize), 1.0, Colours::transparentWhite,
                0.0);
            ampLED.setImages(true, true, true,
                ImageCache::getFromMemory(BinaryData::led_blue_off_png, BinaryData::led_blue_off_pngSize), 1.0, Colours::transparentWhite,
                Image(), 1.0, Colours::transparentWhite,
                ImageCache::getFromMemory(BinaryData::led_blue_off_png, BinaryData::led_blue_off_pngSize), 1.0, Colours::transparentWhite,
                0.0);
        }
        else {
            ampOnButton.setImages(true, true, true,
                ImageCache::getFromMemory(BinaryData::Power_switch_on_png, BinaryData::Power_switch_on_pngSize), 1.0, Colours::transparentWhite,
                Image(), 1.0, Colours::transparentWhite,
                ImageCache::getFromMemory(BinaryData::Power_switch_on_png, BinaryData::Power_switch_on_pngSize), 1.0, Colours::transparentWhite,
                0.0);
            ampLED.setImages(true, true, true,
                ImageCache::getFromMemory(BinaryData::led_blue_on_png, BinaryData::led_blue_on_pngSize), 1.0, Colours::transparentWhite,
                Image(), 1.0, Colours::transparentWhite,
                ImageCache::getFromMemory(BinaryData::led_blue_on_png, BinaryData::led_blue_on_pngSize), 1.0, Colours::transparentWhite,
                0.0);
        }
    } else if (processor.skin == 1) {
        if (processor.amp_state == 0) {
            ampOnButton.setImages(true, true, true,
                ImageCache::getFromMemory(BinaryData::power_switch_up_png, BinaryData::power_switch_up_pngSize), 1.0, Colours::transparentWhite,
                Image(), 1.0, Colours::transparentWhite,
                ImageCache::getFromMemory(BinaryData::power_switch_down_png, BinaryData::power_switch_down_pngSize), 1.0, Colours::transparentWhite,
                0.0);
            ampLED.setImages(true, true, true,
                ImageCache::getFromMemory(BinaryData::led_blue_off_png, BinaryData::led_blue_off_pngSize), 1.0, Colours::transparentWhite,
                Image(), 1.0, Colours::transparentWhite,
                ImageCache::getFromMemory(BinaryData::led_blue_off_png, BinaryData::led_blue_off_pngSize), 1.0, Colours::transparentWhite,
                0.0);
        }
        else {
            ampOnButton.setImages(true, true, true,
                ImageCache::getFromMemory(BinaryData::power_switch_down_png, BinaryData::Power_switch_on_pngSize), 1.0, Colours::transparentWhite,
                Image(), 1.0, Colours::transparentWhite,
                ImageCache::getFromMemory(BinaryData::power_switch_down_png, BinaryData::power_switch_down_pngSize), 1.0, Colours::transparentWhite,
                0.0);
            ampLED.setImages(true, true, true,
                ImageCache::getFromMemory(BinaryData::led_blue_on_png, BinaryData::led_blue_on_pngSize), 1.0, Colours::transparentWhite,
                Image(), 1.0, Colours::transparentWhite,
                ImageCache::getFromMemory(BinaryData::led_blue_on_png, BinaryData::led_blue_on_pngSize), 1.0, Colours::transparentWhite,
                0.0);
        }
    }
}

void SmartAmpProAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
	
    // Amp Widgets
    ampBassKnob.setBounds(203, 242, 55, 75);
    ampMidKnob.setBounds(283, 242, 55, 75);
    ampTrebleKnob.setBounds(368, 242, 55, 75);
    ampGainKnob.setBounds(100, 225, 75, 95);
    ampMasterKnob.setBounds(542, 225, 75, 95);

    ampOnButton.setBounds(54, 259, 15, 25);
    ampLED.setBounds(636, 240, 15, 15);
}

void SmartAmpProAudioProcessorEditor::modelSelectChanged()
{
    const int selectedFileIndex = modelSelect.getSelectedItemIndex();
    if (selectedFileIndex >= 0 && selectedFileIndex < processor.jsonFiles.size()) {
        processor.loadConfig(processor.jsonFiles[selectedFileIndex]);
        processor.current_model_index = modelSelect.getSelectedItemIndex();
    }
}



void SmartAmpProAudioProcessorEditor::buttonClicked(juce::Button* button)
{
    if (button == &ampOnButton) {
        ampOnButtonClicked();
        helpLabel.setText("", juce::NotificationType::dontSendNotification);
    } 
}


void SmartAmpProAudioProcessorEditor::ampOnButtonClicked() {
    if (processor.amp_state == 0) {
        processor.amp_state = 1;
        // Reset the directory in case user is manually adding or removing models from the SmartAmpPro directory
        processor.resetDirectory(processor.userAppDataDirectory_tones);
    }
    else {
        processor.amp_state = 0;
    }
    repaint();
}


void SmartAmpProAudioProcessorEditor::sliderValueChanged(Slider* slider)
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
