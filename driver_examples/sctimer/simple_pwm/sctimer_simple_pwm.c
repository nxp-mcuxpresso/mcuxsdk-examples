/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019, 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_sctimer.h"

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
 * @brief Main function
 */
int main(void)
{
    sctimer_config_t sctimerInfo;
    sctimer_pwm_signal_param_t pwmParam;
    uint32_t periodEvent;
    uint32_t pulseEvent;
    uint32_t sctimerClock;

    /* Board pin, clock, debug console init */
    BOARD_InitHardware();

    PRINTF("MCUX SDK version: %s\r\n", MCUXSDK_VERSION_FULL_STR);

    sctimerClock = SCTIMER_CLK_FREQ;

    /* Print a note to terminal */
    PRINTF("\r\nSCTimer example to output 2 center-aligned PWM signals\r\n");
    PRINTF("\r\nProbe the signal using an oscilloscope");

    SCTIMER_GetDefaultConfig(&sctimerInfo);

    /* Initialize SCTimer module */
    SCTIMER_Init(SCT0, &sctimerInfo);

    /* Both channels run at the same 24 kHz frequency. Each SCTIMER_SetupSharedPeriodPwm() call
     * configures one channel from pwmParam: the first call creates the shared period/limit event and
     * that channel's pulse event; the second call reuses the same period event and adds only its own
     * pulse event. pwmParam is consumed during each call, so it is reused for the second channel, and
     * the returned event numbers are not needed after setup, so pulseEvent is reused as well. */
    pwmParam.output           = DEMO_FIRST_SCTIMER_OUT;
    pwmParam.level            = kSCTIMER_HighTrue;
    pwmParam.dutyCyclePercent = DUTY_CYCLE_CH1;
    if (SCTIMER_SetupSharedPeriodPwm(SCT0, &pwmParam, PWM_MODE, 24000U, sctimerClock, &periodEvent,
                                     &pulseEvent) != kStatus_Success)
    {
        return -1;
    }

    pwmParam.output           = DEMO_SECOND_SCTIMER_OUT;
    pwmParam.level            = kSCTIMER_LowTrue;
    pwmParam.dutyCyclePercent = DUTY_CYCLE_CH2;
    if (SCTIMER_SetupSharedPeriodPwm(SCT0, &pwmParam, PWM_MODE, 24000U, sctimerClock, &periodEvent,
                                     &pulseEvent) != kStatus_Success)
    {
        return -1;
    }

    /* Start the 32-bit unify timer */
    SCTIMER_StartTimer(SCT0, kSCTIMER_Counter_U);

    while (1)
    {
    }
}
