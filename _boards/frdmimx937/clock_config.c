/*
 * Copyright 2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "clock_config.h"

/*******************************************************************************
 ************************ BOARD_InitBootClocks function ************************
 ******************************************************************************/
void BOARD_InitBootClocks(void)
{
    BOARD_BootClockRUN();
}

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* clang-format off */

/* clang-format on */

/*******************************************************************************
 * Code
 ******************************************************************************/
static void BOARD_InitClock(void)
{
    clock_ip_name_t src_idx = kCLOCK_ext;
    for (; src_idx < CLOCK_NUM_SRC; src_idx++)
    {
        CLOCK_GetRate(src_idx);
    }
}

void BOARD_BootClockRUN(void)
{
    BOARD_InitClock();
}
