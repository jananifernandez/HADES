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

/**
 * @file ehades.c
 * @brief Using HADES to go from microphone array direct to binaural with some
 *        optional enhancement processing inbetween the analysis and synthesis
 * @author Janani Fernandez & Leo McCormack
 * @date 09.04.2021
 * @license GNU GPLv2
 */

#include "ehades.h"
#include "ehades_internal.h"

void hades_renderer_create
(
    void ** const phHdR
)
{
    hades_renderer_data* pData = (hades_renderer_data*)malloc1d(sizeof(hades_renderer_data));
    *phHdR = (void*)pData;
    int i;

    /* Default user parameters */
    pData->sofa_filepath_MAIR = NULL;
    pData->useDefaultHRIRsFLAG = 1;
    pData->sofa_filepath_HRIR = NULL;
    pData->binConfig.lHRIR = pData->binConfig.nHRIR = pData->binConfig.hrir_fs = 0;
    pData->binConfig.hrirs = NULL;
    pData->binConfig.hrir_dirs_deg = NULL;
    pData->refsensor_idx[0] = pData->refsensor_idx[1] = -1;
    pData->diffOption = HADES_RENDERER_USE_COMEDIE;
    pData->doaOption  = HADES_RENDERER_USE_MUSIC;
    pData->beamOption = HADES_RENDERER_BEAMFORMER_FILTER_AND_SUM;
    pData->enableCovMatching = 0; 

    /* Default values for the radial editor */
    for(i=0; i<360; i++)
        pData->dirGain_dB[i] = 0.0f;

    /* internal parameters */
    pData->inputFrameTD =  (float**)malloc2d(HADES_MAX_NUM_CHANNELS, FRAME_SIZE, sizeof(float));
    pData->outputFrameTD = (float**)malloc2d(NUM_EARS, FRAME_SIZE, sizeof(float));
    pData->fs = 48000.0f;
    pData->hAna = NULL;
    pData->hSyn = NULL;
    pData->hPCon = NULL;
    pData->hSCon = NULL;
    pData->hREd = NULL;

    /* Local copy of internal parameter vectors (for optional thread-safe GUI plotting) */
    pData->nBands_local = 0;
    pData->freqVector_local = NULL;
    pData->streamBalBands_local = NULL;

    /* our codec data */
    pData->progressBar0_1 = 0.0f;
    pData->progressBarText = malloc1d(HADES_PROGRESSBARTEXT_CHAR_LENGTH*sizeof(char));
    strcpy(pData->progressBarText,"");
    
    /* flags */
    pData->MAIR_SOFA_isLoadedFLAG = 0;
    pData->procStatus = PROC_STATUS_NOT_ONGOING;
    pData->codecStatus = CODEC_STATUS_NOT_INITIALISED;

    /* Init codec with defaults */
    hades_renderer_initCodec(*phHdR);
}

void hades_renderer_destroy
(
    void ** const phHdR
)
{
    hades_renderer_data *pData = (hades_renderer_data*)(*phHdR);

    if (pData != NULL) {
        /* not safe to free memory during intialisation/processing loop */
        while (pData->codecStatus == CODEC_STATUS_INITIALISING ||
               pData->procStatus == PROC_STATUS_ONGOING){
            SAF_SLEEP(10);
        }
        free(pData->binConfig.hrirs);
        free(pData->binConfig.hrir_dirs_deg);
        hades_analysis_destroy(&(pData->hAna));
        hades_param_container_destroy(&(pData->hPCon));
        hades_signal_container_destroy(&(pData->hSCon));
        hades_synthesis_destroy(&(pData->hSyn));
        hades_radial_editor_destroy(&(pData->hREd));
        free(pData->progressBarText);
        free(pData->inputFrameTD);
        free(pData->outputFrameTD);
        free(pData->freqVector_local);
        free(pData->streamBalBands_local);

        free(pData);
        pData = NULL;
        (*phHdR) = NULL;
    }
}

void hades_renderer_init
(
    void * const hHdR,
    int          sampleRate
)
{
    hades_renderer_data *pData = (hades_renderer_data*)(hHdR);

    if(sampleRate!=(int)pData->fs){
        pData->fs = (float)sampleRate;
        hades_renderer_setCodecStatus(hHdR, CODEC_STATUS_NOT_INITIALISED);
    }

    /* reset (flush internal buffers with zeros etc.) */
    if(pData->codecStatus == CODEC_STATUS_INITIALISED){
        hades_analysis_reset(pData->hAna);
        hades_synthesis_reset(pData->hSyn); 
    }
}

