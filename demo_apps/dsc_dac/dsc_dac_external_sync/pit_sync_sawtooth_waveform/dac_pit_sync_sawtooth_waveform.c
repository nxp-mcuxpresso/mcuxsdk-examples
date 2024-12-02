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
#include "fsl_pit.h"
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
uint16_t g_DACMinValue = DEMO_SAWTOOTH_WAVEFORM_INIT_MIN_VALUE;
uint16_t g_DACMaxValue = DEMO_SAWTOOTH_WAVEFORM_INIT_MAX_VALUE;
/*******************************************************************************
 * Code
 ******************************************************************************/

void DEMO_PIT_IRQHander(void)
{
    PIT_ClearStatusFlags(DEMO_PIT_BASE);
    g_DACMaxValue -= 500U;
    if (g_DACMaxValue <= (g_DACMinValue + 500U))
    {
        g_DACMaxValue = DEMO_SAWTOOTH_WAVEFORM_INIT_MAX_VALUE;
    }
    DAC_WriteMaxValue(DEMO_DAC_BASE, g_DACMaxValue);
    DAC_SetLDOK(DEMO_DAC_BASE);
}

int main(void)
{
    dac_config_t sDacConfig;
    pit_config_t sPitConfig;
    char cInputChar;
    uint32_t u32TmpDivider;

    BOARD_InitHardware();

    PRINTF("DAC Pit Sync Sawtooth Waveform Demo.\r\n");
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
    sDacConfig.uOperationConfig.sAutomaticModeConfig.eWaveformType   = kDAC_OneShotSawtoothWaveform1;
    sDacConfig.uOperationConfig.sAutomaticModeConfig.u16CompareValue = 500U;
    sDacConfig.uOperationConfig.sAutomaticModeConfig.u16MaxValue     = DEMO_SAWTOOTH_WAVEFORM_INIT_MAX_VALUE;
    sDacConfig.uOperationConfig.sAutomaticModeConfig.u16MinValue     = DEMO_SAWTOOTH_WAVEFORM_INIT_MIN_VALUE;
    sDacConfig.uOperationConfig.sAutomaticModeConfig.u16StartValue   = DEMO_SAWTOOTH_WAVEFORM_INIT_MIN_VALUE;
    sDacConfig.uOperationConfig.sAutomaticModeConfig.u16StepSize     = DEMO_SAWTOOTH_WAVEFORM_INIT_STEP_VALUE;
    sDacConfig.bEnableAnalogPortion                                  = true;
    DAC_Init(DEMO_DAC_BASE, &sDacConfig);

    XBARA_Init(DEMO_XBARA_BASE);
    XBARA_SetSignalsConnection(DEMO_XBARA_BASE, DEMO_XBARA_PIT_CHANNEL, DEMO_XBARA_DAC_CHANNEL);

    /*
     * Gets PIT default configuration.
     * psConfig->ePrescaler = kPIT_PrescalerDivBy1;
     * psConfig->bEnableInterrupt = false;
     * psConfig->bEnableSlaveMode = false;
     * psConfig->bEnableTimer = false;
     * psConfig->eClockSource = kPIT_CountClockSource0;
     * psConfig->u16PeriodCount = 0xFFFFU;
     */
    EnableIRQWithPriority(PIT0_ROLLOVR_IRQn, 1);
    PIT_GetDefaultConfig(&sPitConfig);
    sPitConfig.bEnableInterrupt = true;
    sPitConfig.bEnableTimer     = true;
#if defined(FSL_FEATURE_PIT_32BIT_COUNTER) && FSL_FEATURE_PIT_32BIT_COUNTER
    sPitConfig.u32PeriodCount = DEMO_PIT_PERIOD_COUNT;
#else
    sPitConfig.u16PeriodCount = DEMO_PIT_PERIOD_COUNT;
#endif
    sPitConfig.ePrescaler = kPIT_PrescalerDivBy2048;
    PIT_Init(DEMO_PIT_BASE, &sPitConfig);

    while (1)
    {
    }
}
