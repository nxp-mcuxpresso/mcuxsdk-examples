/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "clock_config.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

static void BOARD_InitClock(void)
{
    clock_ip_name_t src_idx = kCLOCK_Ext;
    for (; src_idx < CLOCK_NUM_SRC; src_idx++)
    {
        CLOCK_GetRate(src_idx);
    }
}

void BOARD_BootClockRUN(void)
{
    BOARD_InitClock();
    SystemCoreClockUpdate();
}
