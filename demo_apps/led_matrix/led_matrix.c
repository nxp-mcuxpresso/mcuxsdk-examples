/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"
#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
int main(void)
{
    uint32_t i;

    /* Init board hardware. */
    BOARD_InitHardware();

    while (1)
    {
        for (i = 0; i < 10; i++)
        {
            BOARD_LedMatrixShowNumber(i, 0x60, false);
        }
    }
}