void hades_renderer_initCodec
(
    void* const hHdR
)
{
    hades_renderer_data *pData = (hades_renderer_data*)(hHdR);
    int load_prevFLAG, nBands;
    float* eq, *streamBalance, *tmp;
    SAF_SOFA_ERROR_CODES error;
    saf_sofa_container sofa;
    float* grid_dirs_deg;
    HADES_DOA_ESTIMATORS doaOpt;
    HADES_DIFFUSENESS_ESTIMATORS diffOpt;
    HADES_BEAMFORMER_TYPE beamOpt;

    if (pData->codecStatus != CODEC_STATUS_NOT_INITIALISED)
        return; /* re-init not required, or already happening */
    while (pData->procStatus == PROC_STATUS_ONGOING){
        /* re-init required, but we need to wait for the current processing loop to end */
        pData->codecStatus = CODEC_STATUS_INITIALISING; /* indicate that we want to init */
        SAF_SLEEP(10);
    }
    
    /* for progress bar */
    pData->codecStatus = CODEC_STATUS_INITIALISING;
    strcpy(pData->progressBarText,"Intialising Codec");
    pData->progressBar0_1 = 0.0f;

    /* Local copy of internal settings (since they are overriden) */
    if(pData->hAna!=NULL && pData->hSyn!=NULL){
        load_prevFLAG = 1;
        tmp = hades_synthesis_getEqPtr(pData->hSyn, &nBands);
        eq = malloc1d(nBands*sizeof(float));
        memcpy(eq, tmp, nBands*sizeof(float));
        tmp = hades_synthesis_getStreamBalancePtr(pData->hSyn, NULL);
        streamBalance = malloc1d(nBands*sizeof(float));
        memcpy(streamBalance, tmp, nBands*sizeof(float));
    }
    else{
        load_prevFLAG = 0;
        eq = streamBalance = NULL;
        nBands = -1;
    }

    /* Parse hades renderer enums into hades api enums */
    switch(pData->doaOption){
        default: /* fall through */
        case HADES_RENDERER_USE_MUSIC: doaOpt = HADES_USE_MUSIC; break;
    }
    switch(pData->diffOption){
        default: /* fall through */
        case HADES_RENDERER_USE_COMEDIE: diffOpt = HADES_USE_COMEDIE; break;
    }
    switch(pData->beamOption){
        default: /* fall through */
        case HADES_RENDERER_BEAMFORMER_NONE: beamOpt = HADES_BEAMFORMER_NONE; break;
        case HADES_RENDERER_BEAMFORMER_FILTER_AND_SUM: beamOpt = HADES_BEAMFORMER_FILTER_AND_SUM; break;
        case HADES_RENDERER_BEAMFORMER_BMVDR: beamOpt = HADES_BEAMFORMER_BMVDR; break;
    }

//    pData->refsensor_idx[0] = 1;
//    pData->refsensor_idx[1] = 5;
//    "/Users/mccorml1/Documents/git/JanProjects/matlab/h_array/h_array_horiz1deg_357.sofa"

    /* Load SOFA file */
    error = saf_sofa_open(&sofa, pData->sofa_filepath_MAIR, SAF_SOFA_READER_OPTION_DEFAULT);
    if(error==SAF_SOFA_OK){
        pData->nDirs = sofa.nSources;
        pData->nMics = sofa.nReceivers;
        pData->IR_fs = sofa.DataSamplingRate;
        pData->IRlength = sofa.DataLengthIR;
        grid_dirs_deg = malloc1d(pData->nDirs*2*sizeof(float));
        cblas_scopy(pData->nDirs, sofa.SourcePosition, 3, grid_dirs_deg, 2); /* azi */
        cblas_scopy(pData->nDirs, &sofa.SourcePosition[1], 3, &grid_dirs_deg[1], 2); /* elev */

        /* Default reference sensor indices (if not defined or not valid based on the number of sensors) */
        if(pData->refsensor_idx[0]<0             || pData->refsensor_idx[1]<0 ||
           pData->refsensor_idx[0]>=pData->nMics || pData->refsensor_idx[1]>=pData->nMics){
            /* Assuming that the first nMics/2 sensors belong to the left device, and the rest to the right device */
            pData->refsensor_idx[0] = 0;
            pData->refsensor_idx[1] = (int)((float)pData->nMics/2.0f + 0.0001f);
        }

        /* Analysis */
        strcpy(pData->progressBarText,"Intialising Analysis");
        pData->progressBar0_1 = 0.3f;
        hades_analysis_destroy(&(pData->hAna));
        hades_analysis_create(&(pData->hAna), pData->fs, HADES_USE_AFSTFT, HOP_SIZE, FRAME_SIZE, SAF_TRUE /*hybridmode*/,
                              sofa.DataIR, grid_dirs_deg, pData->nDirs, pData->nMics, sofa.DataLengthIR,
                              diffOpt, doaOpt);
        *hades_analysis_getCovarianceAvagingCoeffPtr(pData->hAna) = 0.77f;
        free(grid_dirs_deg);

        /* Parameter/signal containers */
        strcpy(pData->progressBarText,"Intialising Containers");
        pData->progressBar0_1 = 0.5f;
        hades_param_container_destroy(&(pData->hPCon));
        hades_param_container_create(&(pData->hPCon), pData->hAna);
        hades_signal_container_destroy(&(pData->hSCon));
        hades_signal_container_create(&(pData->hSCon), pData->hAna);

        /* Synthesis */
        strcpy(pData->progressBarText,"Intialising Synthesis");
        pData->progressBar0_1 = 0.8f;
        saf_sofa_close(&sofa); /* Close previous */
        error = saf_sofa_open(&sofa, "/Users/mccorml1/Documents/HRIRs_SOFA/D2_48K_24bit_256tap_FIR_SOFA_KEMAR.sofa", SAF_SOFA_READER_OPTION_DEFAULT); //pData->useDefaultHRIRsFLAG ? NULL : pData->sofa_filepath_HRIR, //);
        if(error==SAF_SOFA_OK){
            pData->binConfig.nHRIR = sofa.nSources;
            pData->binConfig.hrir_fs = sofa.DataSamplingRate;
            pData->binConfig.lHRIR = sofa.DataLengthIR;
            pData->binConfig.hrir_dirs_deg = realloc1d(pData->binConfig.hrir_dirs_deg, pData->binConfig.nHRIR*2*sizeof(float));
            cblas_scopy(pData->binConfig.nHRIR, sofa.SourcePosition, 3, pData->binConfig.hrir_dirs_deg, 2); /* azi */
            cblas_scopy(pData->binConfig.nHRIR, &sofa.SourcePosition[1], 3, &pData->binConfig.hrir_dirs_deg[1], 2); /* elev */
            pData->binConfig.hrirs = realloc1d(pData->binConfig.hrirs, pData->binConfig.nHRIR*NUM_EARS*pData->binConfig.lHRIR*sizeof(float));
            cblas_scopy(pData->binConfig.nHRIR*NUM_EARS*pData->binConfig.lHRIR, sofa.DataIR, 1, pData->binConfig.hrirs, 1);
        }
        else{ /* Load default HRIRs: */
            pData->binConfig.hrirs = realloc1d(pData->binConfig.hrirs, __default_N_hrir_dirs*NUM_EARS*__default_hrir_len*sizeof(float));
            cblas_scopy(__default_N_hrir_dirs*NUM_EARS*__default_hrir_len, (float*)__default_hrirs, 1, pData->binConfig.hrirs, 1);
            pData->binConfig.hrir_dirs_deg = realloc1d(pData->binConfig.hrir_dirs_deg, __default_N_hrir_dirs*2*sizeof(float));
            cblas_scopy(__default_N_hrir_dirs*2, (float*)__default_hrir_dirs_deg, 1, pData->binConfig.hrir_dirs_deg, 1);
            pData->binConfig.nHRIR = __default_N_hrir_dirs;
            pData->binConfig.lHRIR = __default_hrir_len;
            pData->binConfig.hrir_fs = __default_hrir_fs;
            pData->useDefaultHRIRsFLAG = 1;
        }
        hades_synthesis_destroy(&(pData->hSyn));
        hades_synthesis_create(&(pData->hSyn), pData->hAna, beamOpt, pData->enableCovMatching, pData->refsensor_idx, &pData->binConfig, HADES_HRTF_INTERP_NEAREST);
        *hades_synthesis_getSynthesisAveragingCoeffPtr(pData->hSyn) = 0.77f;

        /* Parameter radial editor */
        hades_radial_editor_destroy(&(pData->hREd));
        hades_radial_editor_create(&(pData->hREd), pData->hAna);

        /* All went OK */
        pData->MAIR_SOFA_isLoadedFLAG = 1;
    }
    else /* Bypass audio, try to load a valid SOFA file instead: */
        pData->MAIR_SOFA_isLoadedFLAG = 0;
    saf_sofa_close(&sofa);

    /* Load previous internal settings (if not first init, and nBands is the same) */
    if(load_prevFLAG && (nBands==hades_analysis_getNbands(pData->hAna))){
        tmp = hades_synthesis_getEqPtr(pData->hSyn, &nBands);
        memcpy(tmp, eq, nBands*sizeof(float));
        tmp = hades_synthesis_getStreamBalancePtr(pData->hSyn, NULL);
        memcpy(tmp, streamBalance, nBands*sizeof(float));
    }
 
    /* Local copy of internal parameter vectors (for optional thread-safe GUI plotting) */
    if(!load_prevFLAG || (hades_analysis_getNbands(pData->hAna)!=pData->nBands_local)){
        /* If first init... Or nBands has changed */
        pData->nBands_local = hades_analysis_getNbands(pData->hAna);
        pData->freqVector_local = realloc1d(pData->freqVector_local, pData->nBands_local*sizeof(float));
        pData->streamBalBands_local = realloc1d(pData->streamBalBands_local, pData->nBands_local*sizeof(float));
    }
    tmp = (float*)hades_analysis_getFrequencyVectorPtr(pData->hAna, NULL);
    memcpy(pData->freqVector_local, tmp, pData->nBands_local*sizeof(float));
    tmp = hades_synthesis_getStreamBalancePtr(pData->hSyn, NULL);
    memcpy(pData->streamBalBands_local, tmp, pData->nBands_local*sizeof(float));

    /* done! */
    strcpy(pData->progressBarText,"Done!");
    pData->progressBar0_1 = 1.0f;
    pData->codecStatus = CODEC_STATUS_INITIALISED;
    free(eq);
    free(streamBalance);
}

