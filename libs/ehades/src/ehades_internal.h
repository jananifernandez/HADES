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
 * @file ehades_internal.h
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

#ifndef __EHADES_INTERNAL_H_INCLUDED__
#define __EHADES_INTERNAL_H_INCLUDED__

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <float.h>
#include "ehades.h"
#include "saf.h"
#include "saf_externals.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * Current status of the processing loop
 *
 * These are used to keep things thread-safe. i.e., the codec will not be
 * initialised if the currently configured codec is being used to process a
 * block of audio. Likewise, if the codec is being initialised, then the
 * "process" functions are bypassed.
 */
typedef enum {
    PROC_STATUS_ONGOING = 0, /**< Codec is processing input audio, and should
                              *   not be reinitialised at this time. */
    PROC_STATUS_NOT_ONGOING  /**< Codec is not processing input audio, and may
                              *   be reinitialised if needed. */
}PROC_STATUS;


/* ========================================================================== */
/*                            Internal Parameters                             */
/* ========================================================================== */

#ifndef FRAME_SIZE
# define FRAME_SIZE ( 512 )
#endif
#define MAX_NUM_SH_SIGNALS ( (MAX_SH_ORDER + 1)*(MAX_SH_ORDER + 1)  )    /* (L+1)^2 */
#define HOP_SIZE ( 128 )
#if (FRAME_SIZE % HOP_SIZE != 0)
# error "FRAME_SIZE must be an integer multiple of HOP_SIZE"
#endif 

/* ========================================================================== */
/*                                 Structures                                 */
/* ========================================================================== */

/** Main structure for hades_renderer */
typedef struct _hades_renderer {
    /* audio buffers and afSTFT stuff */
    float** inputFrameTD;                    /**< Input frame; nInputs x FRAME_SIZE */ 
    float** outputFrameTD;                   /**< Output frame; nOutputs x FRAME_SIZE */
    float fs;                                /**< Sampling rate */

    /* Internal */
    int MAIR_SOFA_isLoadedFLAG;              /**< 0: no MAIR SOFA file has been loaded, so do not render audio; 1: SOFA file HAS been loaded */
    hades_analysis_handle hAna;              /**< Analysis handle */
    hades_synthesis_handle hSyn;             /**< Synthesis handle */
    hades_param_container_handle hPCon;      /**< Parameter Container handle */
    hades_signal_container_handle hSCon;     /**< Signal Container handle */
    hades_radial_editor_handle hREd;         /**< Parameter radial editor handle */
    HADES_CODEC_STATUS codecStatus;          /**< see #HADES_CODEC_STATUS */
    float progressBar0_1;                    /**< Progress bar value [0..1] */
    char* progressBarText;                   /**< Progress bar text; HADES_PROGRESSBARTEXT_CHAR_LENGTH x 1*/
    PROC_STATUS procStatus;                  /**< see #_PROC_STATUS */

    /* Local copy of internal parameter vectors (for optional thread-safe GUI plotting) */
    int nBands_local;                        /**< Number of bands used for plotting */
    int nDirs_local;                         /**< Number of bands */
    float* freqVector_local;                 /**< Local frequency vector; nBands_local x 1 */
    float* streamBalBands_local;             /**< Local stream balance vector; nBands_local x 1 */

    /* For the radial editor */
    float dirGain_dB[360];                   /**< extra radial gain control for the direct stream only, in dB */ 
 
    /* IR data */
    int nMics;                               /**< Number of microphones/hydrophones in the array */
    int nDirs;                               /**< Number of measurement directions/IRs */
    int IRlength;                            /**< Length of IRs, in samples */
    float IR_fs;                             /**< Sample rate used for measuring the IRs */

    /* user parameters */
    hades_binaural_config binConfig;         /**< Binaural configuration settings */
    char* sofa_filepath_MAIR;                /**< microphone array IRs; absolute/relevative file path for a sofa file */
    int useDefaultHRIRsFLAG;                 /**< 0: use specified sofa file, 1: use default HRIR set */
    char* sofa_filepath_HRIR;                /**< HRIRs; absolute/relevative file path for a sofa file */
    int refsensor_idx[2];                    /**< Indices defining the left 0 and right 1 reference sensors */
    HADES_RENDERER_DIFFUSENESS_ESTIMATORS diffOption; /**< see #HADES_RENDERER_DIFFUSENESS_ESTIMATORS */
    HADES_RENDERER_DOA_ESTIMATORS doaOption; /**< see #HADES_RENDERER_DOA_ESTIMATORS */
    HADES_RENDERER_BEAMFORMER_TYPE beamOption; /**< see #HADES_RENDERER_BEAMFORMER_TYPE */
    int enableCovMatching;                   /**< 0: disabled; 1: spatial covariance matching is enabled */
    
} hades_renderer_data;


/* ========================================================================== */
/*                             Internal Functions                             */
/* ========================================================================== */
  
/**
 * Sets codec status.
 *
 * @param[in] hHdR      hades_renderer handle
 * @param[in] newStatus Codec status (see #HADES_CODEC_STATUS enum)
 */
void hades_renderer_setCodecStatus(void* const hHdR,
                                   HADES_CODEC_STATUS newStatus);


#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* __EHADES_INTERNAL_H_INCLUDED__ */
