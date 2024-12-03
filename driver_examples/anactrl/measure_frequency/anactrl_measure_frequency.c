/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "app.h"
#include "board.h"
#include "fsl_anactrl.h"
#include "fsl_debug_console.h"
#include "fsl_inputmux.h"
#include "fsl_inputmux_connections.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Main function.
 */
int main(void)
{
    uint32_t targetClkFreq = 0U;

    BOARD_InitHardware();

    /* Init inputmux. */
    INPUTMUX_Init(DEMO_INPUTMUX);

    /* Init analog control. */
    ANACTRL_Init(DEMO_ANACTRL);

    /* Set the reference and target clock source. */
    INPUTMUX_AttachSignal(DEMO_INPUTMUX, 0U, DEMO_REFERENCE_CLOCK);
    INPUTMUX_AttachSignal(DEMO_INPUTMUX, 0U, DEMO_TARGET_CLOCK);

    PRINTF("Analog control measure frequency example.\r\n");

    /* Start Measurement. */
    targetClkFreq = ANACTRL_MeasureFrequency(DEMO_ANACTRL, DEMO_SCALE, DEMO_REFERENCE_CLOCK_FREQUENCY);
    PRINTF(DEMO_REFERENCE_CLOCK_SOURCE);
    PRINTF(DEMO_TARGET_CLOCK_SOURCE);
    PRINTF("\r\nTarget clock frequency: %d Hz.\r\n", targetClkFreq);

    while (1)
    {
    }
}
