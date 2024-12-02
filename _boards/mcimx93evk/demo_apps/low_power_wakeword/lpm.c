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
#if defined(SRTM_DDR_RETENTION_USED) && SRTM_DDR_RETENTION_USED
#include "fsl_sema42.h"
#endif

#include "app_srtm.h"
#include "srtm_pdm_edma_adapter.h"

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

    /* Indicates ATF that CM33 is active. */
    W32(M33_ACTIVE_FLAG, M33_ACTIVE);

#if defined(SRTM_DDR_RETENTION_USED) && SRTM_DDR_RETENTION_USED
    /* Indicates that the DDR is currently in use by CM33 so retention is not possible. */
    CLRBIT32(DDR_RETENTION_BASE, DDR_RETENTION_M33_FLAG);
    /* Indicates that A55 can turn off the PLLs when setting DDR in retention mode.
     * The CM33 code here is able to manage by itself the PLLs it needs. */
    SETBIT32(DDR_RETENTION_BASE, DDR_RETENTION_PLL_LPM);

    SEMA42_Init(APP_SEMA42);

    timing_info = (struct dram_timing_info *)SAVED_DRAM_DATA_BASE_ADDR;
#endif

    return true;
}

void LPM_Deinit(void)
{
}

lpm_power_mode_t LPM_GetPowerMode(void)
{
    if ((s_curMode == LPM_PowerModeRun) && (srtmState == APP_SRTM_StateLinkedUp))
    {
        s_curMode = LPM_PowerModeWait;
    }

    return s_curMode;
}

bool LPM_WaitForInterrupt(uint32_t timeoutMilliSec)
{
    uint32_t irqMask;
    bool ret = true;
    uint32_t clock_backup;

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
            break;
        case LPM_PowerModeStop:
            GPC_CTRL_CM33->CM_MODE_CTRL = 0x2U;
            clock_backup                = CCM_CTRL->CLOCK_ROOT[kCLOCK_Root_M33].CLOCK_ROOT_CONTROL.RW;
            CCM_CTRL->CLOCK_ROOT[kCLOCK_Root_M33].CLOCK_ROOT_CONTROL.RW = 0; // M33 on 24M clock
            __DSB();
            __WFI();
            __ISB();
            CCM_CTRL->CLOCK_ROOT[kCLOCK_Root_M33].CLOCK_ROOT_CONTROL.RW = clock_backup;
            break;
        default:
            ret = false;
            break;
    }

    // LPTMR_StartTimer(SYSTICK_BASE);
    // NVIC_EnableIRQ(SYSTICK_IRQn);
    EnableGlobalIRQ(irqMask);

    return ret;
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

#if defined(SRTM_DDR_RETENTION_USED) && SRTM_DDR_RETENTION_USED
/*
 * Reduces system clocks
 * This has to be called only when A-core is suspended
 * and under protection of SEMA42 in case A-core wakes-up at the same time
 */

void LPM_ocram_access_cb(bool enable)
{
    /* Lock gate 0 with Processor ID 8 */
    SEMA42_Lock(APP_SEMA42, APP_SEMA42_GATE, APP_CORTEX_M_DID);
    if (R32(DDR_RETENTION_BASE) & DDR_RETENTION_A55_FLAG)
    {
        if (enable)
        {
            /* 24M clock when access to OCRAM is needed while A-core is suspended. */
            CCM_CTRL->CLOCK_ROOT[kCLOCK_Root_WakeupAxi].CLOCK_ROOT_CONTROL.RW = 0;
            CCM_CTRL->CLOCK_ROOT[kCLOCK_Root_BusWakeup].CLOCK_ROOT_CONTROL.RW = 0;
            CCM_CTRL->CLOCK_ROOT[kCLOCK_Root_Nic].CLOCK_ROOT_CONTROL.RW       = 0;
        }
        else
        {
            /* reduce bus clocks when access to OCRAM is not needed. */
            CCM_CTRL->CLOCK_ROOT[kCLOCK_Root_WakeupAxi].CLOCK_ROOT_CONTROL.RW = 0x03;
            CCM_CTRL->CLOCK_ROOT[kCLOCK_Root_BusWakeup].CLOCK_ROOT_CONTROL.RW = 0x03;
            CCM_CTRL->CLOCK_ROOT[kCLOCK_Root_Nic].CLOCK_ROOT_CONTROL.RW       = 0x03;
        }
    }
    /* unlock gate */
    SEMA42_Unlock(APP_SEMA42, APP_SEMA42_GATE);
}

