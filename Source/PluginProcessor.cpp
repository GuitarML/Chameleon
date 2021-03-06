/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <iostream>
#include <fstream>

//==============================================================================
ChameleonAudioProcessor::ChameleonAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", AudioChannelSet::stereo(), true)
#endif
    ),
    //treeState(*this, nullptr, "PARAMETER", { std::make_unique<AudioParameterFloat>(GAIN_ID, GAIN_NAME, NormalisableRange<float>(-10.0f, 10.0f, 0.01f), 0.0f),
    treeState(*this, nullptr, "PARAMETER", { std::make_unique<AudioParameterFloat>(GAIN_ID, GAIN_NAME, NormalisableRange<float>(-10.0f, 10.0f, 0.01f), 0.0f),
                        std::make_unique<AudioParameterFloat>(BASS_ID, BASS_NAME, NormalisableRange<float>(-8.0f, 8.0f, 0.01f), 0.0f),
                        std::make_unique<AudioParameterFloat>(MID_ID, MID_NAME, NormalisableRange<float>(-8.0f, 8.0f, 0.01f), 0.0f),
                        std::make_unique<AudioParameterFloat>(TREBLE_ID, TREBLE_NAME, NormalisableRange<float>(-8.0f, 8.0f, 0.01f), 0.0f),
                        std::make_unique<AudioParameterFloat>(PRESENCE_ID, PRESENCE_NAME, NormalisableRange<float>(-8.0f, 8.0f, 0.01f), 0.0f),
                        std::make_unique<AudioParameterFloat>(MASTER_ID, MASTER_NAME, NormalisableRange<float>(-36.0f, 0.0f, 0.01f), 0.0f) })

#endif
{
    setupDataDirectories();
    installTones();
    loadConfig(red_tone);
}

ChameleonAudioProcessor::~ChameleonAudioProcessor()
{
}

//==============================================================================
const String ChameleonAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ChameleonAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ChameleonAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool ChameleonAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double ChameleonAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ChameleonAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int ChameleonAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ChameleonAudioProcessor::setCurrentProgram (int index)
{
}

const String ChameleonAudioProcessor::getProgramName (int index)
{
    return {};
}

void ChameleonAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void ChameleonAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    LSTM.reset();

    // prepare resampler for target sample rate: 44.1 kHz
    constexpr double targetSampleRate = 44100.0;
    resampler.prepareWithTargetSampleRate ({ sampleRate, (uint32) samplesPerBlock, 1 }, targetSampleRate);

    // set up DC blocker
    dcBlocker.coefficients = dsp::IIR::Coefficients<float>::makeHighPass (sampleRate, 35.0f);
    dsp::ProcessSpec spec { sampleRate, static_cast<uint32> (samplesPerBlock), 2 };
    dcBlocker.prepare (spec);
}

void ChameleonAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ChameleonAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif


void ChameleonAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;

    // Setup Audio Data
    const int numSamples = buffer.getNumSamples();
    const int numInputChannels = getTotalNumInputChannels();
    const int sampleRate = getSampleRate();


    // Amp =============================================================================
    if (amp_state == 1) {
        //    EQ (Presence, Bass, Mid, Treble)
        eq4band.process(buffer.getReadPointer(0), buffer.getWritePointer(0), midiMessages, numSamples, numInputChannels, sampleRate);


        // Apply ramped changes for gain smoothing
        if (ampDrive == previousAmpDrive)
        {
            buffer.applyGain(ampDrive);
        }
        else {
            buffer.applyGainRamp(0, (int) buffer.getNumSamples(), previousAmpDrive, ampDrive);
            previousAmpDrive = ampDrive;
        }

        // resample to target sample rate
        auto block = dsp::AudioBlock<float> (buffer.getArrayOfWritePointers(), 1, numSamples);
        auto block44k = resampler.processIn (block);

	// Apply LSTM model
        LSTM.process(block44k.getChannelPointer(0), block44k.getChannelPointer(0), (int) block44k.getNumSamples());

        // resample back to original sample rate
        resampler.processOut (block44k, block);

        // Master Volume 
        // Apply ramped changes for gain smoothing
        if (ampMaster == previousAmpMaster)
        {
            buffer.applyGain(ampMaster);
        }
        else {
            buffer.applyGainRamp(0, (int) buffer.getNumSamples(), previousAmpMaster, ampMaster);
            previousAmpMaster = ampMaster;
        }

        // Custom Level for quieter models
        if (current_model_index == 2) {
            buffer.applyGain(2.0);
        }
    }
    

    // process DC blocker
    auto monoBlock = dsp::AudioBlock<float> (buffer).getSingleChannelBlock (0);
    dcBlocker.process (dsp::ProcessContextReplacing<float> (monoBlock));

    // Handle stereo input by copying channel 1 to channel 2
    for (int ch = 1; ch < buffer.getNumChannels(); ++ch)
        buffer.copyFrom(ch, 0, buffer, 0, 0, buffer.getNumSamples());
}

