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
 * @file ehades.h
 * @brief Using HADES to go from microphone array direct to binaural with some
 *        optional enhancement processing inbetween the analysis and synthesis
 *
 * @see [1] Fernandez, J., McCormack, L., Hyvärinen, P., Politis, A., and
 *          Pulkki, V. 2022. “Enhancing binaural rendering of head-worn
 *          microphone arrays through the use of adaptive spatial covariance
 *          matching”, The Journal of the Acoustical Society of America 151,
 *          2624-2635
 *
 * @author Janani Fernandez & Leo McCormack
 * @date 09.04.2021
 * @license GNU GPLv2
 */

#ifndef __EHADES_H_INCLUDED__
#define __EHADES_H_INCLUDED__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* ========================================================================== */
/*                   HADES Renderer Configurations Options                    */
/* ========================================================================== */

/**
 * Options for source number estimation (also known as "detection" in array
 * processing literature) for hades
 */
typedef enum {
    HADES_RENDERER_USE_COMEDIE = 1    /**< Array diffuseness measure */
}HADES_RENDERER_DIFFUSENESS_ESTIMATORS;

/** Options for DoA estimation for hades */
typedef enum {
    HADES_RENDERER_USE_MUSIC = 1      /**< Use MUSIC */
}HADES_RENDERER_DOA_ESTIMATORS;

/** Beamforming options for hades */
typedef enum {
    HADES_RENDERER_BEAMFORMER_NONE = 1,       /**< No beamforming */
    HADES_RENDERER_BEAMFORMER_FILTER_AND_SUM, /**< Filter-and-sum beamforming */
    HADES_RENDERER_BEAMFORMER_BMVDR           /**< Binaural minimum-variance
                                               *   distortion-less response
                                               *   (MVDR) beamforming */
}HADES_RENDERER_BEAMFORMER_TYPE;

/**
 * Current status of the codec
 *
 * These can be used to find out whether the codec is initialised, currently
 * in the process of intialising, or it is not yet initialised.
 */
typedef enum {
    CODEC_STATUS_INITIALISED = 0, /**< Codec is initialised and ready to process
                                   *   input audio. */
    CODEC_STATUS_NOT_INITIALISED, /**< Codec has not yet been initialised, or
                                   *   the codec configuration has changed.
                                   *   Input audio should not be processed. */
    CODEC_STATUS_INITIALISING     /**< Codec is currently being initialised,
                                   *   input audio should not be processed. */
} HADES_CODEC_STATUS;

/** Length of progress bar string */
#define HADES_PROGRESSBARTEXT_CHAR_LENGTH ( 256 )

/** Maximum number of input/output channels supported */
#define HADES_MAX_NUM_CHANNELS ( 64 )

/** Maximum number of input channels supported */
#define HADES_MAX_NUM_INPUTS ( HADES_MAX_NUM_CHANNELS )

/** Maximum number of output channels supported */
#define HADES_MAX_NUM_OUTPUTS ( HADES_MAX_NUM_CHANNELS )

/* ========================================================================== */
/*                               Main Functions                               */
/* ========================================================================== */

/**
 * Creates an instance of the mighty hades_renderer
 *
 * @param[in] phHdR (&) address of hades_renderer handle
 */
void hades_renderer_create(void** const phHdR);

/**
 * Destroys an instance of the mighty hades_renderer
 *
 * @param[in] phHdR (&) address of hades_renderer handle
 */
void hades_renderer_destroy(void** const phHdR);

/**
 * Initialises an instance of hades_renderer
 *
 * @param[in] hHdR       hades_renderer handle
 * @param[in] samplerate Host samplerate.
 */
void hades_renderer_init(void* const hHdR,
                         int samplerate);
    
/**
 * Intialises the compass codecs based on current global/user parameters
 *
 * @param[in] hHdR hades_renderer handle
 */
void hades_renderer_initCodec(void* const hHdR);

/**
 * Performs the HADES processing
 *
 * @param[in] hHdR     hades_renderer handle
 * @param[in] inputs   Input channel buffers; 2-D array: nInputs x nSamples
 * @param[in] outputs  Output channel buffers; 2-D array: nOutputs x nSamples
 * @param[in] nInputs  Number of input channels
 * @param[in] nOutputs Number of output channels
 * @param[in] nSamples Number of samples in 'inputs'/'output' matrices
 */
void hades_renderer_process(void* const hHdR,
                            float** const inputs,
                            float** const outputs,
                            int nInputs,
                            int nOutputs,
                            int nSamples);


/* ========================================================================== */
/*                                Set Functions                               */
/* ========================================================================== */

/**
 * Sets all intialisation flags to 1; i.e. re-initialise all settings/variables
 * as hades_renderer is currently configured, at next available opportunity.
 */
void hades_renderer_refreshSettings(void* const hHdR);

