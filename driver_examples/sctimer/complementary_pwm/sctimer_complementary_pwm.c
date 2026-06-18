/*
 * Copyright 2026 NXP
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
#define DEMO_PWM_FREQ_HZ    (20000U)
#define DEMO_DEADTIME_TICKS (120U)
#define DEMO_DUTY_MIN_PERCENT (10U)
#define DEMO_DUTY_MAX_PERCENT (90U)
/* Hold each duty step long enough to see it on a scope. */
#define DEMO_DUTY_STEP_HOLD_US (50000U)

/*******************************************************************************
 * Variables
 ******************************************************************************/
static volatile bool sctimerIsrFlag      = false;
static volatile bool dutyRising          = true;
static volatile uint8_t pair1DutyPercent = DEMO_DUTY_MIN_PERCENT;
/* Shared period event (interrupt source); set in main once the pairs are configured. */
static uint32_t periodEventNumber;

/*******************************************************************************
 * Code
 ******************************************************************************/
/* Fires on the shared period event once per PWM period; ramps the duty cycle up and down. */
void SCTIMER_DUTY_HANDLER(void)
{
    sctimerIsrFlag = true;

    if (dutyRising)
    {
        if (pair1DutyPercent < DEMO_DUTY_MAX_PERCENT)
        {
            ++pair1DutyPercent;
        }
        if (pair1DutyPercent >= DEMO_DUTY_MAX_PERCENT)
        {
            dutyRising = false;
        }
    }
    else
    {
        if (pair1DutyPercent > DEMO_DUTY_MIN_PERCENT)
        {
            --pair1DutyPercent;
        }
        if (pair1DutyPercent <= DEMO_DUTY_MIN_PERCENT)
        {
            dutyRising = true;
        }
    }

    if (0U != (SCTIMER_GetStatusFlags(SCT0) & (1UL << periodEventNumber)))
    {
        SCTIMER_ClearStatusFlags(SCT0, (1UL << periodEventNumber));
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    sctimer_config_t sctimerInfo;
    sctimer_complementary_pwm_handle_t handle1, handle2;
    uint32_t sctClock;

    /* Board pin, clock, debug console init */
    BOARD_InitHardware();

    /* Read the SCT clock after the board clocks are configured. */
    sctClock = SCTIMER_CLK_FREQ;

    (void)PRINTF("MCUX SDK version: %s\r\n", MCUXSDK_VERSION_FULL_STR);
    (void)PRINTF("\r\nSCTimer complementary PWM + dead-time example\r\n");
    (void)PRINTF("Two center-aligned pairs, %u Hz, dead time %u ticks; duty ramps %u%%-%u%%.\r\n",
                 (unsigned)DEMO_PWM_FREQ_HZ, (unsigned)DEMO_DEADTIME_TICKS, (unsigned)DEMO_DUTY_MIN_PERCENT,
                 (unsigned)DEMO_DUTY_MAX_PERCENT);

    SCTIMER_GetDefaultConfig(&sctimerInfo);
    (void)SCTIMER_Init(SCT0, &sctimerInfo);

    /* Two center-aligned complementary pairs on one SCTimer; both share the unified counter, so they use
     * the same frequency and alignment. Pair 2 reuses pair 1's period/limit event. */
    if (SCTIMER_SetupComplementaryPwm(SCT0, DEMO_PAIR1_OUT_HIGH, DEMO_PAIR1_OUT_LOW, DEMO_DUTY_MIN_PERCENT,
                                      DEMO_DEADTIME_TICKS, kSCTIMER_CenterAlignedPwm, DEMO_PWM_FREQ_HZ, sctClock,
                                      &handle1) != kStatus_Success)
    {
        (void)PRINTF("Setup pair 1 failed\r\n");
        return -1;
    }

    if (SCTIMER_SetupComplementaryPwm(SCT0, DEMO_PAIR2_OUT_HIGH, DEMO_PAIR2_OUT_LOW,
                                      (uint8_t)(100U - DEMO_DUTY_MIN_PERCENT), DEMO_DEADTIME_TICKS,
                                      kSCTIMER_CenterAlignedPwm, DEMO_PWM_FREQ_HZ, sctClock, &handle2) != kStatus_Success)
    {
        (void)PRINTF("Setup pair 2 failed\r\n");
        return -1;
    }

    /* Both pairs share one period event; use it as the per-period interrupt that paces the duty ramp. */
    periodEventNumber = handle1.periodEvent;
    SCTIMER_SetCallback(SCT0, &SCTIMER_DUTY_HANDLER, periodEventNumber);
    SCTIMER_EnableInterrupts(SCT0, (1UL << periodEventNumber));
    (void)EnableIRQ(SCT0_IRQn);

    SCTIMER_StartTimer(SCT0, (uint32_t)kSCTIMER_Counter_U);

    while (true)
    {
        if (sctimerIsrFlag)
        {
            /* Freeze the duty value while this step is held by disabling the period interrupt. */
            SCTIMER_DisableInterrupts(SCT0, (1UL << periodEventNumber));
            sctimerIsrFlag = false;

            /* Update both pairs glitchlessly at the next cycle boundary (reload-only, counter not halted).
             * Pair 2 mirrors pair 1 so the two pairs ramp in opposite directions. */
            (void)SCTIMER_UpdateComplementaryPwmDutycycle(SCT0, &handle1, pair1DutyPercent,
                                                          kSCTIMER_UpdateOnNextPeriod);
            (void)SCTIMER_UpdateComplementaryPwmDutycycle(SCT0, &handle2, (uint8_t)(100U - pair1DutyPercent),
                                                          kSCTIMER_UpdateOnNextPeriod);

            SDK_DelayAtLeastUs(DEMO_DUTY_STEP_HOLD_US, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);

            SCTIMER_EnableInterrupts(SCT0, (1UL << periodEventNumber));
        }
    }
}