uint8_t clocks_to_24M[] = {
    kCLOCK_Root_BusWakeup,
    kCLOCK_Root_BusAon,
    kCLOCK_Root_WakeupAxi,
    kCLOCK_Root_Nic,
};

uint8_t osc_lpm_settings[] = {
    1, // SYSTEM PLL
    0, // SYSTEM PLL PFD0
    0, // SYSTEM PLL PFD0 CLK
    0, // SYSTEM PLL PFD0 DIV2 CLK
    1, // SYSTEM PLL PFD1
    0, // SYSTEM PLL PFD1 CLK
    1, // SYSTEM PLL PFD1 DIV2 CLK
    0, // SYSTEM PLL PFD2
    0, // SYSTEM PLL PFD2 CLK
    0, // SYSTEM PLL PFD2 DIV2 CLK
    3, // AUDIO PLL
    3, // AUDIO PLL CLK
    0, // DRAM PLL
    0, // DRAM PLL CLK
    0, // VIDEO PLL
    0  // VIDEO PLL CLK
};

static void LPM_low_clocks_setting(bool low, bool apply_settings)
{
    static uint32_t clock_backup[4] = {0};
    static uint32_t last_status     = 0;
    static uint32_t iomux_backup    = 0;

    if (last_status != low)
    {
        if (low)
        {
            if (apply_settings != 0)
            {
                // change mux of unused pad during suspend but draining lot of current to a pull up
                iomux_backup                = IOMUXC1->SW_MUX_CTL_PAD[91];
                IOMUXC1->SW_MUX_CTL_PAD[91] = 0;

                // change some clocks to 24M so the source PLLs can be turned off
                for (int i = 0; i < sizeof(clocks_to_24M); i++)
                {
                    clock_backup[i] = CCM_CTRL->CLOCK_ROOT[clocks_to_24M[i]].CLOCK_ROOT_CONTROL.RW;
                    CCM_CTRL->CLOCK_ROOT[clocks_to_24M[i]].CLOCK_ROOT_CONTROL.RW = 0;
                }

                // configure PLLs for LPM mode
                for (int i = 3; i < 17; i++)
                {
                    CCM_CTRL->OSCPLL[i].LPM0 = (CCM_CTRL->OSCPLL[i].LPM0 & ~CCM_OSCPLL_LPM0_LPM_SETTING_D2_MASK) |
                                               CCM_OSCPLL_LPM0_LPM_SETTING_D2(osc_lpm_settings[i - 3]);
                }
                DRAMPLL->CTRL.CLR = PLL_CTRL_POWERUP_MASK;
            }
            // enable callback to reduce power when buffer in ocram is not used
            SRTM_PdmEdmaAdapter_SetRxExtBufAccessCb(pdmAdapter, LPM_ocram_access_cb);

            s_curMode = LPM_PowerModeStop;
        }
        else
        {
            s_curMode = LPM_PowerModeWait;

            // prevent reducing power when buffer in ocram is not used by SRTM
            SRTM_PdmEdmaAdapter_SetRxExtBufAccessCb(pdmAdapter, NULL);

            if (apply_settings != 0)
            {
                // restore pad settings
                IOMUXC1->SW_MUX_CTL_PAD[91] = iomux_backup;

                // restore modified clocks to their initial value
                for (int i = 0; i < sizeof(clocks_to_24M); i++)
                {
                    CCM_CTRL->CLOCK_ROOT[clocks_to_24M[i]].CLOCK_ROOT_CONTROL.RW = clock_backup[i];
                }

                // restore all PLLs to always active
                DRAMPLL->CTRL.SET = PLL_CTRL_POWERUP_MASK;
            }
            // Need to do this even if A-core already disabled auto LPM to avoid issues at beginning of next sleep
            for (int i = 3; i < 17; i++)
            {
                CCM_CTRL->OSCPLL[i].LPM0 |= CCM_OSCPLL_LPM0_LPM_SETTING_D2(3);
            }
        }
        last_status = low;
    }
}
#endif