void hades_renderer_process
(
    void  *  const hHdR,
    float ** const inputs,
    float ** const outputs,
    int            nInputs,
    int            nOutputs,
    int            nSamples
)
{
    hades_renderer_data *pData = (hades_renderer_data*)(hHdR);
    int ch, nMics;

    /* Local copies of parameters */
    nMics = pData->nMics;
    
    /* Process Frame if everything is ready */
    if ((nSamples == FRAME_SIZE) && (pData->codecStatus == CODEC_STATUS_INITIALISED) && pData->MAIR_SOFA_isLoadedFLAG) {
        pData->procStatus = PROC_STATUS_ONGOING;

        /* Load time-domain data */
        for(ch=0; ch < SAF_MIN(nMics, nInputs); ch++)
            utility_svvcopy(inputs[ch], FRAME_SIZE, pData->inputFrameTD[ch]);
        for(; ch<nMics; ch++)
            memset(pData->inputFrameTD[ch], 0, FRAME_SIZE * sizeof(float)); /* fill remaining channels with zeros */

        /* Apply hades analysis */
        hades_analysis_apply(pData->hAna, pData->inputFrameTD, nMics, FRAME_SIZE, pData->hPCon, pData->hSCon);

        /* Apply the hades parameter radial editor */
        hades_radial_editor_apply(pData->hREd, pData->hPCon, pData->dirGain_dB);

        /* Apply hades synthesis */
        hades_synthesis_apply(pData->hSyn, pData->hPCon, pData->hSCon, NUM_EARS, FRAME_SIZE, pData->outputFrameTD);

        /* Copy to output */
        for(ch=0; ch<SAF_MIN(NUM_EARS,nOutputs); ch++)
            memcpy(outputs[ch], pData->outputFrameTD[ch], FRAME_SIZE*sizeof(float));
    }
    else{
        /* output zero if one of the pre-requrisite conditions are not met */
        for(ch=0; ch<nOutputs; ch++)
            memset(outputs[ch], 0, FRAME_SIZE*sizeof(float));
    }

    pData->procStatus = PROC_STATUS_NOT_ONGOING;
}
    
