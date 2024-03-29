/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "myLookAndFeel.h"
#include <stdlib.h>

//==============================================================================
/**
*/
class ChameleonAudioProcessorEditor  : public AudioProcessorEditor,
                                       private Button::Listener,
                                       private Slider::Listener
                      
                                
{
public:
    ChameleonAudioProcessorEditor (ChameleonAudioProcessor&);
    ~ChameleonAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

    void resetImages();

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    ChameleonAudioProcessor& processor;

    // Amp Widgets
    Slider ampBassKnob;
    Slider ampMidKnob;
    Slider ampTrebleKnob;
    Slider ampGainKnob;
    Slider ampMasterKnob;
    Slider ampPresenceKnob;
    Label versionLabel;

    ImageButton colorSelectButton;
    ImageButton ampLED;

    
    // LookandFeels and Graphics
    Image background = ImageCache::getFromMemory(BinaryData::chameleon_amp_jpg, BinaryData::chameleon_amp_jpgSize);
    myLookAndFeel ampSilverKnobLAF;
    juce::Rectangle<int> ClipRect;

    juce::String fname;
    virtual void buttonClicked(Button* button) override;
    virtual void sliderValueChanged(Slider* slider) override;
    void colorSelectClicked();
    
public:
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> gainSliderAttach;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> bassSliderAttach;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> midSliderAttach;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> trebleSliderAttach;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> presenceSliderAttach;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> masterSliderAttach;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ChameleonAudioProcessorEditor)
};
