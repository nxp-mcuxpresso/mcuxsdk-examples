/*
 * Copyright 2026 NXP
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
#define CE_STCM_ADDRESSES  0x04030000

/*******************************************************************************
 * Variables
 ******************************************************************************/
#if defined(__ICCARM__)
#pragma data_alignment = 128
const unsigned char ce_fw[] = {
#elif defined(__GNUC__)
const unsigned char ce_fw[] __attribute__((aligned(128))) = {
#endif
#include "ce_kw43_mcxw70.txt"
};
/*******************************************************************************
 * Code
 ******************************************************************************/
void BOARD_CE_Init(void)
{
    ce_copy_image_t ce_image;

    ce_image.srcAddr  = (uint32_t)CE_IMAGE_START;
    ce_image.size     = CE_IMAGE_SIZE;
    /* If ce_image.destAddr is set to 0, DSP-V Lite loads firmware directly from flash.
     * To load firmware from SRAM instead, set ce_image.destAddr to CE_STCM_ADDRESSES.
     */
    ce_image.destAddr = 0;

    CE_Init(&ce_image);
}
