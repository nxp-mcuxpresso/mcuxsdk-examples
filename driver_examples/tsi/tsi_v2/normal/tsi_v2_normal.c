/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*****************************************************************************
 *  Includes
 ****************************************************************************/

#include "app.h"
#include "board.h"
#include "fsl_tsi_v2.h"
#include "fsl_gpio.h"
#include "fsl_debug_console.h"

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

/*******************************************************************************
 * Code
 ******************************************************************************/
/* Delay n ms */
void DelayMs(uint8_t n)
{
    SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;   /* use core clock */
    SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;    /* disable interrupt */
    SysTick->LOAD = n * ((CORE_CLK_FREQ) / 1000U); /* n ms */
    SysTick->VAL  = 0U;                            /* clear COUNTFLAG */

    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;

    /* wait for timeout */
    while (0U == (SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk))
    {
    }

    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

void TSI0_IRQHandler(void)
{
    if (TSI_IsChannelEnabled(TSI0, BOARD_TSI_ELECTRODE_1))
    {
        if (TSI_GetNormalModeCounter(TSI0, BOARD_TSI_ELECTRODE_1) >
            (uint16_t)(buffer.calibratedData[BOARD_TSI_ELECTRODE_1] * (1.0F + TOUCH_DELTA_PERCENT)))
        {
            LED2_ON();
            DelayMs(50);
            DelayMs(50);
            LED2_OFF();
        }
    }
    if (TSI_IsChannelEnabled(TSI0, BOARD_TSI_ELECTRODE_2))
    {
        if (TSI_GetNormalModeCounter(TSI0, BOARD_TSI_ELECTRODE_2) >
            (uint16_t)(buffer.calibratedData[BOARD_TSI_ELECTRODE_2] * (1.0F + TOUCH_DELTA_PERCENT)))
        {
            LED1_ON();
            DelayMs(50);
            DelayMs(50);
            LED1_OFF();
        }
    }

    /* Clear flags */
    TSI_ClearStatusFlags(TSI0, kTSI_EndOfScanFlag);
    TSI_ClearStatusFlags(TSI0, kTSI_OutOfRangeFlag);
    SDK_ISR_EXIT_BARRIER;
}

/*!
 * @brief Main function
 */
int main(void)
{
    volatile uint32_t i           = 0U;
    tsi_config_t tsiConfig_normal = {0};

    /* Initialize standard SDK demo application pins */
    BOARD_InitHardware();
    /* Init tsi Leds in Demo app */
    LED1_INIT();
    LED2_INIT();

    TSI_GetNormalModeDefaultConfig(&tsiConfig_normal);
    TSI_Init(TSI0, &tsiConfig_normal);
    NVIC_EnableIRQ(TSI0_IRQn);
    TSI_EnableModule(TSI0, true); /* Enable module */

    PRINTF("\r\nTSI_V2 Normal_mode Example Start!\r\n");
    /*********  TSI_V2 CALIBRATION PROCESS ************/
    memset((void *)&buffer, 0, sizeof(buffer));
    TSI_Calibrate(TSI0, &buffer);
    /* Print calibrated counter values */
    for (i = 0U; i < FSL_FEATURE_TSI_CHANNEL_COUNT; i++)
    {
        PRINTF("Calibrated counters for channel %d is: %d \r\n", i, buffer.calibratedData[i]);
    }

    /********** SOFTWARE TRIGGER SCAN USING POLLING METHOD ********/
    PRINTF("\r\nNOW, comes to the software trigger scan using polling method!\r\n");
    TSI_EnablePeriodicalScan(TSI0, false); /* Enable Software trigger scan */
    TSI_DisableInterrupts(TSI0, kTSI_EndOfScanInterruptEnable);
    TSI_ClearStatusFlags(TSI0, kTSI_EndOfScanFlag);
    TSI_EnableChannel(TSI0, BOARD_TSI_ELECTRODE_1, true);
    TSI_StartSoftwareTrigger(TSI0);
    while (!(TSI_GetStatusFlags(TSI0) & kTSI_EndOfScanFlag))
    {
    }
    PRINTF("Channel %d Normal mode counter is: %d \r\n", BOARD_TSI_ELECTRODE_1,
           TSI_GetNormalModeCounter(TSI0, BOARD_TSI_ELECTRODE_1));

    TSI_ClearStatusFlags(TSI0, kTSI_EndOfScanFlag);
    TSI_EnableChannel(TSI0, BOARD_TSI_ELECTRODE_2, true);
    TSI_StartSoftwareTrigger(TSI0);
    while (!(TSI_GetStatusFlags(TSI0) & kTSI_EndOfScanFlag))
    {
    }
    PRINTF("Channel %d Normal mode counter is: %d \r\n", BOARD_TSI_ELECTRODE_2,
           TSI_GetNormalModeCounter(TSI0, BOARD_TSI_ELECTRODE_2));
    TSI_ClearStatusFlags(TSI0, kTSI_EndOfScanFlag);
    TSI_ClearStatusFlags(TSI0, kTSI_OutOfRangeFlag);

    /********** SOFTWARE TRIGGER SCAN USING INTERRUPT METHOD ********/
    PRINTF("\r\nNOW, comes to the software trigger scan using interrupt method!\r\n");
    TSI_EnableInterrupts(TSI0, kTSI_GlobalInterruptEnable);
    TSI_EnableInterrupts(TSI0, kTSI_EndOfScanInterruptEnable);
    TSI_ClearStatusFlags(TSI0, kTSI_EndOfScanFlag);
    TSI_EnableChannel(TSI0, BOARD_TSI_ELECTRODE_1, true);
    TSI_StartSoftwareTrigger(TSI0);
    while (TSI_IsScanInProgress(TSI0))
    {
    }
    PRINTF("Channel %d Normal mode counter is: %d \r\n", BOARD_TSI_ELECTRODE_1,
           TSI_GetNormalModeCounter(TSI0, BOARD_TSI_ELECTRODE_1));

    TSI_EnableChannel(TSI0, BOARD_TSI_ELECTRODE_2, true);
    TSI_StartSoftwareTrigger(TSI0);
    while (TSI_IsScanInProgress(TSI0))
    {
    }
    PRINTF("Channel %d Normal mode counter is: %d \r\n", BOARD_TSI_ELECTRODE_2,
           TSI_GetNormalModeCounter(TSI0, BOARD_TSI_ELECTRODE_2));

    /********** PERIOD TRIGGER SCAN ********/
    PRINTF("\r\nNOW, comes to the periodical scan method!\r\n");
    PRINTF("After running, touch pad %s or %s, you will see LED shines.\r\n", PAD_TSI_ELECTRODE_1_NAME,
           PAD_TSI_ELECTRODE_2_NAME);
    TSI_EnableModule(TSI0, false); /* Disable module */
    TSI_EnablePeriodicalScan(TSI0, true);
    TSI_EnableInterrupts(TSI0, kTSI_GlobalInterruptEnable);
    TSI_EnableInterrupts(TSI0, kTSI_EndOfScanInterruptEnable);
    TSI_ClearStatusFlags(TSI0, kTSI_EndOfScanFlag);

    TSI_EnableChannel(TSI0, BOARD_TSI_ELECTRODE_1, true); /* Select electrode 1 as detecting electrode. */
    TSI_EnableModule(TSI0, true);                         /* Enable module, the hardware trigger scan begins */

    while (1)
    {
    }
}
