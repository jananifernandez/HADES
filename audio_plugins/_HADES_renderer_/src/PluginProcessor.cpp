/*
 * This file is part of HADES
 * Copyright (c) 2021 - Janani Fernandez & Leo McCormack
 *
 * HADES is free software; you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option) any later
 * version.
 *
 * HADES is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * See <http://www.gnu.org/licenses/> for a copy of the GNU General Public
 * License.
 */

#include "PluginProcessor.h"
#include "PluginEditor.h"

#if JucePlugin_Build_AAX && !JucePlugin_AAXDisableDefaultSettingsChunks
# error "AAX Default Settings Chunk is enabled. This may override parameter defaults."
#endif

juce::AudioProcessorValueTreeState::ParameterLayout PluginProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    
    return { params.begin(), params.end() };
}

void PluginProcessor::parameterChanged(const juce::String& /*parameterID*/, float /*newValue*/)
{
}

void PluginProcessor::setParameterValuesUsingInternalState()
{
}

void PluginProcessor::setInternalStateUsingParameterValues()
{
}

PluginProcessor::PluginProcessor():
    AudioProcessor(BusesProperties()
        .withInput("Input", AudioChannelSet::discreteChannels(HADES_MAX_NUM_CHANNELS), true)
        .withOutput("Output", AudioChannelSet::discreteChannels(2), true)),
    ParameterManager(*this, createParameterLayout())
{
	nSampleRate = 48000;
	hades_renderer_create(&hHdR);
    
    /* Grab defaults */
    setParameterValuesUsingInternalState();

    startTimer(40); 
}

PluginProcessor::~PluginProcessor()
{
	hades_renderer_destroy(&hHdR);
}

void PluginProcessor::setCurrentProgram (int /*index*/)
{
}

const String PluginProcessor::getName() const
{
    return JucePlugin_Name;
}

double PluginProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int PluginProcessor::getNumPrograms()
{
    return 1;
}

int PluginProcessor::getCurrentProgram()
{
    return 0;
}

const String PluginProcessor::getProgramName (int /*index*/)
{
    return String();
}

bool PluginProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool PluginProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

void PluginProcessor::changeProgramName (int /*index*/, const String& /*newName*/)
{
}

void PluginProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    nHostBlockSize = samplesPerBlock;
    nNumInputs =  jmin(getTotalNumInputChannels(), 256);
    nNumOutputs = jmin(getTotalNumOutputChannels(), 256);
    nSampleRate = (int)(sampleRate + 0.5);
    
    hades_renderer_init(hHdR, sampleRate);
    AudioProcessor::setLatencySamples(hades_renderer_getProcessingDelay(hHdR));
}

void PluginProcessor::releaseResources()
{
}

void PluginProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& /*midiMessages*/)
{
    ScopedNoDenormals noDenormals;
    
    int nCurrentBlockSize = nHostBlockSize = buffer.getNumSamples();
    nNumInputs = jmin(getTotalNumInputChannels(), buffer.getNumChannels(), 256);
    nNumOutputs = jmin(getTotalNumOutputChannels(), buffer.getNumChannels(), 256);
    float* const* bufferData = buffer.getArrayOfWritePointers(); 
    float* pFrameData[256];
    int frameSize = hades_renderer_getFrameSize();

    if(nCurrentBlockSize % frameSize == 0){ /* divisible by frame size */
        for(int frame = 0; frame < nCurrentBlockSize/frameSize; frame++) {
            for(int ch = 0; ch < jmin(buffer.getNumChannels(), 256); ch++)
                pFrameData[ch] = &bufferData[ch][frame*frameSize];

            /* perform processing */
            hades_renderer_process(hHdR, pFrameData, pFrameData, nNumInputs, nNumOutputs, frameSize);
        }
    }
    else
        buffer.clear();
}

//==============================================================================
bool PluginProcessor::hasEditor() const
{
    return true; 
}

AudioProcessorEditor* PluginProcessor::createEditor()
{
    return new PluginEditor (*this);
}

