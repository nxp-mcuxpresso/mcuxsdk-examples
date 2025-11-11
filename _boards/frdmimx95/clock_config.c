/*
 * Copyright 2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "clock_config.h"
#include "hal_clock.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

static void BOARD_InitClock(void)
{
    hal_clk_id_e src_idx = hal_clock_ext;
    for (; src_idx < HAL_CLOCK_PLATFORM_SOURCE_NUM; src_idx++)
    {
        HAL_ClockGetRate(src_idx);
    }
}

void BOARD_BootClockRUN(void)
{
    BOARD_InitClock();
}
