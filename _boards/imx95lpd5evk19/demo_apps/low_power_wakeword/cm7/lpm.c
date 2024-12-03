/*
 * Copyright 2017-2024 NXP
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
#include "lpm.h"
#include "fsl_debug_console.h"
#include "fsl_cache.h"

#include "app_srtm.h"
#include "srtm_pdm_edma_adapter.h"
#include "scmi_cpu.h"
#include "scmi_sys.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define SYSTICK_SOURCE_CLOCK (32000000U)

void SysTick_Handler(void);

static lpm_power_mode_t s_curMode;
extern volatile app_srtm_state_t srtmState;
extern srtm_sai_adapter_t pdmAdapter;
#if defined(SRTM_DDR_RETENTION_USED) && SRTM_DDR_RETENTION_USED
extern struct dram_timing_info *timing_info;
#endif


bool LPM_Init(void)
{
    s_curMode = LPM_PowerModeRun;

    int32_t status = 0;
    scmi_pd_lpm_config_t lpmConfig[2];

    /* Keep data path from Micfil to TCM enabled when M7 is suspended. */
    lpmConfig[0].domainId = PWR_MIX_SLICE_IDX_M7;
    lpmConfig[0].lpmSetting = SCMI_CPU_LPM_SETTING_ON_ALWAYS;
    lpmConfig[0].retMask = 1U << PWR_MEM_SLICE_IDX_M7;
    lpmConfig[1].domainId = PWR_MIX_SLICE_IDX_WAKEUP;
    lpmConfig[1].lpmSetting = SCMI_CPU_LPM_SETTING_ON_ALWAYS;
    lpmConfig[1].retMask = 0U;

    /* Configure LPM setting */
    status = SCMI_CpuPdLpmConfigSet(SCMI_A2P, CPU_IDX_M7P, 2U, lpmConfig);
    if (status != 0)
    {
        PRINTF("SCMI_CpuPdLpmConfigSet SET FAIL\r\n");
    }

    return true;
}

void LPM_Deinit(void)
{
}


lpm_power_mode_t LPM_GetPowerMode(void)
{

    uint32_t runMode, sleepMode, velow, vehigh;
    int32_t status = 0;
    static int32_t prev_scmi_status = 0;

    if ((s_curMode == LPM_PowerModeRun) && (srtmState == APP_SRTM_StateLinkedUp))
    {
        s_curMode = LPM_PowerModeWait;
    }

    if ((s_curMode == LPM_PowerModeWait) && (prev_scmi_status == 0))
    {
        SCMI_CpuInfoGet(SCMI_A2P, AP_DOMAIN_LD, &runMode, &sleepMode, &velow, &vehigh);

        if (sleepMode == CPU_SLEEP_MODE_SUSPEND)
        {
            /* Configure system suspend mode.
             * sleep mode performance level = 0
             * sleep flags: keep OSC24M active
             */
            status = SCMI_SystemPowerStateSet(SCMI_A2P, 0,
                                              SCMI_SYS_STATE_MODE | 0 << 16 | SSM_OSC24M_ACTIVE_MASK);
            if (status == 0) {
                /* System sleep configured correctly - safe to suspend M7. */
                s_curMode = LPM_PowerModeSuspend;
            } else {
                /* Error configuing system sleep mode. Keep using wait to prevent M7 suspend.
                 * Also avoid calling SCMI_SystemPowerStateSet in loop.
                 */
                PRINTF("Error configuring system sleep mode (error %d) - M7 will not suspend.\r\n", status);
                prev_scmi_status = status;
            }
        }
    }

    return s_curMode;
}

/* The systick interrupt handler. */
void SYSTICK_HANDLER(void)
{
    //PRINTF("%s %d\r\n", __func__, __LINE__);
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
    xPortSysTickHandler();
}

bool LPM_WaitForInterrupt(uint32_t timeoutMilliSec)
{
    uint32_t irqMask;
    status_t status = kStatus_Success;

    irqMask = DisableGlobalIRQ();

    DisableIRQ(SYSTICK_IRQn);
    LPTMR_StopTimer(SYSTICK_BASE);

    switch (s_curMode)
    {
        case LPM_PowerModeWait:
             status = SCMI_CpuSleepModeSet(SCMI_A2P, CPU_IDX_M7P, SCMI_CPU_FLAGS_IRQ_MUX(0), SCMI_CPU_SLEEP_WAIT);
            __DSB();
            __WFI();
            __ISB();
            status = kStatus_Success;
            break;
        case LPM_PowerModeStop:
            status = SCMI_CpuSleepModeSet(SCMI_A2P, CPU_IDX_M7P, SCMI_CPU_FLAGS_IRQ_MUX(0), SCMI_CPU_SLEEP_STOP);
            __DSB();
            __WFI();
            __ISB();
            status = kStatus_Success;
            break;
        case LPM_PowerModeSuspend:
            status = SCMI_CpuSleepModeSet(SCMI_A2P, CPU_IDX_M7P, SCMI_CPU_FLAGS_IRQ_MUX(0), SCMI_CPU_SLEEP_SUSPEND);
            __DSB();
            __WFI();
            __ISB();
            status = kStatus_Success;
            break;
        default:
            break;
    }

    LPTMR_StartTimer(SYSTICK_BASE);
    NVIC_EnableIRQ(SYSTICK_IRQn);
    EnableGlobalIRQ(irqMask);

    return status == kStatus_Success;
}

void APP_PowerPreSwitchHook(lpm_power_mode_t targetMode)
{

}

void APP_PowerPostSwitchHook(lpm_power_mode_t targetMode, bool result)
{

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

/* Override the default definition of vPortSetupTimerInterrupt() that is weakly
 * defined in the FreeRTOS Cortex-M4F port layer with a version that configures LPTMR0
 * to generate the tick interrupt. */
void vPortSetupTimerInterrupt(void)
{

    lptmr_config_t lptmrConfig;

    LPTMR_GetDefaultConfig(&lptmrConfig);
    /* Select SIRC as tick timer clock source */
    lptmrConfig.prescalerClockSource = kLPTMR_PrescalerClock_2;
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

