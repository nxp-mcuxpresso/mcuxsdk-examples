/*
 * Copyright 2021-2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "app.h"
#include "dsp_support.h"
#include "fsl_debug_console.h"

/*!
 * @brief Main function
 */
int main(void)
{
    /* Initialize standard SDK demo application pins */
    BOARD_InitHardware();

    /* Print the initial banner */
    PRINTF("\r\nStarting HiFi1 example from Cortex-M33 core\r\n");

    /* Copy DSP image to RAM and start DSP core. */
    BOARD_DSP_Init();

    for (;;)
        ;
}

