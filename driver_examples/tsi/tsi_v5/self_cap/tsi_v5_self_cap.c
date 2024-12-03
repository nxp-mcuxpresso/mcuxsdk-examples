/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"
#include "fsl_tsi_v5.h"
#include "fsl_debug_console.h"
#include "fsl_lptmr.h"
#include "fsl_trgmux.h"
#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
tsi_calibration_data_t buffer;
/* Array of TSI peripheral base address. */
#if defined(TSI0)
#define APP_TSI TSI0
#elif defined(TSI)
#define APP_TSI TSI
#endif

/*******************************************************************************
 * Code
 ******************************************************************************/
void TSI0_IRQHandler(void)
{
    if (TSI_GetSelfCapMeasuredChannel(APP_TSI) == BOARD_TSI_ELECTRODE_1)
    {
        if (TSI_GetCounter(APP_TSI) < (uint16_t)((float)buffer.calibratedData[BOARD_TSI_ELECTRODE_1] * (1.0F - TOUCH_DELTA_PERCENT)))
        {
            LED1_TOGGLE(); /* Toggle the touch event indicating LED */
        }
    }

    /* Clear endOfScan flag */
    TSI_ClearStatusFlags(APP_TSI, kTSI_EndOfScanFlag);
    SDK_ISR_EXIT_BARRIER;
}

/*!
 * @brief Main function
 */