/* Set Functions */
    
void hades_renderer_refreshSettings(void* const hHdR)
{
    hades_renderer_setCodecStatus(hHdR, CODEC_STATUS_NOT_INITIALISED);
}

void hades_renderer_setDoAestimator(void* const hHdR, HADES_RENDERER_DOA_ESTIMATORS newEstimator)
{
    hades_renderer_data *pData = (hades_renderer_data*)(hHdR);
    if(newEstimator!=pData->doaOption){
        pData->doaOption = newEstimator;
        hades_renderer_setCodecStatus(hHdR, CODEC_STATUS_NOT_INITIALISED);
    }
}

void hades_renderer_setDiffusenessEstimator(void* const hHdR, HADES_RENDERER_DIFFUSENESS_ESTIMATORS newEstimator)
{
    hades_renderer_data *pData = (hades_renderer_data*)(hHdR);
    if(newEstimator!=pData->diffOption){
        pData->diffOption = newEstimator;
        hades_renderer_setCodecStatus(hHdR, CODEC_STATUS_NOT_INITIALISED);
    }
}

void hades_renderer_setBeamformer(void* const hHdR, HADES_RENDERER_BEAMFORMER_TYPE newType)
{
    hades_renderer_data *pData = (hades_renderer_data*)(hHdR);
    if(newType!=pData->beamOption){
        pData->beamOption = newType;
        hades_renderer_setCodecStatus(hHdR, CODEC_STATUS_NOT_INITIALISED);
    }
}

