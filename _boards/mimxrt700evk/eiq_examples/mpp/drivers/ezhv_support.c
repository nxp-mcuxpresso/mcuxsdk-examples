/*
 * Copyright 2023-2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_ezhv.h"
#include "ezhv_config.h"
#include "ezhv_support.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
void BOARD_EZHV_Init(void *para, uint32_t size)
{
    ezhv_copy_image_t ezhv_image;

#pragma section = "EZHV_section"
    ezhv_image.srcAddr = ZENV_IMAGE_START;
    ezhv_image.size= EZHV_IMAGE_SIZE;
    ezhv_image.destAddr = EZHV_ITCM_ADDRESSESS;

    EZHV_Init(&ezhv_image);
    EZHV_InitPara(para, size);
    EZHV_Boot(EZHV_ITCM_ADDRESSESS, NULL);
}
