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
    treeState(*this, nullptr, "PARAMETER", { std::make_unique<AudioParameterFloat>(GAIN_ID, GAIN_NAME, NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.5f),
                        std::make_unique<AudioParameterFloat>(BASS_ID, BASS_NAME, NormalisableRange<float>(-8.0f, 8.0f, 0.01f), 0.0f),
                        std::make_unique<AudioParameterFloat>(MID_ID, MID_NAME, NormalisableRange<float>(-8.0f, 8.0f, 0.01f), 0.0f),
                        std::make_unique<AudioParameterFloat>(TREBLE_ID, TREBLE_NAME, NormalisableRange<float>(-8.0f, 8.0f, 0.01f), 0.0f),
                        std::make_unique<AudioParameterFloat>(PRESENCE_ID, PRESENCE_NAME, NormalisableRange<float>(-8.0f, 8.0f, 0.01f), 0.0f),
                        std::make_unique<AudioParameterFloat>(MASTER_ID, MASTER_NAME, NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.5f) })

#endif
{
    setMode();

    gainParam = treeState.getRawParameterValue (GAIN_ID);
    bassParam = treeState.getRawParameterValue (BASS_ID);
    midParam = treeState.getRawParameterValue (MID_ID);
    trebleParam = treeState.getRawParameterValue (TREBLE_ID);
    presenceParam = treeState.getRawParameterValue (PRESENCE_ID);
    masterParam = treeState.getRawParameterValue (MASTER_ID);

    auto bassValue = static_cast<float> (bassParam->load());
    auto midValue = static_cast<float> (midParam->load());
    auto trebleValue = static_cast<float> (trebleParam->load());
    auto presenceValue = static_cast<float> (presenceParam->load());

    eq4band.setParameters(bassValue, midValue, trebleValue, presenceValue);
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
    LSTM2.reset();

    // prepare resampler for target sample rate: 44.1 kHz
    constexpr double targetSampleRate = 44100.0;
    resampler.prepareWithTargetSampleRate ({ sampleRate, (uint32) samplesPerBlock, 2 }, targetSampleRate);

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

    auto ampDrive = static_cast<float> (gainParam->load());
    auto bassValue = static_cast<float> (bassParam->load());
    auto midValue = static_cast<float> (midParam->load());
    auto trebleValue = static_cast<float> (trebleParam->load());
    auto presenceValue = static_cast<float> (presenceParam->load());
    auto ampMaster = static_cast<float> (masterParam->load());

    dsp::AudioBlock<float> block(buffer);
    // Amp =============================================================================
    if (amp_state == 1) {

        // Apply ramped changes for gain smoothing
        if (ampDrive == previousAmpDrive)
        {
            buffer.applyGain(ampDrive*2.0);
        }
        else {
            buffer.applyGainRamp(0, (int) buffer.getNumSamples(), previousAmpDrive*2.0, ampDrive*2.0);
            previousAmpDrive = ampDrive;
        }

        // resample to target sample rate
        auto block44k = resampler.processIn (block);

        for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
        {
            // Apply LSTM model
            if (ch == 0) {
                LSTM.process(block44k.getChannelPointer(0), block44k.getChannelPointer(0), (int) block44k.getNumSamples());
            
            }
            else if (ch == 1) {
                LSTM2.process(block44k.getChannelPointer(1), block44k.getChannelPointer(1), (int) block44k.getNumSamples());
            }
        }

	
        // resample back to original sample rate
        resampler.processOut (block44k, block);

        //eq4band.setParameters(bassValue, midValue, trebleValue, presenceValue);

        for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
        {
            // Apply EQ
            if (ch == 0) {
                eq4band.process(buffer.getReadPointer(0), buffer.getWritePointer(0), midiMessages, numSamples, numInputChannels, sampleRate);
            
            }
            else if (ch == 1) {
                eq4band.process(buffer.getReadPointer(1), buffer.getWritePointer(1), midiMessages, numSamples, numInputChannels, sampleRate);
            }
        }

        // Master Volume 
        // Apply ramped changes for gain smoothing
        if (ampMaster == previousAmpMaster)
        {
            buffer.applyGain(ampMaster *2.0);
        }
        else {
            buffer.applyGainRamp(0, (int) buffer.getNumSamples(), previousAmpMaster *2.0, ampMaster * 2.0);
            previousAmpMaster = ampMaster;
        }

        // Custom Level for quieter models
        if (current_model_index == 2) {
            buffer.applyGain(2.0);
        }
    }
    
    // process DC blocker
    dsp::ProcessContextReplacing<float> context(block);
    dcBlocker.process(context);
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
            setMode();

            if (auto* editor = dynamic_cast<ChameleonAudioProcessorEditor*> (getActiveEditor()))
                editor->resetImages();
        }
    }
}

void ChameleonAudioProcessor::set_ampEQ(float bass_slider, float mid_slider, float treble_slider, float presence_slider)
{
    eq4band.setParameters(bass_slider, mid_slider, treble_slider, presence_slider);
}

void ChameleonAudioProcessor::setMode()
{
    
    if (current_model_index ==0) {
        MemoryInputStream jsonInputStream(BinaryData::red_json, BinaryData::red_jsonSize, false);
        nlohmann::json weights_json = nlohmann::json::parse(jsonInputStream.readEntireStreamAsString().toStdString());
        LSTM.reset();
        LSTM2.reset();
        LSTM.load_json(weights_json);
        LSTM2.load_json(weights_json);

    } else if (current_model_index == 1) {
        MemoryInputStream jsonInputStream(BinaryData::gold_json, BinaryData::gold_jsonSize, false);
        nlohmann::json weights_json = nlohmann::json::parse(jsonInputStream.readEntireStreamAsString().toStdString());
        LSTM.reset();
        LSTM2.reset();
        LSTM.load_json(weights_json);
        LSTM2.load_json(weights_json);

    } else if (current_model_index == 2) {
        MemoryInputStream jsonInputStream(BinaryData::green_json, BinaryData::green_jsonSize, false);
        nlohmann::json weights_json = nlohmann::json::parse(jsonInputStream.readEntireStreamAsString().toStdString());
        LSTM.reset();
        LSTM2.reset();
        LSTM.load_json(weights_json);
        LSTM2.load_json(weights_json);

    }
  
}
//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ChameleonAudioProcessor();
}
