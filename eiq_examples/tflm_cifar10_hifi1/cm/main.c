/*
 * Copyright 2021-2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "app.h"
#include "fsl_debug_console.h"

/*!
 * @brief Main function
 */
int main(void)
{
    /* Initialize standard SDK demo application pins */
    BOARD_InitHardware();

    /* Print the initial banner */
    PRINTF("\r\nStarting Core1 example from Cortex-M33 core0\r\n");

    for (;;)
        ;
}

