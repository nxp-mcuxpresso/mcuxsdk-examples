/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_dac.h"
#include "fsl_gpio.h"
#include "fsl_xbara.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
uint16_t g_DACMinValue = DEMO_TRIANGLE_WAVEFORM_INIT_MIN_VALUE;
uint16_t g_DACMaxValue = DEMO_TRIANGLE_WAVEFORM_INIT_MAX_VALUE;

/*******************************************************************************
 * Code
 ******************************************************************************/
int main(void)
{
    dac_config_t sDacConfig;
    char cInputChar;
    uint32_t u32TmpDivider;

    BOARD_InitHardware();

    PRINTF("DAC Pin Sync Triangle Waveform Demo.\r\n");
    /*
     * Gets DAC default configuration.
     * psConfig->eOperationMode                                     = kDAC_NormalOperationMode;
     * psConfig->uOperationConfig.sNormalModeConfig.u16DataFIFO = 0U;
     * psConfig->uOperationConfig.sNormalModeConfig.bEnableDMA      = false;
     * psConfig->uOperationConfig.sNormalModeConfig.eWatermarkLevel = kDAC_WatermarkValue2;
     * psConfig->eSyncInputEdge                                     = kDAC_RisingEdge;
     * psConfig->eSpeedMode                                         = kDAC_HighSpeedMode;
     * psConfig->eDataFormat                                        = kDAC_DataWordRightJustified;
     * psConfig->eSyncSignal                                        = kDAC_InternalClockSignal;
     * psConfig->bEnableAnalogPortion                               = false;
     * psConfig->bEnableGlitchFilter                                = true;
     * psConfig->u8GlitchFilterCount                                = 29U;
     */
    DAC_GetDefaultConfig(&sDacConfig);
    sDacConfig.eOperationMode                                        = kDAC_AutomaticOperationMode;
    sDacConfig.eSyncSignal                                           = kDAC_ExternalSyncInSignal;
    sDacConfig.eSyncInputEdge                                        = kDAC_FallingEdge;
    sDacConfig.uOperationConfig.sAutomaticModeConfig.eWaveformType   = kDAC_RepeatTriangleWaveform1;
    sDacConfig.uOperationConfig.sAutomaticModeConfig.u16CompareValue = 488U;
    sDacConfig.uOperationConfig.sAutomaticModeConfig.u16MaxValue     = DEMO_TRIANGLE_WAVEFORM_INIT_MAX_VALUE;
    sDacConfig.uOperationConfig.sAutomaticModeConfig.u16MinValue     = DEMO_TRIANGLE_WAVEFORM_INIT_MIN_VALUE;
    sDacConfig.uOperationConfig.sAutomaticModeConfig.u16StartValue   = DEMO_TRIANGLE_WAVEFORM_INIT_MAX_VALUE / 2U;
    sDacConfig.uOperationConfig.sAutomaticModeConfig.u16StepSize     = DEMO_TRIANGLE_WAVEFORM_INIT_STEP_VALUE;
    sDacConfig.bEnableAnalogPortion                                  = true;
    DAC_Init(DEMO_DAC_BASE, &sDacConfig);

    XBARA_Init(DEMO_XBARA_BASE);
    XBARA_SetSignalsConnection(DEMO_XBARA_BASE, DEMO_XBARA_SWITCH_CHANNEL, DEMO_XBARA_DAC_CHANNEL);

    while (1)
    {
        g_DACMinValue += 500U;
        g_DACMaxValue -= 500U;
        if (g_DACMinValue >= g_DACMaxValue)
        {
            g_DACMinValue = DEMO_TRIANGLE_WAVEFORM_INIT_MIN_VALUE;
            g_DACMaxValue = DEMO_TRIANGLE_WAVEFORM_INIT_MAX_VALUE;
        }
        DAC_WriteMinValue(DEMO_DAC_BASE, g_DACMinValue);
        DAC_WriteMaxValue(DEMO_DAC_BASE, g_DACMaxValue);
        DAC_WriteStepSize(DEMO_DAC_BASE, DEMO_TRIANGLE_WAVEFORM_INIT_STEP_VALUE);

        /* Calculates the compare value based on the frequency, minimum value, maximum value and step size. */
        u32TmpDivider = (DEMO_CLOCK_FREQUENCY / DEMO_TRIANGLE_WAVEFORM_FREQ * DEMO_TRIANGLE_WAVEFORM_INIT_STEP_VALUE);
        u32TmpDivider /= ((2UL * (g_DACMaxValue - g_DACMinValue)) - 1UL);
        if (u32TmpDivider > 4095UL)
        {
            g_DACMinValue = DEMO_TRIANGLE_WAVEFORM_INIT_MIN_VALUE;
            g_DACMaxValue = DEMO_TRIANGLE_WAVEFORM_INIT_MAX_VALUE;
            continue;
        }
        PRINTF("\r\nPlease input any keys to allow new values of minimum, maximum, step size\r\n");
        PRINTF("to be updated by active edge of switch %s.\r\n", DEMO_SWITCH_NAME);
        cInputChar = GETCHAR();
        DAC_SetLDOK(DEMO_DAC_BASE);
        PRINTF("\r\nPlease press switch %s to update the waveform!\r\n", DEMO_SWITCH_NAME);
        while (DAC_GetLDOKValue(DEMO_DAC_BASE))
        {
        }
        DAC_ConfigRefreshFrequency(DEMO_DAC_BASE, (uint16_t)u32TmpDivider);
        PRINTF("\r\nThe updated triangle waveform can be viewed on the Oscilloscope.\r\n");
    }
}
