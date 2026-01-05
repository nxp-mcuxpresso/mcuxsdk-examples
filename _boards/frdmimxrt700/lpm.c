/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#if defined(__ICCARM__)
#include <intrinsics.h>
#endif

#include "fsl_power.h"
#include "fsl_ostimer.h"
#include "lpm.h"

/*
 * The number of SysTick increments that make up one tick period.
 */
static uint32_t ulTimerCountsForOneTick         = 0U;
static uint32_t xMaximumPossibleSuppressedTicks = 0U;

/*
 * The number of LPTIMER increments that make up one tick period.
 */
static uint32_t ulLPTimerCountsForOneTick = 0U;

extern uint32_t SystemCoreClock; /* in Kinetis SDK, this contains the system core clock speed */

TickType_t LPM_EnterTicklessIdle(TickType_t xExpectedIdleTime, uint64_t *pCounter)
{
    uint64_t xOstimerStartValue = 0U;
    status_t status;
    uint32_t ulReloadValue;

    assert(configTICK_RATE_HZ <= 1000U);

    /* If timeout < 2 ticks, don't do tickless idle. ulReloadValue can't be 0. */
    if (xExpectedIdleTime < 2U)
    {
        return 0U;
    }

    /* Make sure the SysTick reload value does not overflow the counter. */
    if (xExpectedIdleTime > xMaximumPossibleSuppressedTicks)
    {
        xExpectedIdleTime = xMaximumPossibleSuppressedTicks;
    }

    /* Stop the LPTimer and systick momentarily. The time the LPTimer and systick is stopped for
    is accounted for as best it can be, but using the tickless mode will inevitably result in some tiny drift of the
    time maintained by the kernel with respect to calendar time. */
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;

    if ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) != 0U)
    {
        NVIC_ClearPendingIRQ(SysTick_IRQn);
    }

    /* Calculate the reload value required to wait xExpectedIdleTime
    tick periods.  -1 is used because this code will execute part way
    through one of the tick periods. */
    assert(xExpectedIdleTime < (UINT32_MAX / ulLPTimerCountsForOneTick + 1UL));
    ulReloadValue = ulLPTimerCountsForOneTick * (xExpectedIdleTime - 1UL);

    OSTIMER_Init(TICKLESS_OSTIMER_BASE_PTR);
    OSTIMER_ClearStatusFlags(TICKLESS_OSTIMER_BASE_PTR, kOSTIMER_MatchInterruptFlag);
    EnableDeepSleepIRQ(TICKLESS_OSTIMER_IRQn);

    xOstimerStartValue = OSTIMER_GetCurrentTimerValue(TICKLESS_OSTIMER_BASE_PTR);
    assert((xOstimerStartValue + ulReloadValue) < UINT64_MAX);
    status = OSTIMER_SetMatchValue(TICKLESS_OSTIMER_BASE_PTR, (xOstimerStartValue + ulReloadValue), NULL);

    if (status != kStatus_Success)
    {
        assert(false);
    }

    *pCounter = xOstimerStartValue;
    return xExpectedIdleTime;
}

void LPM_ExitTicklessIdle(TickType_t xExpectedIdleTime, uint64_t base)
{
    uint32_t ulCompleteTickPeriods;

    assert(xExpectedIdleTime >= 1U);

    if ((OSTIMER_GetStatusFlags(TICKLESS_OSTIMER_BASE_PTR) & (uint32_t)kOSTIMER_MatchInterruptFlag) == 0U)
    {
        /* Not woken up from OSTMR, further alarm is not needed. */
        (void)DisableIRQ(OS_EVENT_IRQn);
        ulCompleteTickPeriods =
            (OSTIMER_GetCurrentTimerValue(TICKLESS_OSTIMER_BASE_PTR) - base) / ulLPTimerCountsForOneTick;
    }
    else
    {
        OSTIMER_ClearStatusFlags(TICKLESS_OSTIMER_BASE_PTR, kOSTIMER_MatchInterruptFlag);
        NVIC_ClearPendingIRQ(OS_EVENT_IRQn);
        ulCompleteTickPeriods = xExpectedIdleTime - 1UL;
    }

    /* Stop LPTimer when CPU waked up then set SysTick->LOAD back to its standard
        value.  The critical section is used to ensure the tick interrupt
        can only execute once in the case that the reload register is near
        zero. */
    OSTIMER_Deinit(TICKLESS_OSTIMER_BASE_PTR);
    DisableDeepSleepIRQ(TICKLESS_OSTIMER_IRQn);
    portENTER_CRITICAL();
    {
        SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
        vTaskStepTick(ulCompleteTickPeriods);
        SysTick->LOAD = ulTimerCountsForOneTick - 1UL;
    }
    portEXIT_CRITICAL();
}

/*
 * Setup the systick timer to generate the tick interrupts at the required
 * frequency.
 */
void vPortSetupTimerInterrupt(void)
{
    /* Calculate the constants required to configure the tick interrupt. */
    ulTimerCountsForOneTick   = (configCPU_CLOCK_HZ / configTICK_RATE_HZ);
    ulLPTimerCountsForOneTick = (configOSTIMER_CLOCK_HZ / configTICK_RATE_HZ);
    if (ulLPTimerCountsForOneTick == 0)
    {
        /* ulLPTimerCountsForOneTick is zero, not allowed state */
        while (1)
            ;
    }
    else
    {
        xMaximumPossibleSuppressedTicks = portMAX_32_BIT_NUMBER / ulLPTimerCountsForOneTick;
    }

    /* Configure SysTick to interrupt at the requested rate. */
    SysTick->LOAD = (configCPU_CLOCK_HZ / configTICK_RATE_HZ) - 1UL;
    SysTick->VAL  = 0UL;
    SysTick->CTRL = (SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk);
}