//==============================================================================
bool ChameleonAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* ChameleonAudioProcessor::createEditor()
{
    return new ChameleonAudioProcessorEditor (*this);  // Note: error on this line caused by unused inherited classes in Editor
}

//==============================================================================
void ChameleonAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.

    auto state = treeState.copyState();
    std::unique_ptr<XmlElement> xml (state.createXml());
    xml->setAttribute ("current_tone", current_model_index);
    copyXmlToBinary (*xml, destData);
}

void ChameleonAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.

    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));

    if (xmlState.get() != nullptr)
    {
        if (xmlState->hasTagName (treeState.state.getType()))
        {
            treeState.replaceState (juce::ValueTree::fromXml (*xmlState));
            current_model_index = xmlState->getIntAttribute ("current_tone");

            switch (current_model_index)
            {
            case 0:
                loadConfig (red_tone);
                break;
            case 1:
                loadConfig (gold_tone);
                break;
            case 2:
                loadConfig (green_tone);
                break;
            }

            if (auto* editor = dynamic_cast<ChameleonAudioProcessorEditor*> (getActiveEditor()))
                editor->resetImages();
        }
    }
}

void ChameleonAudioProcessor::loadConfig(File configFile)
{
    this->suspendProcessing(true);
    model_loaded = 1;
    String path = configFile.getFullPathName();
    char_filename = path.toUTF8();

    LSTM.load_json(char_filename);

    this->suspendProcessing(false);
}


void ChameleonAudioProcessor::setupDataDirectories()
{
    // User app data directory
    File userAppDataTempFile = userAppDataDirectory.getChildFile("tmp.pdl");

    File userAppDataTempFile_tones = userAppDataDirectory_tones.getChildFile("tmp.pdl");

    // Create (and delete) temp file if necessary, so that user doesn't have
    // to manually create directories
    if (!userAppDataDirectory.exists()) {
        userAppDataTempFile.create();
    }
    if (userAppDataTempFile.existsAsFile()) {
        userAppDataTempFile.deleteFile();
    }

    if (!userAppDataDirectory_tones.exists()) {
        userAppDataTempFile_tones.create();
    }
    if (userAppDataTempFile_tones.existsAsFile()) {
        userAppDataTempFile_tones.deleteFile();
    }
}

void ChameleonAudioProcessor::installTones()
//====================================================================
// Description: Checks that the default tones
//  are installed to the Chameleon directory, and if not, 
//  copy them from the binary data in the plugin to that directory.
//
//====================================================================
{
    if (red_tone.existsAsFile() == false) {
        std::string string_command = red_tone.getFullPathName().toStdString();
        const char* char_red = &string_command[0];

        std::ofstream myfile;
        myfile.open(char_red);
        myfile << BinaryData::red_json;

        myfile.close();
    }

    if (gold_tone.existsAsFile() == false) {
        std::string string_command = gold_tone.getFullPathName().toStdString();
        const char* char_gold = &string_command[0];

        std::ofstream myfile;
        myfile.open(char_gold);
        myfile << BinaryData::gold_json;

        myfile.close();
    }

    if (green_tone.existsAsFile() == false) {
        std::string string_command = green_tone.getFullPathName().toStdString();
        const char* char_green = &string_command[0];

        std::ofstream myfile;
        myfile.open(char_green);
        myfile << BinaryData::green_json;

        myfile.close();
    }
}


void ChameleonAudioProcessor::set_ampDrive(float db_ampDrive)
{
    ampDrive = decibelToLinear(db_ampDrive);
    ampGainKnobState = db_ampDrive;
}

void ChameleonAudioProcessor::set_ampMaster(float db_ampMaster)
{
    ampMasterKnobState = db_ampMaster;
    if (db_ampMaster == -36.0) {
        ampMaster = decibelToLinear(-100.0);
    } else {
        ampMaster = decibelToLinear(db_ampMaster);
    }
}


void ChameleonAudioProcessor::set_ampEQ(float bass_slider, float mid_slider, float treble_slider, float presence_slider)
{
    eq4band.setParameters(bass_slider, mid_slider, treble_slider, presence_slider);
}

float ChameleonAudioProcessor::decibelToLinear(float dbValue)
{
    return powf(10.0, dbValue/20.0);
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ChameleonAudioProcessor();
}
