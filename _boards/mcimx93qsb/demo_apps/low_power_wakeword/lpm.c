/*
 * Copyright 2017-2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_lptmr.h"
#include "fsl_mu.h"
#include "fsl_sentinel.h"
#include "lpm.h"
#include "fsl_debug_console.h"
#include "fsl_cache.h"

#include "app_srtm.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define SYSTICK_SOURCE_CLOCK (32000000U)

void SysTick_Handler(void);

static lpm_power_mode_t s_curMode;
extern volatile app_srtm_state_t srtmState;

bool LPM_Init(void)
{
    s_curMode = LPM_PowerModeRun;

    // Request ATF to let DDR active when Cortex-A is suspended
    *((uint32_t *)M33_ACTIVE_FLAG) = M33_ACTIVE;
    return true;
}

void LPM_Deinit(void)
{
}

lpm_power_mode_t LPM_GetPowerMode(void)
{
    //    if (srtmState == APP_SRTM_StateLinkedUp)
    //    {
    //        s_curMode = LPM_PowerModeSuspend;
    //    }

    return s_curMode;
}

bool LPM_WaitForInterrupt(uint32_t timeoutMilliSec)
{
    uint32_t irqMask;
    status_t status = kStatus_Success;

    irqMask = DisableGlobalIRQ();

    // DisableIRQ(SYSTICK_IRQn);
    // LPTMR_StopTimer(SYSTICK_BASE);

    switch (s_curMode)
    {
        case LPM_PowerModeWait:
            GPC_CTRL_CM33->CM_MODE_CTRL = 0x1U;
            __DSB();
            __WFI();
            __ISB();
            status = kStatus_Success;
            break;
        case LPM_PowerModeStop:
            GPC_CTRL_CM33->CM_MODE_CTRL = 0x2U;
            __DSB();
            __WFI();
            __ISB();
            status = kStatus_Success;
            break;
        case LPM_PowerModeSuspend:
            GPC_CTRL_CM33->CM_MODE_CTRL |= GPC_CPU_CTRL_CM_MODE_CTRL_CPU_MODE_TARGET_MASK;
            __DSB();
            __WFI();
            __ISB();
            status = kStatus_Success;
            break;
        default:
            break;
    }

    // LPTMR_StartTimer(SYSTICK_BASE);
    // NVIC_EnableIRQ(SYSTICK_IRQn);
    EnableGlobalIRQ(irqMask);

    return status == kStatus_Success;
}

/************ Internal public API start **************/
/* The systick interrupt handler. */
void SYSTICK_HANDLER(void)
{
    /* Clear the interrupt. */
    LPTMR_ClearStatusFlags(SYSTICK_BASE, kLPTMR_TimerCompareFlag);

    if (SYSTICK_BASE->CSR & LPTMR_CSR_TFC_MASK)
    {
        /* Freerun timer means this is the first tick after tickless exit. */
        LPTMR_StopTimer(SYSTICK_BASE);
        LPTMR_SetTimerPeriod(SYSTICK_BASE, SYSTICK_SOURCE_CLOCK / configTICK_RATE_HZ);
        SYSTICK_BASE->CSR &= ~LPTMR_CSR_TFC_MASK;
        LPTMR_StartTimer(SYSTICK_BASE);
    }
    /* Call FreeRTOS tick handler. */
    vPortSysTickHandler();
}

/* Override the default definition of vPortSetupTimerInterrupt() that is weakly
 * defined in the FreeRTOS Cortex-M4F port layer with a version that configures LPTMR0
 * to generate the tick interrupt. */
void vPortSetupTimerInterrupt(void)
{
    lptmr_config_t lptmrConfig;

    /*
     * lptmrConfig.timerMode = kLPTMR_TimerModeTimeCounter;
     * lptmrConfig.pinSelect = kLPTMR_PinSelectInput_0;
     * lptmrConfig.pinPolarity = kLPTMR_PinPolarityActiveHigh;
     * lptmrConfig.enableFreeRunning = false;
     * lptmrConfig.bypassPrescaler = true;
     * lptmrConfig.prescalerClockSource = kLPTMR_PrescalerClock_1;
     * lptmrConfig.value = kLPTMR_Prescale_Glitch_0;
     */
    LPTMR_GetDefaultConfig(&lptmrConfig);
    /* Select SIRC as tick timer clock source */
    lptmrConfig.prescalerClockSource = kLPTMR_PrescalerClock_1;
    /* Initialize the LPTMR */
    LPTMR_Init(SYSTICK_BASE, &lptmrConfig);

    /* Set timer period */
    LPTMR_SetTimerPeriod(SYSTICK_BASE, SYSTICK_SOURCE_CLOCK / configTICK_RATE_HZ);

    /* Enable timer interrupt */
    LPTMR_EnableInterrupts(SYSTICK_BASE, kLPTMR_TimerInterruptEnable);
    NVIC_SetPriority(SYSTICK_IRQn, configLIBRARY_LOWEST_INTERRUPT_PRIORITY);
    NVIC_EnableIRQ(SYSTICK_IRQn);

    /* Start counting */
    LPTMR_StartTimer(SYSTICK_BASE);
}

void APP_PowerPreSwitchHook(lpm_power_mode_t targetMode)
{
}

void APP_PowerPostSwitchHook(lpm_power_mode_t targetMode, bool result)
{
    if (LPM_PowerModeRun != targetMode)
    {
        /* Clear the CM MODE value. */
        GPC_CTRL_CM33->CM_MODE_CTRL = 0x0;
    }
}

void vPortSuppressTicksAndSleep(TickType_t xExpectedIdleTime)
{
    uint32_t irqMask;
    lpm_power_mode_t targetPowerMode;
    bool result;

    irqMask = DisableGlobalIRQ();

    /* Only when no context switch is pending and no task is waiting for the scheduler
     * to be unsuspended then enter low power entry.
     */
    if (eTaskConfirmSleepModeStatus() != eAbortSleep)
    {
        targetPowerMode = LPM_GetPowerMode();
        if (targetPowerMode != LPM_PowerModeRun)
        {
            /* Only wait when target power mode is not running */
            APP_PowerPreSwitchHook(targetPowerMode);
            result = LPM_WaitForInterrupt((uint64_t)1000 * xExpectedIdleTime / configTICK_RATE_HZ);
            APP_PowerPostSwitchHook(targetPowerMode, result);
        }
    }
    EnableGlobalIRQ(irqMask);
}
