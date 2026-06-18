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
/*!
 * @brief delay a while.
 */
void delay(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile bool sctimerIsrFlag      = false;
volatile bool brightnessUp        = true; /* Indicate LED is brighter or dimmer */
volatile uint8_t updatedDutycycle = 10U;
uint32_t eventNumberOutput; /* Shared period event (also the interrupt source) */
uint32_t pulseEventOutput;  /* This channel's pulse (duty) event */

/*******************************************************************************
 * Code
 ******************************************************************************/
void delay(void)
{
    volatile uint32_t i = 0U;
    for (i = 0U; i < 80000U; ++i)
    {
        __asm("NOP"); /* delay */
    }
}

/* The interrupt callback function is used to update the PWM dutycycle */
void SCTIMER_LED_HANDLER()
{
    sctimerIsrFlag = true;

    if (brightnessUp)
    {
        /* Increase duty cycle until it reach limited value, don't want to go upto 100% duty cycle
         * as channel interrupt will not be set for 100%
         */
        if (++updatedDutycycle >= 99U)
        {
            updatedDutycycle = 99U;
            brightnessUp     = false;
        }
    }
    else
    {
        /* Decrease duty cycle until it reach limited value */
        if (--updatedDutycycle == 1U)
        {
            brightnessUp = true;
        }
    }

    if (SCTIMER_GetStatusFlags(SCT0) & (1 << eventNumberOutput))
    {
        /* Clear interrupt flag.*/
        SCTIMER_ClearStatusFlags(SCT0, (1 << eventNumberOutput));
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    sctimer_config_t sctimerInfo;
    sctimer_pwm_signal_param_t pwmParam;
    uint32_t sctimerClock;

    /* Board pin, clock, debug console init */
    BOARD_InitHardware();

    PRINTF("MCUX SDK version: %s\r\n", MCUXSDK_VERSION_FULL_STR);

    sctimerClock = SCTIMER_CLK_FREQ;
    /* Print a note to terminal */
    PRINTF("\r\nSCTimer example to output center-aligned PWM signal\r\n");
    PRINTF("\r\nYou will see a change in LED brightness if an LED is connected to the SCTimer output pin");
    PRINTF("\r\nIf no LED is connected to the pin, then probe the signal using an oscilloscope");

    SCTIMER_GetDefaultConfig(&sctimerInfo);

    /* Initialize SCTimer module */
    SCTIMER_Init(SCT0, &sctimerInfo);

    /* Configure a single 24 kHz PWM channel. */
    pwmParam.output           = DEMO_SCTIMER_OUT;
    pwmParam.level            = kSCTIMER_HighTrue;
    pwmParam.dutyCyclePercent = updatedDutycycle;
    if (SCTIMER_SetupSharedPeriodPwm(SCT0, &pwmParam, kSCTIMER_CenterAlignedPwm, 24000U, sctimerClock,
                                     &eventNumberOutput, &pulseEventOutput) != kStatus_Success)
    {
        return -1;
    }

    /* Enable the interrupt for the PWM period event; the ISR uses it to update the duty cycle. */
    SCTIMER_EnableInterrupts(SCT0, (1 << eventNumberOutput));

    /* Receive notification when event is triggered */
    SCTIMER_SetCallback(SCT0, SCTIMER_LED_HANDLER, eventNumberOutput);

    /* Enable at the NVIC */
    EnableIRQ(SCT0_IRQn);

    /* Start the 32-bit unify timer */
    SCTIMER_StartTimer(SCT0, kSCTIMER_Counter_U);

    /* Periodically update the PWM duty cycle from the main loop. */
    while (1)
    {
        /* The ISR sets sctimerIsrFlag each PWM period; apply the next duty cycle when it does. */
        if (true == sctimerIsrFlag)
        {
            /* Disable interrupt to retain current dutycycle for a few seconds */
            SCTIMER_DisableInterrupts(SCT0, (1 << eventNumberOutput));

            sctimerIsrFlag = false;

            /* Update PWM duty cycle using the explicit period/pulse events, applied at the next counter
             * cycle (kSCTIMER_UpdateOnNextPeriod) so the counter is not halted and the output is glitchless. */
            SCTIMER_UpdatePwmDutycycleByEvent(SCT0, DEMO_SCTIMER_OUT, updatedDutycycle, eventNumberOutput,
                                              pulseEventOutput, kSCTIMER_UpdateOnNextPeriod);

            /* Delay to view the updated PWM dutycycle */
            delay();

            /* Enable interrupt flag to update PWM dutycycle */
            SCTIMER_EnableInterrupts(SCT0, (1 << eventNumberOutput));
        }
    }
}
