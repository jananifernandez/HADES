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

PluginProcessor::PluginProcessor():
AudioProcessor(BusesProperties()
    .withInput("Input", AudioChannelSet::discreteChannels(HADES_MAX_NUM_CHANNELS), true)
    .withOutput("Output", AudioChannelSet::discreteChannels(2), true))
{
	nSampleRate = 48000;
	hades_renderer_create(&hHdR);

    startTimer(TIMER_PROCESSING_RELATED, 40); 
}

PluginProcessor::~PluginProcessor()
{
	hades_renderer_destroy(&hHdR);
}

void PluginProcessor::setParameter (int index, float /*newValue*/)
{
	switch (index) { 
		default: break;
	}
}

void PluginProcessor::setCurrentProgram (int /*index*/)
{
}

float PluginProcessor::getParameter (int index)
{
    switch (index) {
		default: return 0.0f;
	}
}

int PluginProcessor::getNumParameters()
{
	return k_NumOfParameters;
}

const String PluginProcessor::getName() const
{
    return JucePlugin_Name;
}

const String PluginProcessor::getParameterName (int index)
{
    switch (index) {
        default: return "NULL";
	}
}

const String PluginProcessor::getParameterText(int index)
{
    switch (index) {
        default: return "NULL";
    }
}

const String PluginProcessor::getInputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

const String PluginProcessor::getOutputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
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

bool PluginProcessor::isInputChannelStereoPair (int /*index*/) const
{
    return true;
}

bool PluginProcessor::isOutputChannelStereoPair (int /*index*/) const
{
    return true;
}

bool PluginProcessor::isBusesLayoutSupported(const BusesLayout& /*layouts*/) const
{
    return true;
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

bool PluginProcessor::silenceInProducesSilenceOut() const
{
    return false;
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
    return new PluginEditor (this);
}

//==============================================================================
void PluginProcessor::getStateInformation (MemoryBlock& destData)
{
	/* Create an outer XML element.. */ 
	XmlElement xml("HADESRENDERERAUDIOPLUGINSETTINGS");
 
    for(int band=0; band<hades_renderer_getNumberOfBands(hHdR); band++){
        xml.setAttribute("Balance"+String(band), hades_renderer_getStreamBalance(hHdR, band));
    }
    xml.setAttribute("DoAestimator", String(hades_renderer_getDoAestimator(hHdR)));
    xml.setAttribute("DiffEstimator", String(hades_renderer_getDiffusenessEstimator(hHdR)));
    xml.setAttribute("beamformerType", String(hades_renderer_getBeamformer(hHdR)));
    xml.setAttribute("covMatchingEnable", String(hades_renderer_getEnableCovMatching(hHdR)));
    xml.setAttribute("analysisAveraging", String(hades_renderer_getAnalysisAveraging(hHdR)));
    xml.setAttribute("synthesisAveraging", String(hades_renderer_getSynthesisAveraging(hHdR)));
    xml.setAttribute("refSensorIndexLEFT", String(hades_renderer_getReferenceSensorIndex(hHdR, 0)));
    xml.setAttribute("refSensorIndexRIGHT", String(hades_renderer_getReferenceSensorIndex(hHdR, 1)));

    //if(!hades_renderer_getSofaFilePathMAIR(hHdR))
         xml.setAttribute("SofaFilePath_MAIR", String(hades_renderer_getSofaFilePathMAIR(hHdR)));
    //if(!hades_renderer_getSofaFilePathHRIR(hHdR))
         xml.setAttribute("SofaFilePath_HRIR", String(hades_renderer_getSofaFilePathHRIR(hHdR)));

	copyXmlToBinary(xml, destData);
}

void PluginProcessor::setStateInformation (const void* data, int sizeInBytes)
{
	/* This getXmlFromBinary() function retrieves XML from the binary blob */
    std::unique_ptr<XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    
	if (xmlState != nullptr) {
		/* make sure that it's actually the correct type of XML object */
		if (xmlState->hasTagName("HADESRENDERERAUDIOPLUGINSETTINGS")) {

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

            hades_renderer_refreshSettings(hHdR);
        }
	}
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PluginProcessor();
}

