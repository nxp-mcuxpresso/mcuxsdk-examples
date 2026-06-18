/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
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
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
int main(void)
{
    sctimer_config_t sctimerInfo;
    sctimer_pwm_signal_param_t pwmParam;
    uint32_t stateNumber;
    uint32_t periodEvent, pulseEventFirst, pulseEventSecond, eventNumberInput;
    uint32_t sctimerClock;

    /* Board pin, clock, debug console init */
    BOARD_InitHardware();

    PRINTF("MCUX SDK version: %s\r\n", MCUXSDK_VERSION_FULL_STR);

    sctimerClock = SCTIMER_CLK_FREQ;

    /* Print a note to terminal */
    PRINTF("\r\nSCTimer example to output edge-aligned PWM signal\r\n");
    PRINTF("\r\nWhen user presses a switch the PWM signal will be seen from Out %d  ",
           (uint32_t)DEMO_SECOND_SCTIMER_OUT);
    PRINTF("\r\nWhen user presses the switch again PWM signal on Out %d will turn off ",
           (uint32_t)DEMO_SECOND_SCTIMER_OUT);
    PRINTF("\r\nThe PWM signal from Out %d will remain active all the time ", (uint32_t)DEMO_FIRST_SCTIMER_OUT);

    /* Default configuration operates the counter in 32-bit mode */
    SCTIMER_GetDefaultConfig(&sctimerInfo);

    /* Initialize SCTimer module */
    SCTIMER_Init(SCT0, &sctimerInfo);

    stateNumber = SCTIMER_GetCurrentState(SCT0);

    /* Configure PWM params with frequency 24kHZ from first output */
    pwmParam.output           = DEMO_FIRST_SCTIMER_OUT;
    pwmParam.level            = kSCTIMER_HighTrue;
    pwmParam.dutyCyclePercent = 10;

    /* Schedule events in current state; State 0. Generate a 24 kHz PWM with 10% duty cycle from the first
     * out in the current state.
     */
    if (SCTIMER_SetupSharedPeriodPwm(SCT0, &pwmParam, kSCTIMER_EdgeAlignedPwm, 24000U, sctimerClock, &periodEvent,
                                     &pulseEventFirst) != kStatus_Success)
    {
        return -1;
    }

    /* Schedule an event to look for a rising edge on input 1 in this state */
    if (SCTIMER_CreateAndScheduleEvent(SCT0, kSCTIMER_InputRiseEvent, 0, kSCTIMER_Input_1, kSCTIMER_Counter_U,
                                       &eventNumberInput) == kStatus_Fail)
    {
        return -1;
    }

    /* Transition to next state when a rising edge is detected on input 1 */
    SCTIMER_SetupNextStateActionwithLdMethod(SCT0, stateNumber + 1, eventNumberInput, true);

    /* Go to next state; State 1 */
    SCTIMER_IncreaseState(SCT0);

    /* Schedule events in State 1. Generate a 24 kHz PWM with 50% duty cycle from the second Out in this new state. */
    pwmParam.output           = DEMO_SECOND_SCTIMER_OUT;
    pwmParam.dutyCyclePercent = 50;
    if (SCTIMER_SetupSharedPeriodPwm(SCT0, &pwmParam, kSCTIMER_EdgeAlignedPwm, 24000U, sctimerClock, &periodEvent,
                                     &pulseEventSecond) != kStatus_Success)
    {
        return -1;
    }

    /* Keep the first Out's PWM active in State 1 too. The shared period event and the first channel's
     * pulse event were enabled in State 0 only; re-schedule both into this state (the period event is
     * shared, so it must be enabled in every state that runs any channel of the group). */
    SCTIMER_ScheduleEvent(SCT0, periodEvent);
    SCTIMER_ScheduleEvent(SCT0, pulseEventFirst);

    /* Schedule an event to look for a rising edge on input 1 in this state */
    if (SCTIMER_CreateAndScheduleEvent(SCT0, kSCTIMER_InputRiseEvent, 0, kSCTIMER_Input_1, kSCTIMER_Counter_U,
                                       &eventNumberInput) == kStatus_Fail)
    {
        return -1;
    }

    /* Transition back to State 0 when a rising edge is detected on input 1. Back in State 0 only the
     * first output's PWM is active; the second output is off. */
    SCTIMER_SetupNextStateActionwithLdMethod(SCT0, stateNumber, eventNumberInput, true);

    /* Start the 32-bit unify timer */
    SCTIMER_StartTimer(SCT0, kSCTIMER_Counter_U);

    while (1)
    {
    }
}
