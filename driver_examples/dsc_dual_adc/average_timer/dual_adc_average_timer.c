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
 *   DEMO_DUALADC_IRQ_PRIO           : DADC IRQ priority level
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
void DEMO_DUALADC_IRQ_HANDLER_FUNC(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile bool g_DualAdcConversionCompletedFlag = false;
/* Summed results from the average-with-timer (RSLT holds the accumulated sum). */
uint32_t g_DualAdcResultSum[4] = {0};

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
#pragma interrupt off

/*!
 * @brief Main function
 */
int main(void)
{
    dualadc_config_t adcConfig;
    uint32_t average[4] = {0};

    BOARD_InitHardware();

    LOG_INFO("MCUX SDK version: %s\r\n", MCUXSDK_VERSION_FULL_STR);
    LOG_INFO("DualADC Average With Timer Example\r\n");

    /*
     * Populate the whole DualADC configuration then bring the module up with a single
     * DUALADC_Init call: the conversion command sequence, the average-with-timer feature,
     * the End-of-Scan interrupt and the final ADC enable are all applied from this
     * structure by the dsc_dual_adc driver.
     */
    DUALADC_GetDefaultConfig(&adcConfig);

    adcConfig.eConversionMode        = kDUALADC_CooperationMode;
    adcConfig.bEnableAnalogPreEnable = true; /* Pre-enable analog for minimum trigger latency. */

    /*
     * Configure the command buffers. Two A-side channels and two B-side channels,
     * each occupying one command buffer. All use 12-bit standard resolution and a single
     * hardware conversion (no hardware averaging). The sample time is kept short so the
     * whole sequence completes inside the average-with-timer interval.
     */

    /* CMD1: A-side single-ended. */
    adcConfig.sConvCmdConfig[0].u32ChannelNumber    = DEMO_DUALADC_USER_CHANNEL_0;
    adcConfig.sConvCmdConfig[0].u32ResolutionMode   = kDUALADC_ResolutionStandard;
    adcConfig.sConvCmdConfig[0].u32ConversionType   = kDUALADC_ConversionSingleEndedA;
    adcConfig.sConvCmdConfig[0].u32SampleTimeMode   = kDUALADC_SampleTime11_5;
    adcConfig.sConvCmdConfig[0].u32HardwareAverages = kDUALADC_HardwareAverageSingle;

    /* CMD2: B-side single-ended. */
    adcConfig.sConvCmdConfig[1].u32ChannelNumber    = DEMO_DUALADC_USER_CHANNEL_1;
    adcConfig.sConvCmdConfig[1].u32ResolutionMode   = kDUALADC_ResolutionStandard;
    adcConfig.sConvCmdConfig[1].u32ConversionType   = kDUALADC_ConversionSingleEndedB;
    adcConfig.sConvCmdConfig[1].u32SampleTimeMode   = kDUALADC_SampleTime11_5;
    adcConfig.sConvCmdConfig[1].u32HardwareAverages = kDUALADC_HardwareAverageSingle;

    /* CMD3: A-side single-ended. */
    adcConfig.sConvCmdConfig[2].u32ChannelNumber    = DEMO_DUALADC_USER_CHANNEL_2;
    adcConfig.sConvCmdConfig[2].u32ResolutionMode   = kDUALADC_ResolutionStandard;
    adcConfig.sConvCmdConfig[2].u32ConversionType   = kDUALADC_ConversionSingleEndedA;
    adcConfig.sConvCmdConfig[2].u32SampleTimeMode   = kDUALADC_SampleTime11_5;
    adcConfig.sConvCmdConfig[2].u32HardwareAverages = kDUALADC_HardwareAverageSingle;

    /* CMD4: B-side single-ended. */
    adcConfig.sConvCmdConfig[3].u32ChannelNumber    = DEMO_DUALADC_USER_CHANNEL_3;
    adcConfig.sConvCmdConfig[3].u32ResolutionMode   = kDUALADC_ResolutionStandard;
    adcConfig.sConvCmdConfig[3].u32ConversionType   = kDUALADC_ConversionSingleEndedB;
    adcConfig.sConvCmdConfig[3].u32SampleTimeMode   = kDUALADC_SampleTime11_5;
    adcConfig.sConvCmdConfig[3].u32HardwareAverages = kDUALADC_HardwareAverageSingle;

    /* Mark CMD5 as the sequence terminator so the scan ends after CMD4. */
    adcConfig.sConvCmdConfig[4].u32DisableCommand = true;

    /*
     * Configure average-with-timer for ADCA (cooperation mode: ADCB follows ADCA).
     * The whole scan sequence is repeated DEMO_DUALADC_AVGT_REPEAT times at the
     * configured interval, and the hardware accumulates the sum into RSLT.
     */
    adcConfig.sAvgtConfig.bEnableAvgTimerA = true;
    adcConfig.sAvgtConfig.u16RepeatCountA  = DEMO_DUALADC_AVGT_REPEAT;
    adcConfig.sAvgtConfig.u16IntervalA     = DEMO_DUALADC_AVGT_INTERVAL;
    adcConfig.sAvgtConfig.ePrescalerA      = DEMO_DUALADC_AVGT_PRESCALER;

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