void hades_renderer_setEnableCovMatching(void* const hHdR, int newState)
{
    hades_renderer_data *pData = (hades_renderer_data*)(hHdR);
    if(newState!=pData->enableCovMatching){
        pData->enableCovMatching = newState;
        hades_renderer_setCodecStatus(hHdR, CODEC_STATUS_NOT_INITIALISED);
    }
}

void hades_renderer_setAnalysisAveraging(void* const hHdR, float newValue)
{
    hades_renderer_data *pData = (hades_renderer_data*)(hHdR);
    *hades_analysis_getCovarianceAvagingCoeffPtr(pData->hAna) = newValue;
}

void hades_renderer_setSynthesisAveraging(void* const hHdR, float newValue)
{
    hades_renderer_data *pData = (hades_renderer_data*)(hHdR);
    *hades_synthesis_getSynthesisAveragingCoeffPtr(pData->hSyn) = newValue;
}

void hades_renderer_setReferenceSensorIndex(void* const hHdR, int leftOrRight, int newIndex)
{
    hades_renderer_data *pData = (hades_renderer_data*)(hHdR);
    saf_assert(leftOrRight==0 || leftOrRight==1, "Must be 0 or 1");
    saf_assert(newIndex < pData->nMics, "Index must not exceed the number of mics");
    if(newIndex!=pData->refsensor_idx[leftOrRight]){
        pData->refsensor_idx[leftOrRight] = newIndex;
        hades_renderer_setCodecStatus(hHdR, CODEC_STATUS_NOT_INITIALISED);
    }
}

void hades_renderer_setStreamBalanceFromLocal(void* const hHdR)
{
    hades_renderer_data *pData = (hades_renderer_data*)(hHdR);
    int nBands;
    float* streamBalance;
    streamBalance = hades_synthesis_getStreamBalancePtr(pData->hSyn, &nBands);
    if(nBands==pData->nBands_local && streamBalance != NULL && pData->streamBalBands_local != NULL)
        memcpy(streamBalance, pData->streamBalBands_local, nBands*sizeof(float));
}

void hades_renderer_setStreamBalance(void * const hHdR, float newValue, int bandIdx)
{
    hades_renderer_data *pData = (hades_renderer_data*)(hHdR);
    int nBands;
    float* streamBalance;
    streamBalance = hades_synthesis_getStreamBalancePtr(pData->hSyn, &nBands);
    if(bandIdx>=nBands-1 || streamBalance == NULL)
        return;
    streamBalance[bandIdx] = newValue;
    pData->streamBalBands_local[bandIdx] = newValue;
}

void hades_renderer_setStreamBalanceAllBands(void * const hHdR, float newValue)
{
    hades_renderer_data *pData = (hades_renderer_data*)(hHdR);
    int nBands, band;
    float* streamBalance;
    streamBalance = hades_synthesis_getStreamBalancePtr(pData->hSyn, &nBands);
    for(band=0; band<nBands; band++){ /* nBands==0 when streamBalance==NULL */
        streamBalance[band] = newValue;
        pData->streamBalBands_local[band] = newValue;
    }
}

