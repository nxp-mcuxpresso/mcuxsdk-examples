/*
 * Copyright 2026 NXP
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
#include "board.h"
#include "ld_support.h"
#include "fsl_debug_console.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

void LD_CopyImage(uint32_t addr)
{
    memcpy((void *)addr, (void *)LD_IMAGE_DATA_START, LD_IMAGE_DATA_SIZE);
}

void BOARD_LD_Init(void)
{
#if LD_IMAGE_COPY_TO_RAM

#if defined(__ICCARM__)
#pragma section = "__ld_data_section"
#elif defined(__GNUC__)
#endif
#endif
#if LD_IMAGE_COPY_TO_RAM
    /* Copy LD application to SSRAM. */
    SDK_DelayAtLeastUs(100000, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    LD_CopyImage((uint32_t)LD_SRAM_ADDRESS);
    PRINTF("Finish copy localdimming firmware\r\n");
#endif
}