/** Sets the DoA estimator to use (see #HADES_RENDERER_DOA_ESTIMATORS) */
void hades_renderer_setDoAestimator(void* const hHdR,
                                    HADES_RENDERER_DOA_ESTIMATORS newEstimator);

/**
 * Sets the diffuseness estimator to use (see
 * #HADES_RENDERER_DIFFUSENESS_ESTIMATORS)
 */
void hades_renderer_setDiffusenessEstimator(void* const hHdR,
                                            HADES_RENDERER_DIFFUSENESS_ESTIMATORS newEstimator);

/**
 * Sets the beamformer to employ during rendering (see
 * #HADES_RENDERER_BEAMFORMER_TYPE)
 */
void hades_renderer_setBeamformer(void* const hHdR,
                                  HADES_RENDERER_BEAMFORMER_TYPE newType);

/** Sets whether the covariance matching should be applied (1) or not (0) */
void hades_renderer_setEnableCovMatching(void* const hHdR,
                                         int newState);

/** Sets the analysis averaging coefficient, [0..1] */
void hades_renderer_setAnalysisAveraging(void* const hHdR,
                                         float newValue);

/** Sets the synthesis averaging coefficient, [0..1] */
void hades_renderer_setSynthesisAveraging(void* const hHdR,
                                          float newValue);

/**
 * Sets the reference sensor index [0..63], for either the left (0) or right
 * (1) side */
void hades_renderer_setReferenceSensorIndex(void* const hHdR,
                                            int leftOrRight,
                                            int newIndex);

/**
 * Copies the stream balance values from local, to the internal compass config
 */
void hades_renderer_setStreamBalanceFromLocal(void* const hHdR);

/**
 * Sets the balance between direct and ambient streams (default=1, 50%/50%) for
 * ONE specific frequency band.
 *
 * @param[in] hHdR     hades_renderer handle
 * @param[in] newValue New balance, 0: fully ambient, 1: balanced, 2: fully
 *                     direct
 * @param[in] bandIdx  Frequency band index
 */
void hades_renderer_setStreamBalance(void* const hHdR,
                                     float newValue,
                                     int bandIdx);

/**
 * Sets the balance between direct and ambient streams (default=1, 50%/50%) for
 * ALL frequency bands.
 *
 * @param[in] hHdR     hades_renderer handle
 * @param[in] newValue new balance, 0: fully ambient, 1: balanced, 2: fully
 *                     direct
 */
void hades_renderer_setStreamBalanceAllBands(void* const hHdR,
                                             float newValue);

/**
 * Sets the file path for a .sofa file
 *
 * @param[in] hHdR       hades_renderer handle
 * @param[in] path       File path to .sofa file (WITH file extension)
 */
void hades_renderer_setSofaFilePathMAIR(void* const hHdR, const char* path);

/**
 * Sets flag to dictate whether the default HRIRs in the Spatial_Audio_Framework
 * should be used, or a custom HRIR set loaded via a SOFA file.
 * Note: if the custom set failes to load correctly, hades_renderer will revert to the
 * defualt set. Use hades_renderer_getUseDefaultHRIRsflag() to check if loading was
 * successful.
 *
 * @param[in] hHdR       hades_renderer handle
 * @param[in] newState   0: use custom HRIR set, 1: use default HRIR set
 */
void hades_renderer_setUseDefaultHRIRsflag(void* const hHdR, int newState);


/**
 * Sets the file path for a .sofa file
 *
 * @param[in] hHdR       hades_renderer handle
 * @param[in] path       File path to .sofa file (WITH file extension)
 */
void hades_renderer_setSofaFilePathHRIR(void* const hHdR, const char* path);


/* ========================================================================== */
/*                                Get Functions                               */
/* ========================================================================== */

/**
 * Returns the processing framesize (i.e., number of samples processed with
 * every _process() call )
 */
int hades_renderer_getFrameSize(void);

/**
 * Returns current codec status (see #HADES_CODEC_STATUS enum)
 */
HADES_CODEC_STATUS hades_renderer_getCodecStatus(void* const hHdR);

/**
 * Returns current intialisation/processing progress, between 0..1
 *  - 0: intialisation/processing has started
 *  - 1: intialisation/processing has ended
 */
float hades_renderer_getProgressBar0_1(void* const hHdR);

/**
 * Returns current intialisation/processing progress text
 *
 * @warning "text" string should be (at least) of length:
 *          #HADES_PROGRESSBARTEXT_CHAR_LENGTH
 *
 * @param[in]  hHdR hades_renderer handle
 * @param[out] text Process bar text; #HADES_PROGRESSBARTEXT_CHAR_LENGTH x 1
 */
void hades_renderer_getProgressBarText(void* const hHdR, char* text);

/** Returns the DoA estimator being used (see #HADES_RENDERER_DOA_ESTIMATORS) */
HADES_RENDERER_DOA_ESTIMATORS hades_renderer_getDoAestimator(void* const hHdR);

/**
 * Returns the diffuseness estimator being used (see
 * #HADES_RENDERER_DIFFUSENESS_ESTIMATORS)
 */
