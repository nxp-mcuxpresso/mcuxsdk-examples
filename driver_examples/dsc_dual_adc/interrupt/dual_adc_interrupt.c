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
/* ADC peripheral and interrupt configuration - defined in board-specific app.h:
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
void DEMO_DUALADC_IRQ_HANDLER_FUNC(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile bool g_DualAdcConversionCompletedFlag = false;
uint16_t g_DualAdcResult[4] = {0};

/*******************************************************************************
 * Code
 ******************************************************************************/
#pragma interrupt alignsp saveall
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
#pragma interrupt off

/*!
 * @brief Main function
 */
int main(void)
{
    dualadc_config_t adcConfig;

    BOARD_InitHardware();

    LOG_INFO("MCUX SDK version: %s\r\n", MCUXSDK_VERSION_FULL_STR);
    LOG_INFO("DualADC Interrupt Example\r\n");

    /*
     * Populate the whole DualADC configuration then bring the module up with a single
     * DUALADC_Init call: the conversion command sequence, the End-of-Scan interrupt and
     * the final ADC enable are all applied from this structure.
     */
    DUALADC_GetDefaultConfig(&adcConfig);

    adcConfig.eConversionMode        = kDUALADC_CooperationMode;
    adcConfig.bEnableAnalogPreEnable = true; /* Pre-enable analog for minimum trigger latency. */

    /* CMD1: single-ended A-side, standard 12-bit resolution, longest sample time. */
    adcConfig.sConvCmdConfig[0].u32ChannelNumber  = DEMO_DUALADC_USER_CHANNEL_0;
    adcConfig.sConvCmdConfig[0].u32ResolutionMode = kDUALADC_ResolutionStandard;
    adcConfig.sConvCmdConfig[0].u32ConversionType = kDUALADC_ConversionSingleEndedA;
    adcConfig.sConvCmdConfig[0].u32SampleTimeMode = kDUALADC_SampleTime131_5;
    adcConfig.sConvCmdConfig[0].u32HardwareAverages = kDUALADC_HardwareAverage32;

    /* CMD2: single-ended B-side, standard 12-bit resolution. */
    adcConfig.sConvCmdConfig[1].u32ChannelNumber  = DEMO_DUALADC_USER_CHANNEL_1;
    adcConfig.sConvCmdConfig[1].u32ResolutionMode = kDUALADC_ResolutionStandard;
    adcConfig.sConvCmdConfig[1].u32ConversionType = kDUALADC_ConversionSingleEndedB;
    adcConfig.sConvCmdConfig[1].u32SampleTimeMode = kDUALADC_SampleTime131_5;
    adcConfig.sConvCmdConfig[1].u32HardwareAverages = kDUALADC_HardwareAverage32;

    /*
     * CMD3: dual single-ended, alternate channel B select enabled,
     * standard 12-bit resolution.
     */
    adcConfig.sConvCmdConfig[2].u32ChannelNumber    = DEMO_DUALADC_USER_CHANNEL_2;
    adcConfig.sConvCmdConfig[2].u32EnableAltBChannel = true;
    adcConfig.sConvCmdConfig[2].u32AltBChannelNumber = DEMO_DUALADC_USER_CHANNEL_3;
    adcConfig.sConvCmdConfig[2].u32ResolutionMode   = kDUALADC_ResolutionStandard;
    adcConfig.sConvCmdConfig[2].u32ConversionType   = kDUALADC_ConversionDualSingleEnded;
    adcConfig.sConvCmdConfig[2].u32SampleTimeMode   = kDUALADC_SampleTime131_5;
    adcConfig.sConvCmdConfig[2].u32HardwareAverages = kDUALADC_HardwareAverage32;

    /* Mark CMD4 as the sequence terminator so the scan ends after CMD3. */
    adcConfig.sConvCmdConfig[3].u32DisableCommand = true;

    /* Enable the End-of-Scan interrupt for ADCA. */
    adcConfig.u32InterruptMask = (uint32_t)kDUALADC_InterruptEndOfScanA;

    (void)DUALADC_Init(DEMO_DUALADC_BASE, &adcConfig);

    /* Run full calibration: offset, optional high-speed, and gain. The ADC is already
     * enabled by DUALADC_Init (config.bEnableAdc defaults to true). */
    if (kStatus_Success != DUALADC_DoCalibration(DEMO_DUALADC_BASE))
    {
        LOG_INFO("Calibration failed.\r\n");
        return 0;
    }

    (void)EnableIRQWithPriority(DEMO_DUALADC_IRQn, DEMO_DUALADC_IRQ_PRIO);

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
