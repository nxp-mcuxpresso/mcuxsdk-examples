/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "fsl_inputmux.h"
#include "board.h"
#include "app.h"
#include "fsl_fmeas.h"

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
 * @brief    Measurement cycle with value display
 *
 * @param    srcName : Name of the measured target clock
 * @param    target  : Target (measured) clock
 * @param    freqRef : Reference frequency [Hz]
 * @param    freqExp : Expected frequency [Hz]
 *
 * @return   Measured frequency [Hz]
 */
static uint32_t MeasureDisplay(char *srcName, inputmux_connection_t target, uint32_t freqRef, uint32_t freqExp)
{
    uint32_t freqComp;

    /* Setup to measure the selected target */
    INPUTMUX_AttachSignal(INPUTMUX, EXAMPLE_TARGET_CLOCK_REGISTRY_INDEX, target);

    /* Start a measurement cycle and wait for it to complete. If the target
       clock is not running, the measurement cycle will remain active
       forever, so a timeout may be necessary if the target clock can stop */
    FMEAS_StartMeasure(FMEAS_SYSCON);
    while (!FMEAS_IsMeasureComplete(FMEAS_SYSCON))
    {
    }

    /* Get computed frequency */
    freqComp = FMEAS_GetFrequency(FMEAS_SYSCON, freqRef);

    /* Show the raw capture value and the compute frequency */
    PRINTF("Capture source: %s, reference frequency = %u Hz\r\n", srcName, freqRef);
    PRINTF("Computed frequency value = %u Hz\r\n", freqComp);
    PRINTF("Expected frequency value = %u Hz\r\n\r\n", freqExp);

    return freqComp;
}

/*!
 * @brief Main function
 */
int main(void)
{
    /* Init board hardware. */
    BOARD_InitHardware();

    uint32_t freqRef = EXAMPLE_REFERENCE_CLOCK_FREQUENCY;

    INPUTMUX_AttachSignal(INPUTMUX, EXAMPLE_REFERENCE_CLOCK_REGISTRY_INDEX, EXAMPLE_REFERENCE_CLOCK);

    /* Start frequency measurement 1 and display results */
    MeasureDisplay(EXAMPLE_TARGET_CLOCK_1_NAME " (" EXAMPLE_REFERENCE_CLOCK_NAME " reference)", EXAMPLE_TARGET_CLOCK_1,
                   freqRef, EXAMPLE_TARGET_CLOCK_1_FREQUENCY);

    /* Start frequency measurement 2 and display results */
    MeasureDisplay(EXAMPLE_TARGET_CLOCK_2_NAME " (" EXAMPLE_REFERENCE_CLOCK_NAME " reference)", EXAMPLE_TARGET_CLOCK_2,
                   freqRef, EXAMPLE_TARGET_CLOCK_2_FREQUENCY);

    /* Start frequency measurement 3 and display results */
    MeasureDisplay(EXAMPLE_TARGET_CLOCK_3_NAME " (" EXAMPLE_REFERENCE_CLOCK_NAME " reference)", EXAMPLE_TARGET_CLOCK_3,
                   freqRef, EXAMPLE_TARGET_CLOCK_3_FREQUENCY);

    while (1)
    {
    }
}
