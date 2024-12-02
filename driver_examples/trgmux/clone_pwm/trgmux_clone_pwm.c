/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_lpit.h"

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

/*!
 * @brief Simple example for trgmux driver
 *
 * Observe the PWM wave of the trgmux output using oscilloscope.
 * Find the pin of trgmux out from the board schematics.
 */
int main(void)
{
    uint32_t tval;
    /* Structure of initialize LPIT */
    lpit_config_t lpitConfig;
    lpit_chnl_params_t lpitChannelConfig;

    /* Board pin, clock, debug console init */
    BOARD_InitHardware();

    LPIT_GetDefaultConfig(&lpitConfig);

    /* Init lpit module */
    LPIT_Init(LPIT0, &lpitConfig);

    lpitChannelConfig.chainChannel          = false;
    lpitChannelConfig.enableReloadOnTrigger = false;
    lpitChannelConfig.enableStartOnTrigger  = false;
    lpitChannelConfig.enableStopOnTimeout   = false;
    lpitChannelConfig.timerMode             = kLPIT_DualPeriodicCounter;
    /* Set default values for the trigger source */
    lpitChannelConfig.triggerSelect = kLPIT_Trigger_TimerChn0;
    lpitChannelConfig.triggerSource = kLPIT_TriggerSource_External;

    /* Init lpit channel 0 */
    LPIT_SetupChannel(LPIT0, kLPIT_Chnl_0, &lpitChannelConfig);

    /* Set timer period for channel 0 */
    tval = CLOCK_GetIpFreq(kCLOCK_Lpit0) / (PWM_FREQ * 2U);
    LPIT_SetTimerPeriod(LPIT0, kLPIT_Chnl_0, (tval << 16U) | tval);
    LPIT_StartTimer(LPIT0, kLPIT_Chnl_0);

    BOARD_ConfigTriggerSource();

    PRINTF("\r\nSee PWM wave on oscilloscope.");
    while (true)
    {
    }
}