void hades_renderer_setSofaFilePathMAIR(void* const hHdR, const char* path)
{
    hades_renderer_data *pData = (hades_renderer_data*)(hHdR);
    pData->sofa_filepath_MAIR = realloc1d(pData->sofa_filepath_MAIR, strlen(path) + 1);
    strcpy(pData->sofa_filepath_MAIR, path);
    hades_renderer_setCodecStatus(hHdR, CODEC_STATUS_NOT_INITIALISED);
}

void hades_renderer_setUseDefaultHRIRsflag(void* const hHdR, int newState)
{
    hades_renderer_data *pData = (hades_renderer_data*)(hHdR);
    if((!pData->useDefaultHRIRsFLAG) && (newState)){
        pData->useDefaultHRIRsFLAG = newState;
        hades_renderer_setCodecStatus(hHdR, CODEC_STATUS_NOT_INITIALISED);
    }
}

void hades_renderer_setSofaFilePathHRIR(void* const hHdR, const char* path)
{
    hades_renderer_data *pData = (hades_renderer_data*)(hHdR);
    pData->sofa_filepath_HRIR = realloc1d(pData->sofa_filepath_HRIR, strlen(path) + 1);
    strcpy(pData->sofa_filepath_HRIR, path);
    hades_renderer_setCodecStatus(hHdR, CODEC_STATUS_NOT_INITIALISED);
}


/* Get Functions */

int hades_renderer_getFrameSize(void)
{
    return FRAME_SIZE;
}

HADES_CODEC_STATUS hades_renderer_getCodecStatus(void* const hHdR)
{
    hades_renderer_data *pData = (hades_renderer_data*)(hHdR);
    return pData->codecStatus;
}

float hades_renderer_getProgressBar0_1(void* const hHdR)
{
    hades_renderer_data *pData = (hades_renderer_data*)(hHdR);
    return pData->progressBar0_1;
}

void hades_renderer_getProgressBarText(void* const hHdR, char* text)
{
    hades_renderer_data *pData = (hades_renderer_data*)(hHdR);
    memcpy(text, pData->progressBarText, HADES_PROGRESSBARTEXT_CHAR_LENGTH*sizeof(char));
}

HADES_RENDERER_DOA_ESTIMATORS hades_renderer_getDoAestimator(void* const hHdR)
{
    hades_renderer_data *pData = (hades_renderer_data*)(hHdR);
    return pData->doaOption;
}

HADES_RENDERER_DIFFUSENESS_ESTIMATORS hades_renderer_getDiffusenessEstimator(void* const hHdR)
{
    hades_renderer_data *pData = (hades_renderer_data*)(hHdR);
    return pData->diffOption;
}

HADES_RENDERER_BEAMFORMER_TYPE hades_renderer_getBeamformer(void* const hHdR)
{
    hades_renderer_data *pData = (hades_renderer_data*)(hHdR);
    return pData->beamOption;
}

int hades_renderer_getEnableCovMatching(void* const hHdR)
{
    hades_renderer_data *pData = (hades_renderer_data*)(hHdR);
    return pData->enableCovMatching;
}

float hades_renderer_getAnalysisAveraging(void* const hHdR)
{
    hades_renderer_data *pData = (hades_renderer_data*)(hHdR);
    return *hades_analysis_getCovarianceAvagingCoeffPtr(pData->hAna);
}

float hades_renderer_getSynthesisAveraging(void* const hHdR)
{
    hades_renderer_data *pData = (hades_renderer_data*)(hHdR);
    return *hades_synthesis_getSynthesisAveragingCoeffPtr(pData->hSyn);
}

int hades_renderer_getReferenceSensorIndex(void* const hHdR, int leftOrRight)
{
    hades_renderer_data *pData = (hades_renderer_data*)(hHdR);
    saf_assert(leftOrRight==0 || leftOrRight==1, "Must be 0 or 1");
    return pData->refsensor_idx[leftOrRight];
}

float hades_renderer_getStreamBalance(void* const hHdR, int bandIdx)
{
    hades_renderer_data *pData = (hades_renderer_data*)(hHdR);
    int nBands;
    float* streamBalance;
    streamBalance = hades_synthesis_getStreamBalancePtr(pData->hSyn, &nBands);
    if(bandIdx>=nBands-1)
        return 0.0f;
    else
        return streamBalance == NULL ? 0.0f : streamBalance[bandIdx];
}

