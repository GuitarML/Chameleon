/*
  ==============================================================================

  blend

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"


//==============================================================================

class blend
{
public:
    blend();
    void process (const float* inData, float* outData, MidiBuffer& midiMessages, const int numSamples, const int numInputChannels);
    void setParameters(float blend_slider);

private:
    // Blend variables

    int srate = 44100; //TODO set from input 
	float sample;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (blend)
};
