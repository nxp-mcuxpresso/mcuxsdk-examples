/*
 * Copyright 2021-2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "board.h"
#include "clock_config.h"
#include "dsp_support.h"
#include "fsl_debug_console.h"
#include "pin_mux.h"

/*!
 * @brief Main function
 */
int main(void)
{
    /* Initialize standard SDK demo application pins */
    BOARD_InitAHBSC();
    BOARD_InitBootPins();
    BOARD_InitBootClocks();

    BOARD_InitDebugConsole();

    /* Print the initial banner */
    PRINTF("\r\nStarting HiFi1 example from Cortex-M33 core1\r\n");

    /* Copy DSP image to RAM and start DSP core. */
    BOARD_DSP_Init();

    for (;;)
        ;
}
