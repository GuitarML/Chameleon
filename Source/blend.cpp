/*
  ==============================================================================

  blend
  
  ==============================================================================
*/

#include "blend.h"

blend::blend()
{
    setParameters(0.0, 0.0, 0.0, 0.0);
}

void blend::process (const float* dryData, const float* inData, float* outData,
                                    MidiBuffer& midiMessages, 
                                    const int numSamples, 
                                    const int numInputChannels)
{
    for (int sample = 0; sample < numSamples; ++sample) {
        sample = inData[sample] * blend_ratio + dryData * (1 - blend_ratio);
        outData[sample] = sample;
    }
}

void blend::setParameters(float blend_slider)
{
    blend_ratio = blend_slider;
}