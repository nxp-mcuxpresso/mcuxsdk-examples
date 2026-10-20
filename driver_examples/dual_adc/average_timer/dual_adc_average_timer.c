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

/* Average-with-timer repeat count: each sequence is converted 8 times and summed. */
#define DEMO_DUALADC_AVGT_REPEAT (8U)

/*
 * Average-with-timer interval = intervalA * ADCK_period * 2^prescaler.
 * The interval must be long enough for the whole scan sequence (all CMDs) to
 * finish before the next interval tick; otherwise an interval-time error occurs.
 * A larger prescaler/interval gives more margin.
 */
#define DEMO_DUALADC_AVGT_INTERVAL   (2000U)
#define DEMO_DUALADC_AVGT_PRESCALER  kDUALADC_AvgtPrescaler16

#define LOG_INFO (void)PRINTF

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile bool g_DualAdcConversionCompletedFlag = false;
/* Summed results from the average-with-timer (RSLT holds the accumulated sum). */
uint32_t g_DualAdcResultSum[4] = {0};

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

        /*
         * Read the summed conversion result for each command. When average-with-timer
         * is enabled, RSLT holds the sum of all repeated conversions; reading also
         * clears the result-ready flag.
         */
        g_DualAdcResultSum[0] = DUALADC_GetConversionResult32(DEMO_DUALADC_BASE, 1U);
        g_DualAdcResultSum[1] = DUALADC_GetConversionResult32(DEMO_DUALADC_BASE, 2U);
        g_DualAdcResultSum[2] = DUALADC_GetConversionResult32(DEMO_DUALADC_BASE, 3U);
        g_DualAdcResultSum[3] = DUALADC_GetConversionResult32(DEMO_DUALADC_BASE, 4U);

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
    dualadc_avgt_config_t avgtConfig;
    uint32_t average[4] = {0};

    BOARD_InitHardware();

    LOG_INFO("MCUX SDK version: %s\r\n", MCUXSDK_VERSION_FULL_STR);
    LOG_INFO("DualADC Average With Timer Example\r\n");

    /* Initialize ADC with default configuration, cooperation mode. */
    DUALADC_GetDefaultConfig(&adcConfig);

    adcConfig.conversionMode        = kDUALADC_CooperationMode;
    adcConfig.enableAnalogPreEnable = true; /* Pre-enable analog for minimum trigger latency. */
    (void)DUALADC_Init(DEMO_DUALADC_BASE, &adcConfig);

    /*
     * Configure the command buffers. Two A-side channels and two B-side channels,
     * each occupying one command buffer. All use 12-bit standard resolution and a single
     * hardware conversion (no hardware averaging). The sample time is kept short so the
     * whole sequence completes inside the average-with-timer interval.
     */
    DUALADC_GetDefaultConversionCommandConfig(&cmdConfig);

    cmdConfig.resolutionMode  = kDUALADC_ResolutionStandard;
    cmdConfig.sampleTimeMode  = kDUALADC_SampleTime11_5;
    cmdConfig.hardwareAverages = kDUALADC_HardwareAverageSingle;

    /* CMD1: A-side single-ended. */
    cmdConfig.channelNumber  = DEMO_DUALADC_USER_CHANNEL_0;
    cmdConfig.conversionType = kDUALADC_ConversionSingleEndedA;
    DUALADC_ConfigConversionCommand(DEMO_DUALADC_BASE, 1U, &cmdConfig);

    /* CMD2: B-side single-ended. */
    cmdConfig.channelNumber  = DEMO_DUALADC_USER_CHANNEL_1;
    cmdConfig.conversionType = kDUALADC_ConversionSingleEndedB;
    DUALADC_ConfigConversionCommand(DEMO_DUALADC_BASE, 2U, &cmdConfig);

    /* CMD3: A-side single-ended. */
    cmdConfig.channelNumber  = DEMO_DUALADC_USER_CHANNEL_2;
    cmdConfig.conversionType = kDUALADC_ConversionSingleEndedA;
    DUALADC_ConfigConversionCommand(DEMO_DUALADC_BASE, 3U, &cmdConfig);

    /* CMD4: B-side single-ended. */
    cmdConfig.channelNumber  = DEMO_DUALADC_USER_CHANNEL_3;
    cmdConfig.conversionType = kDUALADC_ConversionSingleEndedB;
    DUALADC_ConfigConversionCommand(DEMO_DUALADC_BASE, 4U, &cmdConfig);

    /* Mark CMD5 as sequence terminator so the sequence ends after CMD4. */
    DUALADC_SetConversionSequenceEndCommand(DEMO_DUALADC_BASE, 5U);

    /*
     * Configure average-with-timer for ADCA (cooperation mode: ADCB follows ADCA).
     * The whole scan sequence is repeated DEMO_DUALADC_AVGT_REPEAT times at the
     * configured interval, and the hardware accumulates the sum into RSLT.
     */
    DUALADC_GetDefaultAverageTimerConfig(&avgtConfig);

    avgtConfig.enableAvgTimerA = true;
    avgtConfig.repeatCountA    = DEMO_DUALADC_AVGT_REPEAT;
    avgtConfig.intervalA       = DEMO_DUALADC_AVGT_INTERVAL;
    avgtConfig.prescalerA      = DEMO_DUALADC_AVGT_PRESCALER;
    DUALADC_ConfigAverageTimer(DEMO_DUALADC_BASE, &avgtConfig);

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
    LOG_INFO("Average with timer repeat count: %d\r\n", DEMO_DUALADC_AVGT_REPEAT);
    LOG_INFO("Please press any key to get user channel's ADC value.\r\n");

    while (true)
    {
        (void)GETCHAR();

        /* Issue a software trigger to start conversion (Trigger0 starts ADCA). */
        DUALADC_SetSoftwareTrigger(DEMO_DUALADC_BASE, (uint32_t)kDUALADC_SoftwareTrigger0);

        /* Wait for End-of-Scan interrupt to signal all repeats are complete. */
        while (!g_DualAdcConversionCompletedFlag)
        {
        }

        /*
         * RSLT holds the sum of DEMO_DUALADC_AVGT_REPEAT raw conversions. In 12-bit
         * standard mode the code sits at bit[14:3]. As the driver recommends, apply the
         * right-shift to the summed raw value first (loses low bits only once), then
         * divide by the repeat count to obtain the averaged 12-bit value.
         */
        average[0] = (g_DualAdcResultSum[0] >> DEMO_DUALADC_RESULT_SHIFT) / DEMO_DUALADC_AVGT_REPEAT;
        average[1] = (g_DualAdcResultSum[1] >> DEMO_DUALADC_RESULT_SHIFT) / DEMO_DUALADC_AVGT_REPEAT;
        average[2] = (g_DualAdcResultSum[2] >> DEMO_DUALADC_RESULT_SHIFT) / DEMO_DUALADC_AVGT_REPEAT;
        average[3] = (g_DualAdcResultSum[3] >> DEMO_DUALADC_RESULT_SHIFT) / DEMO_DUALADC_AVGT_REPEAT;

        LOG_INFO("ADC average value: %d %d %d %d\r\n", average[0], average[1], average[2], average[3]);

        g_DualAdcConversionCompletedFlag = false;
    }
}