float hades_renderer_getStreamBalanceAllBands(void* const hHdR)
{
    hades_renderer_data *pData = (hades_renderer_data*)(hHdR);
    float* streamBalance;
    streamBalance = hades_synthesis_getStreamBalancePtr(pData->hSyn, NULL);
    return streamBalance == NULL ? 0.0f : streamBalance[0];
}
    
void hades_renderer_getStreamBalanceLocalPtrs
(
    void* const hHdR,
    float** pX_vector,
    float** pY_values,
    int* pNpoints
)
{
    hades_renderer_data *pData = (hades_renderer_data*)(hHdR);
    float* tmp;

    (*pNpoints) = pData->nBands_local;
    (*pX_vector) = pData->freqVector_local;
    if(pData->hSyn!=NULL){
        tmp = hades_synthesis_getStreamBalancePtr(pData->hSyn, NULL);
        memcpy(pData->streamBalBands_local, tmp, pData->nBands_local*sizeof(float));
    }
    (*pY_values) = pData->streamBalBands_local;
}

void hades_renderer_getRadialEditorPtr
(
    void* const hHdR,
    float** pDirGain_dB
)
{
    hades_renderer_data *pData = (hades_renderer_data*)(hHdR);
    (*pDirGain_dB) = &(pData->dirGain_dB[0]);
}

int hades_renderer_getNumberOfBands(void* const hHdR)
{
    hades_renderer_data *pData = (hades_renderer_data*)(hHdR);
    return pData->hAna == NULL ? 0 : hades_analysis_getNbands(pData->hAna);
}

int hades_renderer_getNmicsArray(void* const hHdR)
{
    hades_renderer_data *pData = (hades_renderer_data*)(hHdR);
    return pData->nMics;
}

int hades_renderer_getNDirsArray(void* const hHdR)
{
    hades_renderer_data *pData = (hades_renderer_data*)(hHdR);
    return pData->nDirs;
}

int hades_renderer_getIRlengthArray(void* const hHdR)
{
    hades_renderer_data *pData = (hades_renderer_data*)(hHdR);
    return pData->IRlength;
}

int hades_renderer_getIRsamplerateArray(void* const hHdR)
{
    hades_renderer_data *pData = (hades_renderer_data*)(hHdR);
    return pData->IR_fs;
}

int hades_renderer_getNDirsBin(void* const hHdR)
{
    hades_renderer_data *pData = (hades_renderer_data*)(hHdR);
    return pData->binConfig.nHRIR;
}

int hades_renderer_getIRlengthBin(void* const hHdR)
{
    hades_renderer_data *pData = (hades_renderer_data*)(hHdR);
    return pData->binConfig.lHRIR;
}

int hades_renderer_getIRsamplerateBin(void* const hHdR)
{
    hades_renderer_data *pData = (hades_renderer_data*)(hHdR);
    return pData->binConfig.hrir_fs;
}

int hades_renderer_getDAWsamplerate(void* const hHdR)
{
    hades_renderer_data *pData = (hades_renderer_data*)(hHdR);
    return (int)pData->fs;
}

char* hades_renderer_getSofaFilePathMAIR(void* const hHdR)
{
    hades_renderer_data *pData = (hades_renderer_data*)(hHdR);
    if(pData->sofa_filepath_MAIR!=NULL)
        return pData->sofa_filepath_MAIR;
    else
        return "no_file";
}

int hades_renderer_getUseDefaultHRIRsflag(void* const hHdR)
{
    hades_renderer_data *pData = (hades_renderer_data*)(hHdR);
    return pData->useDefaultHRIRsFLAG;
}

char* hades_renderer_getSofaFilePathHRIR(void* const hHdR)
{
    hades_renderer_data *pData = (hades_renderer_data*)(hHdR);
    if(pData->sofa_filepath_HRIR!=NULL)
        return pData->sofa_filepath_HRIR;
    else
        return "no_file";
}

int hades_renderer_getProcessingDelay(void* const hHdR)
{
    hades_renderer_data *pData = (hades_renderer_data*)(hHdR);
    return hades_analysis_getProcDelay(pData->hAna)+hades_synthesis_getProcDelay(pData->hSyn);
} 
