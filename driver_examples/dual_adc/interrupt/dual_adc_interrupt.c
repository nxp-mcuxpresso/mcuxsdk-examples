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
 *   DEMO_DUALADC_USER_CHANNEL_0..3  : analog channels to convert (0~31)
 */

/* 12-bit standard resolution, JLEFT=0: result is at bit[14:3]; right-shift by 3. */
#define DEMO_DUALADC_RESULT_SHIFT (3U)
#define DEMO_DUALADC_FULL_RANGE   (4096U)

#define LOG_INFO (void)PRINTF

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile bool g_DualAdcConversionCompletedFlag = false;
uint16_t g_DualAdcResult[4] = {0};

/*******************************************************************************
 * Code
 ******************************************************************************/
void DEMO_DUALADC_IRQ_HANDLER_FUNC(void)
{
    uint32_t status;

    status = DUALADC_GetScanSequenceStatusFlags(DEMO_DUALADC_BASE);

    if (0U != (status & (uint32_t)kDUALADC_EndofScanAFlag))
    {
        /* Clear End-of-Scan flag for ADCA. */
        DUALADC_ClearScanSequenceStatusFlags(DEMO_DUALADC_BASE, (uint32_t)kDUALADC_EndofScanAFlag);

        /* Read conversion sequence result; reading also clears the result-ready flag. */
        g_DualAdcResult[0] = DUALADC_GetConversionResultLow(DEMO_DUALADC_BASE, 1U);
        g_DualAdcResult[1] = DUALADC_GetConversionResultLow(DEMO_DUALADC_BASE, 2U);
        g_DualAdcResult[2] = DUALADC_GetConversionResultLow(DEMO_DUALADC_BASE, 3U);
        g_DualAdcResult[3] = DUALADC_GetConversionResultHigh(DEMO_DUALADC_BASE, 3U);

        g_DualAdcConversionCompletedFlag = true;
    }

    SDK_ISR_EXIT_BARRIER;
}

/*!
 * @brief Main function
 */
int main(void)
{
    dualadc_config_t adcConfig;
    dualadc_cmd_config_t cmdConfig;

    BOARD_InitHardware();

    LOG_INFO("MCUX SDK version: %s\r\n", MCUXSDK_VERSION_FULL_STR);
    LOG_INFO("DualADC Interrupt Example\r\n");

    /* Initialize ADC with default configuration. */
    DUALADC_GetDefaultConfig(&adcConfig);

    adcConfig.conversionMode = kDUALADC_CooperationMode;
    adcConfig.enableAnalogPreEnable = true; /* Pre-enable analog for minimum trigger latency. */
    (void)DUALADC_Init(DEMO_DUALADC_BASE, &adcConfig);

    /* Configure command buffer CMD1: single-ended A-side, standard 12-bit resolution. */
    DUALADC_GetDefaultConversionCommandConfig(&cmdConfig);

    cmdConfig.channelNumber = DEMO_DUALADC_USER_CHANNEL_0;
    cmdConfig.resolutionMode = kDUALADC_ResolutionStandard;
    cmdConfig.conversionType = kDUALADC_ConversionSingleEndedA;
    cmdConfig.sampleTimeMode = kDUALADC_SampleTime131_5;
    cmdConfig.hardwareAverages = kDUALADC_HardwareAverage32;
    DUALADC_ConfigConversionCommand(DEMO_DUALADC_BASE, 1U, &cmdConfig);

    /* Configure command buffer CMD2: single-ended B-side, standard 12-bit resolution. */
    cmdConfig.channelNumber = DEMO_DUALADC_USER_CHANNEL_1;
    cmdConfig.conversionType = kDUALADC_ConversionSingleEndedB;
    DUALADC_ConfigConversionCommand(DEMO_DUALADC_BASE, 2U, &cmdConfig);

    /*
     * Configure command buffer CMD3: dual single-ended,
     * alternate channel B select enabled, standard 12-bit resolution.
     */
    cmdConfig.enableAltBChannel = true;
    cmdConfig.channelNumber = DEMO_DUALADC_USER_CHANNEL_2;
    cmdConfig.altBChannelNumber = DEMO_DUALADC_USER_CHANNEL_3;
    cmdConfig.conversionType = kDUALADC_ConversionDualSingleEnded;
    DUALADC_ConfigConversionCommand(DEMO_DUALADC_BASE, 3U, &cmdConfig);

    /* Mark CMD4 as sequence terminator so the sequence ends after CMD3. */
    DUALADC_SetConversionSequenceEndCommand(DEMO_DUALADC_BASE, 4U);

    /* Enable End-of-Scan interrupt for ADCA. */
    DUALADC_EnableInterrupts(DEMO_DUALADC_BASE, (uint32_t)kDUALADC_InterruptEndOfScanA);
    (void)EnableIRQ(DEMO_DUALADC_IRQn);

    /* Enable DualADC before calibration. */
    DUALADC_Enable(DEMO_DUALADC_BASE);

    /* Run full calibration: offset, optional high-speed, and gain. */
    if (kStatus_Success != DUALADC_DoCalibration(DEMO_DUALADC_BASE))
    {
        LOG_INFO("Calibration failed.\r\n");
        return 0;
    }

    LOG_INFO("ADC Full Range: %d\r\n", DEMO_DUALADC_FULL_RANGE);
    LOG_INFO("Please press any key to get user channel's ADC value.\r\n");

    while (true)
    {
        (void)GETCHAR();

        /* Issue a software trigger to start conversion (Trigger0 starts ADCA). */
        DUALADC_SetSoftwareTrigger(DEMO_DUALADC_BASE, (uint32_t)kDUALADC_SoftwareTrigger0);

        /* Wait for End-of-Scan interrupt to signal conversion complete. */
        while (!g_DualAdcConversionCompletedFlag)
        {
        }

        g_DualAdcResult[0] >>= DEMO_DUALADC_RESULT_SHIFT;
        g_DualAdcResult[1] >>= DEMO_DUALADC_RESULT_SHIFT;
        g_DualAdcResult[2] >>= DEMO_DUALADC_RESULT_SHIFT;
        g_DualAdcResult[3] >>= DEMO_DUALADC_RESULT_SHIFT;

        /* resultLow holds bit[14:3] of the 12-bit value; right-shift by 3 to recover. */
        LOG_INFO("ADC value: %d %d %d %d\r\n", g_DualAdcResult[0], g_DualAdcResult[1],
                    g_DualAdcResult[2], g_DualAdcResult[3]);

        g_DualAdcConversionCompletedFlag = false;
    }
}