//==============================================================================
void PluginProcessor::getStateInformation (MemoryBlock& destData)
{
    juce::ValueTree state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    xml->setTagName("HADESRENDERERAUDIOPLUGINSETTINGS");
    xml->setAttribute("VersionCode", JucePlugin_VersionCode); // added since 0x10002
    
    /* Now for the other DSP object parameters (that have no JUCE parameter counterpart) */
    for(int band=0; band<hades_renderer_getNumberOfBands(hHdR); band++){
        xml->setAttribute("Balance"+String(band), hades_renderer_getStreamBalance(hHdR, band));
    }
    xml->setAttribute("DoAestimator", String(hades_renderer_getDoAestimator(hHdR)));
    xml->setAttribute("DiffEstimator", String(hades_renderer_getDiffusenessEstimator(hHdR)));
    xml->setAttribute("beamformerType", String(hades_renderer_getBeamformer(hHdR)));
    xml->setAttribute("covMatchingEnable", String(hades_renderer_getEnableCovMatching(hHdR)));
    xml->setAttribute("analysisAveraging", String(hades_renderer_getAnalysisAveraging(hHdR)));
    xml->setAttribute("synthesisAveraging", String(hades_renderer_getSynthesisAveraging(hHdR)));
    xml->setAttribute("refSensorIndexLEFT", String(hades_renderer_getReferenceSensorIndex(hHdR, 0)));
    xml->setAttribute("refSensorIndexRIGHT", String(hades_renderer_getReferenceSensorIndex(hHdR, 1)));

    //if(!hades_renderer_getSofaFilePathMAIR(hHdR))
         xml->setAttribute("SofaFilePath_MAIR", String(hades_renderer_getSofaFilePathMAIR(hHdR)));
    //if(!hades_renderer_getSofaFilePathHRIR(hHdR))
         xml->setAttribute("SofaFilePath_HRIR", String(hades_renderer_getSofaFilePathHRIR(hHdR)));

    /* Save */
    copyXmlToBinary(*xml, destData);
}

void PluginProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    /* Load */
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState != nullptr && xmlState->hasTagName("HADESRENDERERAUDIOPLUGINSETTINGS")){
        if(!xmlState->hasAttribute("VersionCode")){ // pre-0x10002
            for(int band=0; band<hades_renderer_getNumberOfBands(hHdR); band++){
                if(xmlState->hasAttribute("Balance"+String(band)))
                    hades_renderer_setStreamBalance(hHdR, (float)xmlState->getDoubleAttribute("Balance"+String(band),0), band);
            }
            if(xmlState->hasAttribute("SofaFilePath_MAIR")){
                String directory = xmlState->getStringAttribute("SofaFilePath_MAIR", "no_file");
                const char* new_cstring = (const char*)directory.toUTF8();
                hades_renderer_setSofaFilePathMAIR(hHdR, new_cstring);
            }

            if(xmlState->hasAttribute("SofaFilePath_HRIR")){
                String directory = xmlState->getStringAttribute("SofaFilePath_HRIR", "no_file");
                const char* new_cstring = (const char*)directory.toUTF8();
                hades_renderer_setSofaFilePathHRIR(hHdR, new_cstring);
            }

            if(xmlState->hasAttribute("DoAestimator"))
                hades_renderer_setDoAestimator(hHdR, (HADES_RENDERER_DOA_ESTIMATORS)xmlState->getIntAttribute("DoAestimator",1));
            if(xmlState->hasAttribute("DiffEstimator"))
                hades_renderer_setDiffusenessEstimator(hHdR, (HADES_RENDERER_DIFFUSENESS_ESTIMATORS)xmlState->getIntAttribute("DiffEstimator",1));
            if(xmlState->hasAttribute("beamformerType"))
                hades_renderer_setBeamformer(hHdR, (HADES_RENDERER_BEAMFORMER_TYPE)xmlState->getIntAttribute("beamformerType",1));
            if(xmlState->hasAttribute("covMatchingEnable"))
                hades_renderer_setEnableCovMatching(hHdR, xmlState->getIntAttribute("covMatchingEnable",1));
            if(xmlState->hasAttribute("analysisAveraging"))
                hades_renderer_setAnalysisAveraging(hHdR, (float)xmlState->getDoubleAttribute("analysisAveraging",0.5));
            if(xmlState->hasAttribute("synthesisAveraging"))
                hades_renderer_setSynthesisAveraging(hHdR, (float)xmlState->getDoubleAttribute("synthesisAveraging",0.5));
            if(xmlState->hasAttribute("refSensorIndexLEFT"))
                hades_renderer_setReferenceSensorIndex(hHdR, 0, xmlState->getIntAttribute("refSensorIndexLEFT",1));
            if(xmlState->hasAttribute("refSensorIndexRIGHT"))
                hades_renderer_setReferenceSensorIndex(hHdR, 1, xmlState->getIntAttribute("refSensorIndexRIGHT",1));

            setParameterValuesUsingInternalState();
        }
        else if(xmlState->getIntAttribute("VersionCode")>=0x10002){
            parameters.replaceState(juce::ValueTree::fromXml(*xmlState));
            
            /* Now for the other DSP object parameters (that have no JUCE parameter counterpart) */
            for(int band=0; band<hades_renderer_getNumberOfBands(hHdR); band++){
                if(xmlState->hasAttribute("Balance"+String(band)))
                    hades_renderer_setStreamBalance(hHdR, (float)xmlState->getDoubleAttribute("Balance"+String(band),0), band);
            }
            if(xmlState->hasAttribute("SofaFilePath_MAIR")){
                String directory = xmlState->getStringAttribute("SofaFilePath_MAIR", "no_file");
                const char* new_cstring = (const char*)directory.toUTF8();
                hades_renderer_setSofaFilePathMAIR(hHdR, new_cstring);
            }

            if(xmlState->hasAttribute("SofaFilePath_HRIR")){
                String directory = xmlState->getStringAttribute("SofaFilePath_HRIR", "no_file");
                const char* new_cstring = (const char*)directory.toUTF8();
                hades_renderer_setSofaFilePathHRIR(hHdR, new_cstring);
            }

            if(xmlState->hasAttribute("DoAestimator"))
                hades_renderer_setDoAestimator(hHdR, (HADES_RENDERER_DOA_ESTIMATORS)xmlState->getIntAttribute("DoAestimator",1));
            if(xmlState->hasAttribute("DiffEstimator"))
                hades_renderer_setDiffusenessEstimator(hHdR, (HADES_RENDERER_DIFFUSENESS_ESTIMATORS)xmlState->getIntAttribute("DiffEstimator",1));
            if(xmlState->hasAttribute("beamformerType"))
                hades_renderer_setBeamformer(hHdR, (HADES_RENDERER_BEAMFORMER_TYPE)xmlState->getIntAttribute("beamformerType",1));
            if(xmlState->hasAttribute("covMatchingEnable"))
                hades_renderer_setEnableCovMatching(hHdR, xmlState->getIntAttribute("covMatchingEnable",1));
            if(xmlState->hasAttribute("analysisAveraging"))
                hades_renderer_setAnalysisAveraging(hHdR, (float)xmlState->getDoubleAttribute("analysisAveraging",0.5));
            if(xmlState->hasAttribute("synthesisAveraging"))
                hades_renderer_setSynthesisAveraging(hHdR, (float)xmlState->getDoubleAttribute("synthesisAveraging",0.5));
            if(xmlState->hasAttribute("refSensorIndexLEFT"))
                hades_renderer_setReferenceSensorIndex(hHdR, 0, xmlState->getIntAttribute("refSensorIndexLEFT",1));
            if(xmlState->hasAttribute("refSensorIndexRIGHT"))
                hades_renderer_setReferenceSensorIndex(hHdR, 1, xmlState->getIntAttribute("refSensorIndexRIGHT",1));
            
            /* Many hosts will also trigger parameterChanged() for all parameters after calling setStateInformation() */
            /* However, some hosts do not. Therefore, it is better to ensure that the internal state is always up-to-date by calling: */
            setInternalStateUsingParameterValues();
        }
        
        hades_renderer_refreshSettings(hHdR);
	}
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PluginProcessor();
}

