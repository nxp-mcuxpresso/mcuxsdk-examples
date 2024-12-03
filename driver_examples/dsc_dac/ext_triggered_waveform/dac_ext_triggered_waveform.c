/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_dac.h"
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

int main(void)
{
    dac_config_t sDacConfig;

    BOARD_InitHardware();

    /* In this demo, an external clock signal with 1kHz is necessary, which can refresh the DAC buffer register
     * every step. In this sawtooth waveform, the MaxValue is 4000, MinValue is 0, and StepSize is 40, that means
     * every cycle of the waveform should refresh 100 times, thus we can measure a 10 Hz sawtooth waveform on DAC
     * output pin*/
    PRINTF("\r\nDAC waveform generator Example.\r\n");
    PRINTF("Please input a 1 kHz external clock signal.\r\n");

    DAC_GetDefaultConfig(&sDacConfig);
    sDacConfig.eOperationMode                                      = kDAC_AutomaticOperationMode;
    sDacConfig.eSyncSignal                                         = kDAC_ExternalSyncInSignal;
    sDacConfig.uOperationConfig.sAutomaticModeConfig.eWaveformType = kDAC_RepeatSawtoothWaveform0;
    sDacConfig.uOperationConfig.sAutomaticModeConfig.u16MaxValue   = 4000U;
    sDacConfig.uOperationConfig.sAutomaticModeConfig.u16MinValue   = 0U;
    sDacConfig.uOperationConfig.sAutomaticModeConfig.u16StartValue = 0U;
    sDacConfig.uOperationConfig.sAutomaticModeConfig.u16StepSize   = 40U;
    sDacConfig.bEnableAnalogPortion                                = true;
    DAC_Init(DEMO_DAC_BASE, &sDacConfig);

    PRINTF("A 10Hz sawtooth waveform can be viewed on the Oscilloscope.\r\n");
    while (1)
    {
    }
}
