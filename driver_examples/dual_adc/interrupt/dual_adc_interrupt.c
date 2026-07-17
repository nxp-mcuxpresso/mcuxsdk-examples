/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_dual_adc.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* ADC peripheral and interrupt configuration — defined in board-specific app.h:
 *   DEMO_DUALADC_BASE               : DADC peripheral base pointer (e.g. DADC0)
 *   DEMO_DUALADC_IRQn               : DADC IRQ number
 *   DEMO_DUALADC_IRQ_HANDLER_FUNC   : DADC IRQ handler function name
 *   DEMO_DUALADC_USER_CHANNEL       : A-side analog channel to convert (0~31)
 *   DEMO_DUALADC_USER_CMDID         : Command buffer index to use (1~20)
 */

/* 12-bit standard resolution, JLEFT=0: result is at bit[14:3]; right-shift by 3. */
#define DEMO_DUALADC_RESULT_SHIFT (3U)
#define DEMO_DUALADC_FULL_RANGE   (4096U)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile bool g_DualAdcConversionCompletedFlag = false;
dadc_conversion_result_t g_DualAdcResult;

/*******************************************************************************
 * Code
 ******************************************************************************/
void DEMO_DUALADC_IRQ_HANDLER_FUNC(void)
{
    uint32_t status;

    status = DUALADC_GetScanSequenceStatusFlags(DEMO_DUALADC_BASE);

    if (0U != (status & kDADC_EndofScanAFlag))
    {
        /* Clear End-of-Scan flag for ADCA. */
        DUALADC_ClearScanSequenceStatusFlags(DEMO_DUALADC_BASE, kDADC_EndofScanAFlag);

        /* Read CMD1 result; reading also clears the result-ready flag. */
        DUALADC_GetConversionResult(DEMO_DUALADC_BASE, DEMO_DUALADC_USER_CMDID, &g_DualAdcResult);

        g_DualAdcConversionCompletedFlag = true;
    }

    SDK_ISR_EXIT_BARRIER;
}

/*!
 * @brief Main function
 */
int main(void)
{
    dadc_config_t adcConfig;
    dadc_cmd_config_t cmdConfig;

    BOARD_InitHardware();

    PRINTF("MCUX SDK version: %s\r\n", MCUXSDK_VERSION_FULL_STR);
    PRINTF("DualADC Interrupt Example\r\n");

    /* Initialize ADC with default configuration. */
    DUALADC_GetDefaultConfig(&adcConfig);

    adcConfig.conversionMode = kDADC_CooperationMode;
    adcConfig.enableAnalogPreEnable = true; /* Pre-enable analog for minimum trigger latency. */
    DUALADC_Init(DEMO_DUALADC_BASE, &adcConfig);

    /* Enable DualADC before calibration. */
    DUALADC_Enable(DEMO_DUALADC_BASE);

    /* Run full calibration: offset, optional high-speed, and gain. */
    if (kStatus_Success != DUALADC_DoCalibration(DEMO_DUALADC_BASE))
    {
        PRINTF("Calibration failed.\r\n");
        return 0;
    }

    /* Configure command buffer CMD1: single-ended A-side, standard 12-bit resolution. */
    DUALADC_GetDefaultConversionCommandConfig(&cmdConfig);

    cmdConfig.channelNumber = DEMO_DUALADC_USER_CHANNEL;
    cmdConfig.resolutionMode = kDADC_ResolutionStandard;
    cmdConfig.conversionType = kDADC_ConversionSingleEndedA;
    cmdConfig.sampleTimeMode = kDADC_SampleTime131_5;
    DUALADC_ConfigConversionCommand(DEMO_DUALADC_BASE, DEMO_DUALADC_USER_CMDID, &cmdConfig);

    /* Mark CMD2 as sequence terminator so the sequence ends after CMD1. */
    DUALADC_SetConversionSequenceEndCommand(DEMO_DUALADC_BASE, DEMO_DUALADC_USER_CMDID + 1U);

    /* Enable End-of-Scan interrupt for ADCA. */
    DUALADC_EnableInterrupts(DEMO_DUALADC_BASE, kDADC_InterruptEndOfScanA);
    EnableIRQ(DEMO_DUALADC_IRQn);

    PRINTF("ADC Full Range: %d\r\n", DEMO_DUALADC_FULL_RANGE);
    PRINTF("Please press any key to get user channel's ADC value.\r\n");

    while (1)
    {
        GETCHAR();

        /* Issue a software trigger to start conversion (Trigger0 starts ADCA). */
        DUALADC_SetSoftwareTrigger(DEMO_DUALADC_BASE, kDADC_SoftwareTrigger0);

        /* Wait for End-of-Scan interrupt to signal conversion complete. */
        while (!g_DualAdcConversionCompletedFlag)
        {
        }

        /* resultLow holds bit[14:3] of the 12-bit value; right-shift by 3 to recover. */
        PRINTF("ADC value: %d\r\n", (g_DualAdcResult.resultLow >> DEMO_DUALADC_RESULT_SHIFT));

        g_DualAdcConversionCompletedFlag = false;
    }
}
