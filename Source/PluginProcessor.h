/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include <nlohmann/json.hpp>
//#include "lstm.h"
#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Eq4Band.h"

#include "RTNeuralLSTM.h"

#define GAIN_ID "gain"
#define GAIN_NAME "Gain"
#define BASS_ID "bass"
#define BASS_NAME "Bass"
#define MID_ID "mid"
#define MID_NAME "Mid"
#define TREBLE_ID "treble"
#define TREBLE_NAME "Treble"
#define PRESENCE_ID "presence"
#define PRESENCE_NAME "Presence"
#define MASTER_ID "master"
#define MASTER_NAME "Master"


//==============================================================================
/**
*/
class ChameleonAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    ChameleonAudioProcessor();
    ~ChameleonAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    void loadConfig(File configFile);
    void setupDataDirectories();
    void installTones();

    // Amp
    void set_ampDrive(float db_ampCleanDrive);
    void set_ampMaster(float db_ampMaster);
    void set_ampEQ(float bass_slider, float mid_slider, float treble_slider, float presence_slider);

    float decibelToLinear(float dbValue);

    std::vector<File> jsonFiles;
    File currentDirectory = File::getCurrentWorkingDirectory().getFullPathName();
    File userAppDataDirectory = File::getSpecialLocation(File::userDocumentsDirectory).getChildFile(JucePlugin_Manufacturer).getChildFile(JucePlugin_Name);
    File userAppDataDirectory_tones = userAppDataDirectory.getFullPathName() + "/tones";

    File red_tone = userAppDataDirectory_tones.getFullPathName() + "/red.json";
    File gold_tone = userAppDataDirectory_tones.getFullPathName() + "/gold.json";
    File green_tone = userAppDataDirectory_tones.getFullPathName() + "/green.json";

    // Pedal/amp states
    int amp_state = 1; // 0 = off, 1 = on
    int custom_tone = 0; // 0 = custom tone loaded, 1 = default channel tone
    File loaded_tone;
    juce::String loaded_tone_name;
    const char* char_filename = "";
    int model_loaded = 0;
    int current_model_index = 0; // 0 = red, 1 = gold, 2 = green
    int fromUpDown = 0;

    // Amp knob states
    float ampBassKnobState = 0.0;
    float ampMidKnobState = 0.0;
    float ampTrebleKnobState = 0.0;
    float ampGainKnobState = 0.0;
    float ampMasterKnobState = -18.0;
    float ampPresenceKnobState = 0.0;
  
    RT_LSTM LSTM;
    RT_LSTM LSTM2;

    AudioProcessorValueTreeState treeState;

private:
    Eq4Band eq4band; // Amp EQ

    // Amp
    float ampDrive = 1.0;
    float ampMaster = 1.0;
    float previousAmpDrive = 1.0;
    float previousAmpMaster = 1.0;

    var dummyVar;

    chowdsp::ResampledProcess<chowdsp::ResamplingTypes::SRCResampler<>> resampler;

    dsp::IIR::Filter<float> dcBlocker;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ChameleonAudioProcessor)
};