HADES_RENDERER_DIFFUSENESS_ESTIMATORS hades_renderer_getDiffusenessEstimator(void* const hHdR);

/**
 * Returns the beamformer type currently being used during rendering (see
 * #HADES_RENDERER_BEAMFORMER_TYPE)
 */
HADES_RENDERER_BEAMFORMER_TYPE hades_renderer_getBeamformer(void* const hHdR);

/** Returns whether the covariance matching is being applied (1) or not (0) */
int hades_renderer_getEnableCovMatching(void* const hHdR);

/** Returns the analysis averaging coefficient, [0..1] */
float hades_renderer_getAnalysisAveraging(void* const hHdR);

/** Returns the synthesis averaging coefficient, [0..1] */
float hades_renderer_getSynthesisAveraging(void* const hHdR);

/**
 * Returns the reference sensor index [0..63], for either the left (0) or right
 * (1) side */
int hades_renderer_getReferenceSensorIndex(void* const hHdR,
                                           int leftOrRight);

/**
 * Returns the balance between direct and ambient streams (default=1, 50%/50%)
 * for ONE specific frequency band
 *
 * @param[in] hHdR    hades_renderer handle
 * @param[in] bandIdx Frequency band index
 * @returns The current balance value, 0: fully ambient, 1: balanced, 2: fully
 *          direct
 */
float hades_renderer_getStreamBalance(void* const hHdR,
                                      int bandIdx);
    
/**
 * Returns the balance between direct and ambient streams (default=1, 50%/50%)
 * for the FIRST frequency band
 *
 * @param[in] hHdR hades_renderer handle
 * @returns Current balance, 0: fully ambient, 1: balanced, 2: fully direct
 */
float hades_renderer_getStreamBalanceAllBands(void* const hHdR); /* returns the first value */

/**
 * Returns pointers for the balance between direct and ambient streams
 * (default=1, 50%/50%) for ALL frequency bands.
 *
 * @param[in]  hHdR      hades_renderer handle
 * @param[out] pX_vector (&) Frequency vector; pNpoints x 1
 * @param[out] pY_values (&) Balance values per frequency; pNpoints x 1
 * @param[out] pNpoints  (&) Number of frequencies/balance values
 */
void hades_renderer_getStreamBalanceLocalPtrs(void* const hHdR,
                                              float** pX_vector,
                                              float** pY_values,
                                              int* pNpoints);

/**
 * Returns pointers to the radial (360 degree) parameter editor vector
 *
 * @param[in]  hHdR          hades_renderer handle
 * @param[out] pDirGain_dB   (&) Direct stream gain per direction, dB; 360 x 1
 */
void hades_renderer_getRadialEditorPtr(void* const hHdR,
                                       float** pDirGain_dB);

/**
 * Returns the number of frequency bands employed by hades_renderer
 */
int hades_renderer_getNumberOfBands(void* const hHdR);

/** Returns the number of microphones in the currently used array */
int hades_renderer_getNmicsArray(void* const hHdR);

/** Returns the number of directions in the currently used array IR set */
int hades_renderer_getNDirsArray(void* const hHdR);

/** Returns the length of the array IRs, in samples */
int hades_renderer_getIRlengthArray(void* const hHdR);

/** Returns the array IR sample rate */
int hades_renderer_getIRsamplerateArray(void* const hHdR);

/** Returns the number of directions in the currently used HRIR set */
int hades_renderer_getNDirsBin(void* const hHdR);

/** Returns the length of the HRIRs, in samples */
int hades_renderer_getIRlengthBin(void* const hHdR);

/** Returns the HRIR sample rate */
int hades_renderer_getIRsamplerateBin(void* const hHdR);

/** Returns the DAW/Host sample rate */
int hades_renderer_getDAWsamplerate(void* const hHdR);

/**
 * Returns the file path for a .sofa file
 *
 * @param[in] hHdR hades_renderer handle
 * @returns File path to .sofa file (WITH file extension)
 */
char* hades_renderer_getSofaFilePathMAIR(void* const hHdR);

/**
 * Returns the value of a flag used to dictate whether the default HRIRs in the
 * Spatial_Audio_Framework should be used, or a custom HRIR set loaded via a
 * SOFA file
 *
 * @param[in] hHdR hades_renderer handle
 * @returns 0: use custom HRIR set, 1: use default HRIR set
 */
int hades_renderer_getUseDefaultHRIRsflag(void* const hHdR);

/**
 * Returns the file path for a .sofa file
 *
 * @param[in] hHdR hades_renderer handle
 * @returns File path to .sofa file (WITH file extension)
 */
char* hades_renderer_getSofaFilePathHRIR(void* const hHdR);

/**
 * Returns the processing delay in samples (may be used for delay compensation
 * features)
 */
int hades_renderer_getProcessingDelay(void* const hHdR);


#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* __EHADES_H_INCLUDED__ */
