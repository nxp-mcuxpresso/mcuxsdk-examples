/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"
#include "fsl_ce.h"
#include "fsl_mu.h"
#include "ce_support.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define CE_IMAGE_START     ce_fw
#define CE_IMAGE_SIZE      sizeof(ce_fw)
#define CE_STCM_ADDRESSESS CE_STCM5_BASE

/*******************************************************************************
 * Variables
 ******************************************************************************/
unsigned char ce_fw[] = {
#include "ce_kw47_mcxw72.txt"
};
/*******************************************************************************
 * Code
 ******************************************************************************/
void BOARD_CE_Init(void)
{
    ce_copy_image_t ce_image;

    ce_image.srcAddr  = (uint32_t)CE_IMAGE_START;
    ce_image.size     = CE_IMAGE_SIZE;
    ce_image.destAddr = CE_STCM_ADDRESSESS;

    CE_Init(&ce_image);
}
