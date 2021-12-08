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
 * @file ehades_internal.c
 * @brief Using HADES to go from microphone array direct to binaural with some
 *        optional enhancement processing inbetween the analysis and synthesis
 * @author Janani Fernandez & Leo McCormack
 * @date 09.04.2021
 * @license GNU GPLv2
 */

#include "ehades.h"
#include "ehades_internal.h"

void hades_renderer_setCodecStatus(void* const hHdR, HADES_CODEC_STATUS newStatus)
{
    hades_renderer_data *pData = (hades_renderer_data*)(hHdR);
    if(newStatus==CODEC_STATUS_NOT_INITIALISED){
        /* Pause until current initialisation is complete */
        while(pData->codecStatus == CODEC_STATUS_INITIALISING)
            SAF_SLEEP(10);
    }
    pData->codecStatus = newStatus;
}