void APP_PowerPreSwitchHook(lpm_power_mode_t targetMode)
{
#if defined(SRTM_DDR_RETENTION_USED) && SRTM_DDR_RETENTION_USED
    bool low_clock, apply_clock_change;

    /* Lock gate 0 with Processor ID 8 */
    SEMA42_Lock(APP_SEMA42, APP_SEMA42_GATE, APP_CORTEX_M_DID);

    /* Check if clocks can be lowered */
    if (R32(DDR_RETENTION_BASE) & DDR_RETENTION_A55_FLAG)
    {
        if (R32(DDR_RETENTION_BASE) & DDR_RETENTION_M33_FLAG)
        {
            // Both A55 and M33 allow DDR retention, clocks can be reduced
            low_clock          = true;
            apply_clock_change = true;
        }
        else
        {
            // M33  needs DDR,keep clocks at sufficient rate
            low_clock          = false;
            apply_clock_change = true;
        }
    }
    else
    {
        // A55 has resumed by another wakeup source, note the information
        // to keep state machine in a correct state,
        // but don't change the clocks settings
        low_clock          = false;
        apply_clock_change = false;
    }
    LPM_low_clocks_setting(low_clock, apply_clock_change);

    /* unlock gate */
    SEMA42_Unlock(APP_SEMA42, APP_SEMA42_GATE);
#endif
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

#if defined(SRTM_DDR_RETENTION_USED) && SRTM_DDR_RETENTION_USED
void LPM_ddr_pre_access(void)
{
    uint32_t ddr_retention;

    /* Prevent concurrent access from Cortex-A */
    SEMA42_Lock(APP_SEMA42, APP_SEMA42_GATE, APP_CORTEX_M_DID);

    ddr_retention = R32(DDR_RETENTION_BASE);

    // High clocks needed. Don't apply changes if A55 already did
    if (ddr_retention & DDR_RETENTION_A55_FLAG)
    {
        LPM_low_clocks_setting(false, true);
    }
    else
    {
        LPM_low_clocks_setting(false, false);
    }

    if ((ddr_retention & DDR_RETENTION_A55_FLAG) && (ddr_retention & DDR_RETENTION_M33_FLAG))
    {
        // DDR is in retention since both cores were allowing it.
        BOARD_DRAMExitRetention();
    }
    CLRBIT32(DDR_RETENTION_BASE, DDR_RETENTION_M33_FLAG); // M33 need DDR - no retention

    /* release access */
    SEMA42_Unlock(APP_SEMA42, APP_SEMA42_GATE);
}

void LPM_ddr_post_access(void)
{
    uint32_t ddr_retention;

    /* Prevent concurrent access from Cortex-A */
    SEMA42_Lock(APP_SEMA42, APP_SEMA42_GATE, APP_CORTEX_M_DID);

    ddr_retention = R32(DDR_RETENTION_BASE);

    if ((ddr_retention & DDR_RETENTION_A55_FLAG) && !(ddr_retention & DDR_RETENTION_M33_FLAG))
    {
        // Cortex-A allows retention and CM33 was preventing it until now.
        BOARD_DRAMEnterRetention();
    }

    // DDR no more needed, reduce the clocks if A55 also allows DDR retention
    if (ddr_retention & DDR_RETENTION_A55_FLAG)
    {
        LPM_low_clocks_setting(true, true);
    }

    SETBIT32(DDR_RETENTION_BASE, DDR_RETENTION_M33_FLAG); // M33 don't need DDR - rentention possible

    /* release access */
    SEMA42_Unlock(APP_SEMA42, APP_SEMA42_GATE);
}

#endif