int main(void)
{
    volatile uint32_t i = 0;
    tsi_selfCap_config_t tsiConfig_selfCap;
    lptmr_config_t lptmrConfig;
    memset((void *)&lptmrConfig, 0, sizeof(lptmrConfig));

    /* Initialize standard SDK demo application pins */
    BOARD_InitHardware();
    /* Init tsi Leds in Demo app */
    LED1_INIT();

    /* Configure LPTMR */
    LPTMR_GetDefaultConfig(&lptmrConfig);
    /* TSI default hardware configuration for self-cap mode */
    TSI_GetSelfCapModeDefaultConfig(&tsiConfig_selfCap);

    /* Initialize the LPTMR */
    LPTMR_Init(LPTMR0, &lptmrConfig);
    /* Initialize the TSI */
    TSI_InitSelfCapMode(APP_TSI, &tsiConfig_selfCap);
    /* Enable noise cancellation function */
    TSI_EnableNoiseCancellation(APP_TSI, true);

    /* Set timer period */
    LPTMR_SetTimerPeriod(LPTMR0, USEC_TO_COUNT(LPTMR_USEC_COUNT, LPTMR_SOURCE_CLOCK));

    NVIC_EnableIRQ(TSI0_IRQn);
    TSI_EnableModule(APP_TSI, true); /* Enable module */

    PRINTF("\r\nTSI_V5 Self-Cap mode Example Start!\r\n");
    /*********  CALIBRATION PROCESS ************/
    memset((void *)&buffer, 0, sizeof(buffer));
    TSI_SelfCapCalibrate(APP_TSI, &buffer);
    /* Print calibrated counter values */
    for (i = 0U; i < FSL_FEATURE_TSI_CHANNEL_COUNT; i++)
    {
        PRINTF("Calibrated counters for channel %d is: %d \r\n", i, buffer.calibratedData[i]);
    }

    /********** SOFTWARE TRIGGER SCAN USING POLLING METHOD ********/
    PRINTF("\r\nNOW, comes to the software trigger scan using polling method!\r\n");
    TSI_EnableHardwareTriggerScan(APP_TSI, false); /* Enable software trigger scan */
    TSI_DisableInterrupts(APP_TSI, kTSI_EndOfScanInterruptEnable);
    TSI_ClearStatusFlags(APP_TSI, kTSI_EndOfScanFlag);
    TSI_SetSelfCapMeasuredChannel(APP_TSI, BOARD_TSI_ELECTRODE_1);
    TSI_StartSoftwareTrigger(APP_TSI);
    while (!(TSI_GetStatusFlags(APP_TSI) & kTSI_EndOfScanFlag))
    {
    }
    PRINTF("Channel %d Normal mode counter is: %d \r\n", BOARD_TSI_ELECTRODE_1, TSI_GetCounter(APP_TSI));
#if (defined(PAD_TSI_ELECTRODE_2_ENABLED) && PAD_TSI_ELECTRODE_2_ENABLED)
    TSI_ClearStatusFlags(APP_TSI, kTSI_EndOfScanFlag);
    TSI_SetSelfCapMeasuredChannel(APP_TSI, BOARD_TSI_ELECTRODE_2);
    TSI_StartSoftwareTrigger(APP_TSI);
    while (!(TSI_GetStatusFlags(APP_TSI) & kTSI_EndOfScanFlag))
    {
    }
    PRINTF("Channel %d Normal mode counter is: %d \r\n", BOARD_TSI_ELECTRODE_2, TSI_GetCounter(APP_TSI));
#endif
    TSI_ClearStatusFlags(APP_TSI, kTSI_EndOfScanFlag | kTSI_OutOfRangeFlag);

    /********** SOFTWARE TRIGGER SCAN USING INTERRUPT METHOD ********/
    PRINTF("\r\nNOW, comes to the software trigger scan using interrupt method!\r\n");
    TSI_EnableInterrupts(APP_TSI, kTSI_GlobalInterruptEnable);
    TSI_EnableInterrupts(APP_TSI, kTSI_EndOfScanInterruptEnable);
    TSI_ClearStatusFlags(APP_TSI, kTSI_EndOfScanFlag);
    TSI_SetSelfCapMeasuredChannel(APP_TSI, BOARD_TSI_ELECTRODE_1);
    TSI_StartSoftwareTrigger(APP_TSI);
    while (TSI_IsScanInProgress(APP_TSI))
    {
    }
    PRINTF("Channel %d Normal mode counter is: %d \r\n", BOARD_TSI_ELECTRODE_1, TSI_GetCounter(APP_TSI));
#if (defined(PAD_TSI_ELECTRODE_2_ENABLED) && PAD_TSI_ELECTRODE_2_ENABLED)
    TSI_SetSelfCapMeasuredChannel(APP_TSI, BOARD_TSI_ELECTRODE_2);
    TSI_StartSoftwareTrigger(APP_TSI);
    while (TSI_IsScanInProgress(APP_TSI))
    {
    }
    PRINTF("Channel %d Normal mode counter is: %d \r\n", BOARD_TSI_ELECTRODE_2, TSI_GetCounter(APP_TSI));
#endif
    /********** HARDWARE TRIGGER SCAN ********/
    PRINTF("\r\nNOW, comes to the hardware trigger scan method!\r\n");
    PRINTF("After running, touch pad %s each time, you will see LED toggles.\r\n", PAD_TSI_ELECTRODE_1_NAME);
    TSI_EnableModule(APP_TSI, false);
    TSI_EnableHardwareTriggerScan(APP_TSI, true);
    TSI_EnableInterrupts(APP_TSI, kTSI_GlobalInterruptEnable);
    TSI_EnableInterrupts(APP_TSI, kTSI_EndOfScanInterruptEnable);
    TSI_ClearStatusFlags(APP_TSI, kTSI_EndOfScanFlag);

    TSI_SetSelfCapMeasuredChannel(APP_TSI,
                                  BOARD_TSI_ELECTRODE_1); /* Select BOARD_TSI_ELECTRODE_1 as detecting electrode. */
    TSI_EnableModule(APP_TSI, true);
    TRGMUX_SetTriggerSource(TRGMUX0, TSI_TRGMUX_REG_INDEX, kTRGMUX_TriggerInput0, kTRGMUX_SourceLptmr0);
    LPTMR_StartTimer(LPTMR0); /* Start LPTMR triggering */

    while (1)
    {
    }
}
