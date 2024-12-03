/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "timers.h"
#include "fsl_rgpio.h"
#include "fsl_lptmr.h"
#include "fsl_upower.h"
#include "fsl_mu.h"
#include "fsl_debug_console.h"

#include "board.h"
#include "app.h"
#include "app_srtm.h"
#include "lpm.h"
#include "power_mode_switch.h"
#include "fsl_rtd_cmc.h"
#include "fsl_sentinel.h"
#include "fsl_rgpio.h"
#include "fsl_wuu.h"

/*******************************************************************************
 * Struct Definitions
 ******************************************************************************/
#define WUU_WAKEUP_PIN_IDX     (24U) /* WUU0_P24 used for RTD Button2 (SW8) */
#define WUU_WAKEUP_PIN_TYPE    kWUU_ExternalPinFallingEdge
#define APP_WAKEUP_BUTTON_NAME "RTD BUTTON2 (SW8)"

typedef enum _app_wakeup_source
{
    kAPP_WakeupSourceLptmr, /*!< Wakeup by LPTMR.        */
    kAPP_WakeupSourcePin    /*!< Wakeup by external pin. */
} app_wakeup_source_t;

/*******************************************************************************
 * Function Prototypes
 ******************************************************************************/
extern void APP_PowerPreSwitchHook(lpm_rtd_power_mode_e targetMode);
extern void APP_PowerPostSwitchHook(lpm_rtd_power_mode_e targetMode, bool result);
extern void APP_SRTM_WakeupCA35(void);
extern void APP_RebootCA35(void);
extern void APP_ShutdownCA35(void);
extern void APP_BootCA35(void);
extern void UPOWER_InitBuck2Buck3Table(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
static uint32_t s_wakeupTimeout;           /* Wakeup timeout. (Unit: Second) */
static app_wakeup_source_t s_wakeupSource; /* Wakeup source.                 */
static SemaphoreHandle_t s_wakeupSig;
static const char *s_modeNames[] = {"ACTIVE", "WAIT", "STOP", "Sleep", "Deep Sleep", "Power Down", "Deep Power Down"};
extern lpm_ad_power_mode_e AD_CurrentMode;
extern bool option_v_boot_flag;
extern lpm_rtd_power_mode_e s_curMode;
extern lpm_rtd_power_mode_e s_lastMode;
extern pca9460_buck3ctrl_t buck3_ctrl;
extern pca9460_ldo1_cfg_t ldo1_cfg;
extern bool wake_acore_flag;
rtd_mode_and_irq_allow_t current_state = {LPM_PowerModeActive, LPM_PowerModeActive, NotAvail_IRQn, RTD_GIVE_SIG_YES};
// clang-format off
/*
 * For some system low power combine, such as APD->PD, RTD->PD, use GPIO as RTD wakeup source, it will trigger WUU + GPIO irq handler in RTD side, release twice Semaphore Sig.
 * In below mode_combi_array_for_give_sig table, use current_rtd_mode and irq_num to judge whether RTD give semaphore sig.
 * Use current_rtd_mode and last_rtd_mode to judge whether RTD can wakeup APD.
 */
rtd_mode_and_irq_allow_t mode_combi_array_for_give_sig[] = {
  {LPM_PowerModeActive, LPM_PowerModeActive, NotAvail_IRQn, RTD_GIVE_SIG_YES},
  {LPM_PowerModeDeepSleep, LPM_PowerModeActive, WUU0_IRQn, RTD_GIVE_SIG_YES},
  {LPM_PowerModePowerDown, LPM_PowerModeActive, WUU0_IRQn, RTD_GIVE_SIG_YES},
  {LPM_PowerModeDeepPowerDown, LPM_PowerModeActive, WUU0_IRQn, RTD_GIVE_SIG_YES},

  {LPM_PowerModeWait, LPM_PowerModeActive, LPTMR1_IRQn, RTD_GIVE_SIG_YES},
  {LPM_PowerModeStop, LPM_PowerModeActive, LPTMR1_IRQn, RTD_GIVE_SIG_YES},
  {LPM_PowerModeSleep, LPM_PowerModeActive, LPTMR1_IRQn, RTD_GIVE_SIG_YES},
  {LPM_PowerModeDeepSleep, LPM_PowerModeActive, LPTMR1_IRQn, RTD_GIVE_SIG_YES},
  {LPM_PowerModePowerDown, LPM_PowerModeActive, LPTMR1_IRQn, RTD_GIVE_SIG_YES},

  {LPM_PowerModeWait, LPM_PowerModeActive, GPIOB_INT0_IRQn, RTD_GIVE_SIG_YES},
  {LPM_PowerModeStop, LPM_PowerModeActive, GPIOB_INT0_IRQn, RTD_GIVE_SIG_YES},
  {LPM_PowerModeSleep, LPM_PowerModeActive, GPIOB_INT0_IRQn, RTD_GIVE_SIG_YES},
  {LPM_PowerModeDeepSleep, LPM_PowerModeActive, GPIOB_INT0_IRQn, RTD_GIVE_SIG_YES},
};

mode_combi_t mode_combi_array_for_single_boot[] = {
    {LPM_PowerModeActive, AD_ACT, MODE_COMBI_YES},
    {LPM_PowerModeWait, AD_ACT, MODE_COMBI_YES},
    {LPM_PowerModeStop, AD_ACT, MODE_COMBI_YES},
    {LPM_PowerModeSleep, AD_ACT, MODE_COMBI_YES},
    {LPM_PowerModeDeepSleep, AD_ACT, MODE_COMBI_NO},
    {LPM_PowerModePowerDown, AD_ACT, MODE_COMBI_NO},
    {LPM_PowerModeDeepPowerDown, AD_ACT, MODE_COMBI_NO},

    {LPM_PowerModeActive, AD_DSL, MODE_COMBI_YES},
    {LPM_PowerModeWait, AD_DSL, MODE_COMBI_YES},
    {LPM_PowerModeStop, AD_DSL, MODE_COMBI_YES},
    {LPM_PowerModeSleep, AD_DSL, MODE_COMBI_YES},
    {LPM_PowerModeDeepSleep, AD_DSL, MODE_COMBI_YES},
    {LPM_PowerModePowerDown, AD_DSL, MODE_COMBI_NO},
    {LPM_PowerModeDeepPowerDown, AD_DSL, MODE_COMBI_NO},

    {LPM_PowerModeActive, AD_PD, MODE_COMBI_YES},
    {LPM_PowerModeWait, AD_PD, MODE_COMBI_YES},
    {LPM_PowerModeStop, AD_PD, MODE_COMBI_YES},
    {LPM_PowerModeSleep, AD_PD, MODE_COMBI_YES},
    {LPM_PowerModeDeepSleep, AD_PD, MODE_COMBI_YES},
    {LPM_PowerModePowerDown, AD_PD, MODE_COMBI_YES},
    {LPM_PowerModeDeepPowerDown, AD_PD, MODE_COMBI_NO},

    {LPM_PowerModeActive, AD_DPD, MODE_COMBI_YES},
    {LPM_PowerModeWait, AD_DPD, MODE_COMBI_YES},
    {LPM_PowerModeStop, AD_DPD, MODE_COMBI_YES},
    {LPM_PowerModeSleep, AD_DPD, MODE_COMBI_YES},
    {LPM_PowerModeDeepSleep, AD_DPD, MODE_COMBI_YES},
    {LPM_PowerModePowerDown, AD_DPD, MODE_COMBI_YES},
    /* RTD not support Deep Power Down Mode when boot type is SINGLE BOOT TYPE */
    {LPM_PowerModeDeepPowerDown, AD_DPD, MODE_COMBI_NO},
};

mode_combi_t mode_combi_array_for_dual_or_lp_boot[] = {
    {LPM_PowerModeActive, AD_ACT, MODE_COMBI_YES},
    {LPM_PowerModeWait, AD_ACT, MODE_COMBI_YES},
    {LPM_PowerModeStop, AD_ACT, MODE_COMBI_YES},
    {LPM_PowerModeSleep, AD_ACT, MODE_COMBI_YES},
    {LPM_PowerModeDeepSleep, AD_ACT, MODE_COMBI_NO},
    {LPM_PowerModePowerDown, AD_ACT, MODE_COMBI_NO},
    {LPM_PowerModeDeepPowerDown, AD_ACT, MODE_COMBI_NO},

    {LPM_PowerModeActive, AD_DSL, MODE_COMBI_YES},
    {LPM_PowerModeWait, AD_DSL, MODE_COMBI_YES},
    {LPM_PowerModeStop, AD_DSL, MODE_COMBI_YES},
    {LPM_PowerModeSleep, AD_DSL, MODE_COMBI_YES},
    {LPM_PowerModeDeepSleep, AD_DSL, MODE_COMBI_YES},
    {LPM_PowerModePowerDown, AD_DSL, MODE_COMBI_NO},
    {LPM_PowerModeDeepPowerDown, AD_DSL, MODE_COMBI_NO},

    {LPM_PowerModeActive, AD_PD, MODE_COMBI_YES},
    {LPM_PowerModeWait, AD_PD, MODE_COMBI_YES},
    {LPM_PowerModeStop, AD_PD, MODE_COMBI_YES},
    {LPM_PowerModeSleep, AD_PD, MODE_COMBI_YES},
    {LPM_PowerModeDeepSleep, AD_PD, MODE_COMBI_YES},
    {LPM_PowerModePowerDown, AD_PD, MODE_COMBI_YES},
    {LPM_PowerModeDeepPowerDown, AD_PD, MODE_COMBI_NO},

    {LPM_PowerModeActive, AD_DPD, MODE_COMBI_YES},
    {LPM_PowerModeWait, AD_DPD, MODE_COMBI_YES},
    {LPM_PowerModeStop, AD_DPD, MODE_COMBI_YES},
    {LPM_PowerModeSleep, AD_DPD, MODE_COMBI_YES},
    {LPM_PowerModeDeepSleep, AD_DPD, MODE_COMBI_YES},
    {LPM_PowerModePowerDown, AD_DPD, MODE_COMBI_YES},
    {LPM_PowerModeDeepPowerDown, AD_DPD, MODE_COMBI_YES},
};
// clang-format on

/*******************************************************************************
 * Function Code
 ******************************************************************************/
static inline const char *APP_GetAllowCombiName(allow_combi_e allow)
{
    switch (allow)
    {
        GEN_CASE_ENUM_NAME(MODE_COMBI_NO);
        GEN_CASE_ENUM_NAME(MODE_COMBI_YES);
        default:
            return (char *)"WRONG_MODE_COMBI";
    }
}
static inline const char *APP_GetRtdPwrModeName(lpm_rtd_power_mode_e mode)
{
    switch (mode)
    {
        GEN_CASE_ENUM_NAME(LPM_PowerModeActive);
        GEN_CASE_ENUM_NAME(LPM_PowerModeWait);
        GEN_CASE_ENUM_NAME(LPM_PowerModeStop);
        GEN_CASE_ENUM_NAME(LPM_PowerModeSleep);
        GEN_CASE_ENUM_NAME(LPM_PowerModeDeepSleep);
        GEN_CASE_ENUM_NAME(LPM_PowerModePowerDown);
        GEN_CASE_ENUM_NAME(LPM_PowerModeDeepPowerDown);
        default:
            return (char *)"WRONG_LPM_RTD_PowerMode";
    }
}

static inline const char *APP_GetAdPwrModeName(lpm_ad_power_mode_e mode)
{
    switch (mode)
    {
        GEN_CASE_ENUM_NAME(AD_UNKOWN);
        GEN_CASE_ENUM_NAME(AD_ACT);
        GEN_CASE_ENUM_NAME(AD_DSL);
        GEN_CASE_ENUM_NAME(AD_PD);
        GEN_CASE_ENUM_NAME(AD_DPD);
        default:
            return (char *)"WRONG_LPM_AD_PowerMode";
    }
}

static allow_combi_e APP_GetModeAllowCombi(lpm_ad_power_mode_e ad_mode, lpm_rtd_power_mode_e rtd_mode)
{
    int i               = 0;
    allow_combi_e allow = MODE_COMBI_NO;
    ;

    if (BOARD_IsSingleBootType())
    {
        for (i = 0; i < ARRAY_SIZE(mode_combi_array_for_single_boot); i++)
        {
            if ((mode_combi_array_for_single_boot[i].rtd_mode == rtd_mode) &&
                (mode_combi_array_for_single_boot[i].ad_mode == ad_mode))
            {
                allow = mode_combi_array_for_single_boot[i].allow_combi;
                break;
            }
        }
    }
    else
    {
        for (i = 0; i < ARRAY_SIZE(mode_combi_array_for_dual_or_lp_boot); i++)
        {
            if ((mode_combi_array_for_dual_or_lp_boot[i].rtd_mode == rtd_mode) &&
                (mode_combi_array_for_dual_or_lp_boot[i].ad_mode == ad_mode))
            {
                allow = mode_combi_array_for_dual_or_lp_boot[i].allow_combi;
                break;
            }
        }
    }

    return allow;
}

static void APP_ShowModeCombi(void)
{
    int i = 0;

    PRINTF("###############################################\r\n");
    PRINTF("For Single Boot Type\r\n");
    for (i = 0; i < ARRAY_SIZE(mode_combi_array_for_single_boot); i++)
    {
        PRINTF("%s + %s: %s\r\n", APP_GetAdPwrModeName(mode_combi_array_for_single_boot[i].ad_mode),
               APP_GetRtdPwrModeName(mode_combi_array_for_single_boot[i].rtd_mode),
               APP_GetAllowCombiName(mode_combi_array_for_single_boot[i].allow_combi));
    }
    PRINTF("###############################################\r\n");
    PRINTF("\r\n");
    PRINTF("\r\n");

    PRINTF("###############################################\r\n");
    PRINTF("For Dual Boot Type/Low Power Boot Type\r\n");
    for (i = 0; i < ARRAY_SIZE(mode_combi_array_for_dual_or_lp_boot); i++)
    {
        PRINTF("%s + %s: %s\r\n", APP_GetAdPwrModeName(mode_combi_array_for_dual_or_lp_boot[i].ad_mode),
               APP_GetRtdPwrModeName(mode_combi_array_for_dual_or_lp_boot[i].rtd_mode),
               APP_GetAllowCombiName(mode_combi_array_for_dual_or_lp_boot[i].allow_combi));
    }
    PRINTF("###############################################\r\n");
}

static allow_give_sig_e APP_AllowGiveSig(lpm_rtd_power_mode_e current_mode, uint32_t irq)
{
    int i                  = 0;
    allow_give_sig_e allow = false;

    for (i = 0; i < ARRAY_SIZE(mode_combi_array_for_give_sig); i++)
    {
        if ((current_mode == mode_combi_array_for_give_sig[i].current_rtd_mode) &&
            (irq == mode_combi_array_for_give_sig[i].irq_num))
        {
            allow = mode_combi_array_for_give_sig[i].give_semaphore_flag;
            break;
        }
    }
    return allow;
}

/* WUU0 interrupt handler. */
void APP_WUU0_IRQHandler(void)
{
    bool wakeup = false;

    if (WUU_GetInternalWakeupModuleFlag(WUU0, WUU_MODULE_LPTMR1))
    {
        /* Woken up by LPTMR, then clear LPTMR flag. */
        LPTMR_ClearStatusFlags(LPTMR1, kLPTMR_TimerCompareFlag);
        LPTMR_DisableInterrupts(LPTMR1, kLPTMR_TimerInterruptEnable);
        LPTMR_StopTimer(LPTMR1);
        wakeup = true;
    }

    if (WUU_GetExternalWakeupPinFlag(WUU0, WUU_WAKEUP_PIN_IDX))
    {
        /* Woken up by external pin. */
        WUU_ClearExternalWakeupPinFlag(WUU0, WUU_WAKEUP_PIN_IDX);
        wakeup = true;
    }

    if (WUU_GetInternalWakeupModuleFlag(WUU0, WUU_MODULE_SYSTICK))
    {
        /* Woken up by Systick LPTMR, then clear LPTMR flag. */
        LPTMR_ClearStatusFlags(SYSTICK_BASE, kLPTMR_TimerCompareFlag);
    }

    if (wakeup)
    {
        xSemaphoreGiveFromISR(s_wakeupSig, NULL);
        portYIELD_FROM_ISR(pdTRUE);
    }
}

/* LPTMR1 interrupt handler. */
void LPTMR1_IRQHandler(void)
{
    bool wakeup = false;

    if (kLPTMR_TimerInterruptEnable & LPTMR_GetEnabledInterrupts(LPTMR1))
    {
        LPTMR_ClearStatusFlags(LPTMR1, kLPTMR_TimerCompareFlag);
        LPTMR_DisableInterrupts(LPTMR1, kLPTMR_TimerInterruptEnable);
        LPTMR_StopTimer(LPTMR1);
        wakeup = true;
    }

    current_state.give_semaphore_flag = APP_AllowGiveSig(s_curMode, LPTMR1_IRQn);
    if (!current_state.give_semaphore_flag)
    {
        /* skip give semaphore */
        return;
    }

    if (wakeup)
    {
        xSemaphoreGiveFromISR(s_wakeupSig, NULL);
        portYIELD_FROM_ISR(pdTRUE);
    }
}

/*
 * Wakeup RTD: lptimer or button(sw8).
 *
 *              |APD at PD: option(W), sw6(on/off), sw7, sw8.
 * Wakeup APD:  |
 *              |APD at DPD: option(W), sw6(on/off).
 *
 * RTD Wait/Stop/Sleep mode: sw8 will trigger two GPIO interrupt(kRGPIO_FlagEitherEdge) determined by APD IO service.
 * RTD DSL/PD/DPD mode: sw8 will trigger one WUU + one GPIO interrupt.
 */
static void APP_IRQDispatcher(IRQn_Type irq, void *param)
{
    /* ensure that only RTD/APD is woken up at a time button */
    current_state.last_rtd_mode       = current_state.current_rtd_mode;
    current_state.current_rtd_mode    = s_curMode;
    current_state.irq_num             = irq;
    current_state.give_semaphore_flag = RTD_GIVE_SIG_YES;

    if (current_state.current_rtd_mode == current_state.last_rtd_mode ||
        irq == BBNSM_IRQn) /* Always set the wake_acore_flag to true for the interrupt from BBNSM */
    {
        /*
         * RTD don't update low power state by using button wakeup APD.
         * so only when the last state is same with current state, button sw8 can wakeup APD.
         */
        wake_acore_flag = true;
    }
    else
    {
        wake_acore_flag = false;
    }

    current_state.give_semaphore_flag = APP_AllowGiveSig(s_curMode, irq);

    if (!current_state.give_semaphore_flag)
    {
        /* skip give semaphore */
        return;
    }
    switch (irq)
    {
        case WUU0_IRQn:
            APP_WUU0_IRQHandler();
            break;
        case GPIOB_INT0_IRQn:
            if ((1U << APP_PIN_IDX(APP_PIN_RTD_BTN2)) &
                RGPIO_GetPinsInterruptFlags(BOARD_SW8_GPIO, kRGPIO_InterruptOutput2))
            {
                /* Flag will be cleared by app_srtm.c */
                xSemaphoreGiveFromISR(s_wakeupSig, NULL);
                portYIELD_FROM_ISR(pdTRUE);
            }
            break;
        default:
            break;
    }
}

/* Get input from user about wakeup timeout. */
static uint32_t APP_GetWakeupTimeout(void)
{
    uint32_t timeout = 0U;
    uint8_t c;

    while (1)
    {
        PRINTF("Select the wake up timeout in seconds.\r\n");
        PRINTF("The allowed range is 1s ~ 999s.\r\n");
        PRINTF("Eg. enter 5 to wake up in 5 seconds.\r\n");
        PRINTF("\r\nWaiting for input timeout value...\r\n\r\n");

        do
        {
            c = GETCHAR();
            if ((c >= '0') && (c <= '9'))
            {
                PRINTF("%c", c);
                timeout = timeout * 10U + c - '0';
            }
            else if ((c == '\r') || (c == '\n'))
            {
                break;
            }
            else
            {
                PRINTF("%c\r\nWrong value!\r\n", c);
                timeout = 0U;
            }
        } while (timeout != 0U && timeout < 100U);

        if (timeout > 0U)
        {
            PRINTF("\r\n");
            break;
        }
    }

    return timeout;
}

/* Get wakeup source by user input. */
static app_wakeup_source_t APP_GetWakeupSource(void)
{
    uint8_t ch;

    while (1)
    {
        PRINTF("Select the wake up source:\r\n");
        PRINTF("Press T for LPTMR - Low Power Timer\r\n");
        PRINTF("Press S for switch/button %s. \r\n", APP_WAKEUP_BUTTON_NAME);

        PRINTF("\r\nWaiting for key press..\r\n\r\n");

        ch = GETCHAR();

        if ((ch >= 'a') && (ch <= 'z'))
        {
            ch -= 'a' - 'A';
        }

        if (ch == 'T')
        {
            return kAPP_WakeupSourceLptmr;
        }
        else if (ch == 'S')
        {
            return kAPP_WakeupSourcePin;
        }
        else
        {
            PRINTF("Wrong value!\r\n");
        }
    }
}

/* Get wakeup timeout and wakeup source. */
static void APP_GetWakeupConfig(void)
{
    /* Get wakeup source by user input. */
    s_wakeupSource = APP_GetWakeupSource();

    if (kAPP_WakeupSourceLptmr == s_wakeupSource)
    {
        /* Wakeup source is LPTMR, user should input wakeup timeout value. */
        s_wakeupTimeout = APP_GetWakeupTimeout();
        PRINTF("Will wakeup in %d seconds.\r\n", s_wakeupTimeout);
    }
    else
    {
        PRINTF("Press %s to wake up.\r\n", APP_WAKEUP_BUTTON_NAME);
    }
}

static void APP_SetWakeupConfig(lpm_rtd_power_mode_e targetMode)
{
    if (kAPP_WakeupSourceLptmr == s_wakeupSource)
    {
        LPTMR_SetTimerPeriod(LPTMR1, (1000UL * s_wakeupTimeout / 16U));
        LPTMR_StartTimer(LPTMR1);
        LPTMR_EnableInterrupts(LPTMR1, kLPTMR_TimerInterruptEnable);
    }

    /* To avoid conflicting access of WUU with SRTM dispatcher, we put the WUU setting into SRTM dispatcher context.*/
    /* If targetMode is PD/DPD, setup WUU. */
    if ((LPM_PowerModePowerDown == targetMode) || (LPM_PowerModeDeepPowerDown == targetMode))
    {
        if (kAPP_WakeupSourceLptmr == s_wakeupSource)
        {
            /* Set WUU LPTMR1 module wakeup source. */
            APP_SRTM_SetWakeupModule(WUU_MODULE_LPTMR1, LPTMR1_WUU_WAKEUP_EVENT);
            PCC1->PCC_LPTMR1 &= ~PCC1_PCC_LPTMR1_SSADO_MASK;
            PCC1->PCC_LPTMR1 |= PCC1_PCC_LPTMR1_SSADO(1);
        }
        else
        {
            /* Set PORT and WUU wakeup pin. */
            APP_SRTM_SetWakeupPin(APP_PIN_RTD_BTN2, (uint16_t)WUU_WAKEUP_PIN_TYPE | 0x100);
        }
    }
    else
    {
        /* Set PORT pin. */
        if (kAPP_WakeupSourcePin == s_wakeupSource)
        {
            uint16_t event = (uint16_t)WUU_WAKEUP_PIN_TYPE;
            /*
             * Need setup SSADO field when gate core, platform, bus clock(RTD clock mode), unless failed to wakeup
             * cortex-m33 by button. Currently will gate core, platform, bus clock when RTD enter Deep Sleep
             * Mode(LPM_SystemDeepSleep->RTDCMC_SetClockMode), so setup SSADO field here for Deep Sleep Mode.
             */
            if (LPM_PowerModeDeepSleep == targetMode)
            {
                PCC1->PCC_RGPIOB &= ~PCC1_PCC_RGPIOB_SSADO_MASK;
                PCC1->PCC_RGPIOB |= PCC1_PCC_RGPIOB_SSADO(1);
                event |= 0x100; /* enable wakeup flag */
            }
            APP_SRTM_SetWakeupPin(APP_PIN_RTD_BTN2, event);
        }
    }
}

static void APP_ClearWakeupConfig(lpm_rtd_power_mode_e targetMode)
{
    if (kAPP_WakeupSourcePin == s_wakeupSource)
    {
        APP_SRTM_SetWakeupPin(APP_PIN_RTD_BTN2, (uint16_t)kWUU_ExternalPinDisable);
    }
    else if ((LPM_PowerModePowerDown == targetMode) || (LPM_PowerModeDeepPowerDown == targetMode))
    {
        APP_SRTM_ClrWakeupModule(WUU_MODULE_LPTMR1, LPTMR1_WUU_WAKEUP_EVENT);
    }
}

/* Power Mode Switch task */
void PowerModeSwitchTask(void *pvParameters)
{
    lptmr_config_t lptmrConfig;
    lpm_rtd_power_mode_e targetPowerMode;
    uint32_t freq = 0U;
    uint8_t ch;

    /* As IRQ handler main entry locates in app_srtm.c to support services, here need an entry to handle application
     * IRQ events.
     */
    APP_SRTM_SetIRQHandler(APP_IRQDispatcher, NULL);
    /* Add Systick as Power Down wakeup source, depending on SYSTICK_WUU_WAKEUP_EVENT value. */
    APP_SRTM_SetWakeupModule(WUU_MODULE_SYSTICK, SYSTICK_WUU_WAKEUP_EVENT);

    /* Setup LPTMR. */
    LPTMR_GetDefaultConfig(&lptmrConfig);
    lptmrConfig.prescalerClockSource = kLPTMR_PrescalerClock_1;  /* Use RTC 1KHz as clock source. */
    lptmrConfig.bypassPrescaler      = false;
    lptmrConfig.value                = kLPTMR_Prescale_Glitch_3; /* Divide clock source by 16. */
    LPTMR_Init(LPTMR1, &lptmrConfig);
    NVIC_SetPriority(LPTMR1_IRQn, APP_LPTMR1_IRQ_PRIO);

    EnableIRQ(LPTMR1_IRQn);

    SIM_SEC->DGO_GP10  = 2;
    SIM_SEC->DGO_CTRL1 = SIM_SEC_DGO_CTRL1_UPDATE_DGO_GP10_MASK;
    /* Wait DGO GP0 updated */
    while ((SIM_SEC->DGO_CTRL1 & SIM_SEC_DGO_CTRL1_WR_ACK_DGO_GP10_MASK) == 0)
    {
    }
    /* Clear DGO GP0 ACK and UPDATE bits */
    SIM_SEC->DGO_CTRL1 =
        (SIM_SEC->DGO_CTRL1 & ~(SIM_SEC_DGO_CTRL1_UPDATE_DGO_GP10_MASK)) | SIM_SEC_DGO_CTRL1_WR_ACK_DGO_GP10_MASK;

    SIM_SEC->DGO_GP11  = 1; // PTB range to 1.8V
    SIM_SEC->DGO_CTRL1 = SIM_SEC_DGO_CTRL1_UPDATE_DGO_GP11_MASK;
    /* Wait DGO GP0 updated */
    while ((SIM_SEC->DGO_CTRL1 & SIM_SEC_DGO_CTRL1_WR_ACK_DGO_GP11_MASK) == 0)
    {
    }
    /* Clear DGO GP0 ACK and UPDATE bits */
    SIM_SEC->DGO_CTRL1 =
        (SIM_SEC->DGO_CTRL1 & ~(SIM_SEC_DGO_CTRL1_UPDATE_DGO_GP11_MASK)) | SIM_SEC_DGO_CTRL1_WR_ACK_DGO_GP11_MASK;

    SIM_RTD->PTC_COMPCELL = 0x0; // PTC compensation off

    for (;;)
    {
        freq = CLOCK_GetFreq(kCLOCK_Cm33CorePlatClk);
        PRINTF("\r\n####################  Power Mode Switch Task ####################\n\r\n");
        PRINTF("    Build Time: %s--%s \r\n", __DATE__, __TIME__);
        PRINTF("    Core Clock: %dHz \r\n", freq);
        PRINTF("    Boot Type: %s \r\n", BOARD_GetBootTypeName());
        PRINTF("\r\nSelect the desired operation \n\r\n");
        PRINTF("Press  %c to enter: Active mode\r\n", kAPP_PowerModeActive);
        PRINTF("Press  %c to enter: Cortex M33 Wait mode\r\n", kAPP_PowerModeWait);
        PRINTF("Press  %c to enter: Cortex M33 STOP mode\r\n", kAPP_PowerModeStop);
        PRINTF("Press  %c to enter: Sleep mode\r\n", kAPP_PowerModeSleep);
        PRINTF("Press  %c to enter: Deep Sleep mode\r\n", kAPP_PowerModeDeepSleep);
        PRINTF("Press  %c to enter: Power Down(PD) mode\r\n", kAPP_PowerModePowerDown);
        PRINTF("Press  %c to enter: Deep Power Down(DPD) mode\r\n", kAPP_PowerModeDeepPowerDown);
        PRINTF("Press  W for wake up CA35 core from PD/DPD mode\r\n");
        PRINTF("Press  T for reboot CA35 core\r\n");
        PRINTF("Press  U for shutdown CA35 core.\r\n");
        PRINTF("Press  V for boot CA35 core.\r\n");
        PRINTF("Press  S for showing supported LPM Mode Combination.\r\n");
        // clang-format off
        /*
         * OD: Over Drive Mode
         * ND: Norminal Drive Mode
         * Ud: Under Drive Mode
         *+-----------------------------------------------------------------------------------------------------+
         *|      Drive Mode of Cortex-M33   |           OD          |           ND         |          UD        |
         *+-----------------------------------------------------------------------------------------------------+
         *|         Voltage of BUCK2        |    1.05 V ~ 1.10 V    |     0.95 V ~ 1.00 V  |         0.9 V      |
         *+-----------------------------------------------------------------------------------------------------+
         *|         Biasing Option          |         AFBB          |         AFBB         |         ARBB       |
         *+-----------------------------------------------------------------------------------------------------+
         *|         Maximum frequency       |        216 MHz        |        160 MHz       |      38.4 MHz      |
         *+-----------------------------------------------------------------------------------------------------+
         */
        // clang-format on
        PRINTF("Press  M for switch Voltage Drive Mode between OD/ND/UD.\r\n");
        PRINTF(
            "Press  N for supporting Deep Sleep Mode(Pls set it when the option IMX8ULP_DSL_SUPPORT of TF-A is "
            "enabled) of Linux. support_dsl_for_apd = %d\r\n",
            APP_SRTM_GetSupportDSLForApd());
        PRINTF("\r\nWaiting for power mode select..\r\n\r\n");

        /* Wait for user response */
        do
        {
            ch = GETCHAR();
        } while ((ch == '\r') || (ch == '\n'));

        if ((ch >= 'a') && (ch <= 'z'))
        {
            ch -= 'a' - 'A';
        }
        targetPowerMode = (lpm_rtd_power_mode_e)(ch - 'A');
        if (targetPowerMode <= LPM_PowerModeDeepPowerDown)
        {
            if (targetPowerMode == s_curMode)
            {
                /* Same mode, skip it */
                continue;
            }
            if (APP_GetModeAllowCombi(AD_CurrentMode, targetPowerMode) == MODE_COMBI_NO)
            {
                PRINTF("Not support the mode combination: %s + %s\r\n", APP_GetAdPwrModeName(AD_CurrentMode),
                       APP_GetRtdPwrModeName(targetPowerMode));
                continue;
            }
            if (!LPM_SetPowerMode(targetPowerMode))
            {
                PRINTF("Some task doesn't allow to enter mode %s\r\n", s_modeNames[targetPowerMode]);
            }
            else /* Idle task will handle the low power state. */
            {
                APP_GetWakeupConfig();
                APP_SetWakeupConfig(targetPowerMode);
                xSemaphoreTake(s_wakeupSig, portMAX_DELAY);
                /* The call might be blocked by SRTM dispatcher task. Must be called after power mode reset. */
                APP_ClearWakeupConfig(targetPowerMode);
            }
        }
        else if ('W' == ch)
        {
            if (!wake_acore_flag)
            {
                wake_acore_flag = true;
            }
            APP_SRTM_WakeupCA35();
        }
        else if ('T' == ch)
        {
            APP_RebootCA35();
        }
        else if ('U' == ch)
        {
            APP_ShutdownCA35();
        }
        else if ('V' == ch)
        {
            option_v_boot_flag = true;
            APP_BootCA35();
        }
        else if ('S' == ch)
        {
            APP_ShowModeCombi();
        }
        else if ('M' == ch)
        {
            BOARD_SwitchDriveMode();
        }
        else if ('N' == ch)
        {
            PRINTF("Warning: Pls ensure that the option IMX8ULP_DSL_SUPPORT is enabled in TF-A\r\n");
            APP_SRTM_SetSupportDSLForApd(true);
        }
        else
        {
            PRINTF("Invalid command %c[0x%x]\r\n", ch, ch);
        }
        /*update Mode state*/
        s_lastMode = s_curMode;
        s_curMode  = LPM_PowerModeActive;
        PRINTF("\r\nNext loop\r\n");
    }
}

void vApplicationMallocFailedHook(void)
{
    PRINTF("Malloc Failed!!!\r\n");
}

void vPortSuppressTicksAndSleep(TickType_t xExpectedIdleTime)
{
    uint32_t irqMask;
    lpm_rtd_power_mode_e targetPowerMode;
    /* lpm_rtd_power_mode_e targetMode; */
    upwr_pwm_param_t param;
    bool result;

    /* targetMode = LPM_GetPowerMode(); */

    /* Workround for PD/DPD exit fail if sleep more than 1 second */
    /* if ((LPM_PowerModePowerDown == targetMode) || (LPM_PowerModeDeepPowerDown == targetMode)) */
    {
        param.R              = 0;
        param.B.DPD_ALLOW    = 0;
        param.B.DSL_DIS      = 0;
        param.B.SLP_ALLOW    = 0;
        param.B.DSL_BGAP_OFF = 1;
        param.B.DPD_BGAP_ON  = 0;

        UPOWER_SetPwrMgmtParam(&param);
    }

    irqMask = DisableGlobalIRQ();

    /* Only when no context switch is pending and no task is waiting for the scheduler
     * to be unsuspended then enter low power entry.
     */
    if (eTaskConfirmSleepModeStatus() != eAbortSleep)
    {
        targetPowerMode = LPM_GetPowerMode();
        if (targetPowerMode != LPM_PowerModeActive)
        {
            /* Only wait when target power mode is not running */
            APP_PowerPreSwitchHook(targetPowerMode);
            result = LPM_WaitForInterrupt((uint64_t)1000 * xExpectedIdleTime / configTICK_RATE_HZ);
            APP_PowerPostSwitchHook(targetPowerMode, result);
        }
    }
    EnableGlobalIRQ(irqMask);
    /* Recovery clock(switch clock source from FRO to PLL0/1) after interrupt is enabled */
    BOARD_ResumeClockInit();
}

/* Called in PowerModeSwitchTask */
static bool APP_LpmListener(lpm_rtd_power_mode_e curMode, lpm_rtd_power_mode_e newMode, void *data)
{
    PRINTF("WorkingTask %d: Transfer from %s to %s\r\n", (uint32_t)data, s_modeNames[curMode], s_modeNames[newMode]);

    /* Do necessary preparation for this mode change */

    return true; /* allow this switch */
}

/*!
 * @brief simulating working task.
 */
static void WorkingTask(void *pvParameters)
{
    LPM_RegisterPowerListener(APP_LpmListener, pvParameters);

    for (;;)
    {
        /* Use App task logic to replace vTaskDelay */
        PRINTF("Task %d is working now\r\n", (uint32_t)pvParameters);
        vTaskDelay(portMAX_DELAY);
    }
}

/*! @brief Main function */
int main(void)
{
    BOARD_InitHardware();

    LPM_Init();

    s_wakeupSig = xSemaphoreCreateBinary();

    xTaskCreate(PowerModeSwitchTask, "Main Task", 512U, NULL, tskIDLE_PRIORITY + 1U, NULL);
    xTaskCreate(WorkingTask, "Working Task", configMINIMAL_STACK_SIZE, (void *)1, tskIDLE_PRIORITY + 2U, NULL);

    /* Start FreeRTOS scheduler. */
    vTaskStartScheduler();

    /* Application should never reach this point. */
    for (;;)
    {
    }
}
