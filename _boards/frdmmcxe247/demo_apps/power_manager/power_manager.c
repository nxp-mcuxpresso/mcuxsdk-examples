/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
#include "fsl_smc.h"
#include "fsl_rcm.h"
#include "fsl_lptmr.h"
#include "fsl_port.h"
#include "fsl_debug_console.h"
#include "power_manager.h"
#include "fsl_notifier.h"
#include "board.h"
#include "app.h"
#include "peripherals.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*
 * Set the clock configuration for HSRUN mode.
 */
extern void APP_SetClockHsrun(void);

/*
 * Set the clock configuration for RUN mode from HSRUN mode.
 */
extern void APP_SetClockRunFromHsrun(void);

/*
 * Set the clock configuration for RUN mode from VLPR mode.
 */
extern void APP_SetClockRunFromVlpr(void);

/*
 * Set the clock configuration for VLPR mode.
 */
extern void APP_SetClockVlpr(void);

/*
 * Power mode switch callback.
 */
status_t callback0(notifier_notification_block_t *notify, void *dataPtr);

/*******************************************************************************
 * Variables
 ******************************************************************************/
static uint8_t s_wakeupTimeout;            /* Wakeup timeout. (Unit: Second) */
static app_wakeup_source_t s_wakeupSource; /* Wakeup source.                 */

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief LPTMR0 interrupt handler.
 */
void APP_LPTMR_IRQHANDLER(void)
{
    if (kLPTMR_TimerInterruptEnable & LPTMR_GetEnabledInterrupts(APP_LPTMR))
    {
        LPTMR_DisableInterrupts(APP_LPTMR, kLPTMR_TimerInterruptEnable);
        LPTMR_ClearStatusFlags(APP_LPTMR, kLPTMR_TimerCompareFlag);
        LPTMR_StopTimer(APP_LPTMR);
    }
    __DSB();
}

/*!
 * @brief button interrupt handler.
 */
void APP_WAKEUP_BUTTON_IRQ_HANDLER(void)
{
    if ((1U << APP_WAKEUP_BUTTON_GPIO_PIN) & PORT_GetPinsInterruptFlags(APP_WAKEUP_BUTTON_PORT))
    {
        /* Disable interrupt. */
        PORT_SetPinInterruptConfig(APP_WAKEUP_BUTTON_PORT, APP_WAKEUP_BUTTON_GPIO_PIN, kPORT_InterruptOrDMADisabled);
        PORT_ClearPinsInterruptFlags(APP_WAKEUP_BUTTON_PORT, (1U << APP_WAKEUP_BUTTON_GPIO_PIN));
    }
    /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
    exception return operation might vector to incorrect interrupt */
    __DSB();
}

/*!
 * @brief Get input from user to obtain wakeup timeout
 */
static uint8_t APP_GetWakeupTimeout(void)
{
    uint8_t timeout;

    while (1)
    {
        PRINTF("Select the wake up timeout in seconds.\r\n");
        PRINTF("The allowed range is 1s ~ 9s.\r\n");
        PRINTF("Eg. enter 5 to wake up in 5 seconds.\r\n");
        PRINTF("\r\nWaiting for input timeout value...\r\n\r\n");

        timeout = GETCHAR();
        PRINTF("%c\r\n", timeout);
        if ((timeout > '0') && (timeout <= '9'))
        {
            return timeout - '0';
        }
        PRINTF("Wrong value!\r\n");
    }
}

/*!
 * @brief Get wakeup source by user input.
 */
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

/*! @brief Get wakeup timeout and wakeup source. */
void APP_GetWakeupConfig(app_power_mode_t targetMode)
{
    /* Get wakeup source by user input. */
    {
        /* Get wakeup source by user input. */
        s_wakeupSource = APP_GetWakeupSource();
    }

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

/*! @brief Set wakeup timeout and wakeup source. */
void APP_SetWakeupConfig(app_power_mode_t targetMode)
{
    /* Set LPTMR timeout value. */
    if (kAPP_WakeupSourceLptmr == s_wakeupSource)
    {
        LPTMR_SetTimerPeriod(APP_LPTMR, (1000U * s_wakeupTimeout) - 1U);
        LPTMR_StartTimer(APP_LPTMR);
    }

    /* Set the wakeup module. */
    if (kAPP_WakeupSourceLptmr == s_wakeupSource)
    {
        LPTMR_EnableInterrupts(APP_LPTMR, kLPTMR_TimerInterruptEnable);
    }
    else
    {
        PORT_SetPinInterruptConfig(APP_WAKEUP_BUTTON_PORT, APP_WAKEUP_BUTTON_GPIO_PIN, APP_WAKEUP_BUTTON_IRQ_TYPE);
    }

}

/*! @brief Show current power mode. */
void APP_ShowPowerMode(smc_power_state_t currentPowerState)
{
    switch (currentPowerState)
    {
        case kSMC_PowerStateRun:
            PRINTF("    Power mode: RUN\r\n");
            break;
        case kSMC_PowerStateVlpr:
            PRINTF("    Power mode: VLPR\r\n");
            break;
        case kSMC_PowerStateHsrun:
            PRINTF("    Power mode: HSRUN\r\n");
            break;
        default:
            PRINTF("    Power mode wrong\r\n");
            break;
    }
}

/*!
 * @brief check whether could switch to target power mode from current mode.
 * Return true if could switch, return false if could not switch.
 */
bool APP_CheckPowerMode(smc_power_state_t currentPowerState, app_power_mode_t targetPowerMode)
{
    bool modeValid = true;

    /*
     * Check wether the mode change is allowed.
     *
     * 1. If current mode is HSRUN mode, the target mode must be RUN mode.
     * 3. If current mode is VLPR mode, the target mode must not be HSRUN/STOP mode.
     * 4. If already in the target mode, don't need to change.
     */
    switch (currentPowerState)
    {
        case kSMC_PowerStateRun:
            break;
        case kSMC_PowerStateHsrun:
            if (kAPP_PowerModeRun != targetPowerMode)
            {
                PRINTF("Current mode is HSRUN, please choose RUN mode as target mode.\r\n");
                modeValid = false;
            }
            break;

        case kSMC_PowerStateVlpr:
            if ((kAPP_PowerModeHsrun == targetPowerMode) || (kAPP_PowerModeStop == targetPowerMode))
            {
                PRINTF("Could not enter HSRUN/STOP modes from VLPR mode.\r\n");
                modeValid = false;
            }
            break;
        default:
            PRINTF("Wrong power state.\r\n");
            modeValid = false;
            break;
    }

    if (!modeValid)
    {
        return false;
    }

    /* Don't need to change power mode if current mode is already the target mode. */
    if (((kAPP_PowerModeRun == targetPowerMode) && (kSMC_PowerStateRun == currentPowerState)) ||
        ((kAPP_PowerModeHsrun == targetPowerMode) && (kSMC_PowerStateHsrun == currentPowerState)) ||
        ((kAPP_PowerModeVlpr == targetPowerMode) && (kSMC_PowerStateVlpr == currentPowerState)))
    {
        PRINTF("Already in the target power mode.\r\n");
        return false;
    }

    return true;
}

/*!
 * @brief Power mode switch.
 * This function is used to register the notifier_handle_t struct's member userFunction.
 */
status_t APP_PowerModeSwitch(notifier_user_config_t *targetConfig, void *userData)
{
    smc_power_state_t currentPowerMode;         /* Local variable with current power mode */
    app_power_mode_t targetPowerMode;           /* Local variable with target power mode name*/
    power_user_config_t *targetPowerModeConfig; /* Local variable with target power mode configuration */

    targetPowerModeConfig = (power_user_config_t *)targetConfig;
    currentPowerMode      = SMC_GetPowerModeState(SMC);
    targetPowerMode       = targetPowerModeConfig->mode;

    switch (targetPowerMode)
    {
        case kAPP_PowerModeVlpr:
            APP_SetClockVlpr();
            SMC_SetPowerModeVlpr(SMC);
            while (kSMC_PowerStateVlpr != SMC_GetPowerModeState(SMC))
            {
            }
            break;

        case kAPP_PowerModeRun:
            /* If enter RUN from HSRUN, fisrt change clock. */
            if (kSMC_PowerStateHsrun == currentPowerMode)
            {
                APP_SetClockRunFromHsrun();
            }

            /* Power mode change. */
            SMC_SetPowerModeRun(SMC);
            while (kSMC_PowerStateRun != SMC_GetPowerModeState(SMC))
            {
            }

            /* If enter RUN from VLPR, change clock after the power mode change. */
            if (kSMC_PowerStateVlpr == currentPowerMode)
            {
                APP_SetClockRunFromVlpr();
            }
            break;

        case kAPP_PowerModeHsrun:
            SMC_SetPowerModeHsrun(SMC);
            while (kSMC_PowerStateHsrun != SMC_GetPowerModeState(SMC))
            {
            }

            APP_SetClockHsrun(); /* Change clock setting after power mode change. */
            break;

        /* For stop modes. */
        case kAPP_PowerModeStop:
            SMC_SetPowerModeStop(SMC, kSMC_PartialStop);
            break;

        case kAPP_PowerModeVlps:
            SMC_SetPowerModeVlps(SMC);
            break;

        default:
            PRINTF("Wrong value");
            break;
    }
    return kStatus_Success;
}

/*!
 * @brief main demo function.
 */
int main(void)
{
    uint32_t freq = 0;
    uint8_t ch;
    uint8_t targetConfigIndex;
    notifier_handle_t powerModeHandle;
    smc_power_state_t currentPowerState;
    app_power_mode_t targetPowerMode;
    bool needSetWakeup; /* Flag of whether or not need to set wakeup. */

    /*Power mode configurations*/
    power_user_config_t vlprConfig = {
        kAPP_PowerModeVlpr,
    };

    power_user_config_t vlpsConfig = vlprConfig;
    power_user_config_t stopConfig = vlprConfig;
    power_user_config_t runConfig  = vlprConfig;
    power_user_config_t hsrunConfig = vlprConfig;

    /* Initializes array of pointers to power mode configurations */
    notifier_user_config_t *powerConfigs[] = {
        &runConfig, &stopConfig, &vlprConfig, &vlpsConfig, &hsrunConfig,
    };

    /* User callback data0 */
    user_callback_data_t callbackData0;

    /* Initializes callback configuration structure */
    notifier_callback_config_t callbackCfg0 = {callback0, kNOTIFIER_CallbackBeforeAfter, (void *)&callbackData0};

    /* Initializes array of callback configurations */
    notifier_callback_config_t callbacks[] = {callbackCfg0};

    memset(&callbackData0, 0, sizeof(user_callback_data_t));

    /* Initializes configuration structures */
    vlpsConfig.mode = kAPP_PowerModeVlps;
    stopConfig.mode = kAPP_PowerModeStop;
    runConfig.mode  = kAPP_PowerModeRun;
    hsrunConfig.mode = kAPP_PowerModeHsrun;

    /* Create Notifier Handle */
    NOTIFIER_CreateHandle(&powerModeHandle, powerConfigs, ARRAY_SIZE(powerConfigs), callbacks, 1U, APP_PowerModeSwitch,
                          NULL);

    BOARD_InitHardware();

    EnableIRQ(APP_WAKEUP_BUTTON_IRQ);

    while (1)
    {
        currentPowerState = SMC_GetPowerModeState(SMC);

        freq = CLOCK_GetFreq(kCLOCK_CoreSysClk);

        PRINTF("\r\n####################  Power Manager Demo ####################\n\r\n");
        PRINTF("    Core Clock = %dHz \r\n", freq);

        APP_ShowPowerMode(currentPowerState);

        PRINTF("\r\nSelect the desired operation \n\r\n");
        PRINTF("Press  %c for enter: RUN      - Normal RUN mode\r\n", kAPP_PowerModeRun);
        PRINTF("Press  %c for enter: STOP     - Stop mode\r\n", kAPP_PowerModeStop);
        PRINTF("Press  %c for enter: VLPR     - Very Low Power Run mode\r\n", kAPP_PowerModeVlpr);
        PRINTF("Press  %c for enter: VLPS     - Very Low Power Stop mode\r\n", kAPP_PowerModeVlps);
        PRINTF("Press  %c for enter: HSRUN    - High Speed RUN mode\r\n", kAPP_PowerModeHsrun);

        PRINTF("\r\nWaiting for power mode select..\r\n\r\n");

        /* Wait for user response */
        ch = GETCHAR();

        if ((ch >= 'a') && (ch <= 'z'))
        {
            ch -= 'a' - 'A';
        }

        targetPowerMode = (app_power_mode_t)ch;

        if ((targetPowerMode > kAPP_PowerModeMin) && (targetPowerMode < kAPP_PowerModeMax))
        {
            /* If could not set the target power mode, loop continue. */
            if (!APP_CheckPowerMode(currentPowerState, targetPowerMode))
            {
                continue;
            }

#if defined(FSL_FEATURE_SMC_HAS_ERRATA_011063) && (FSL_FEATURE_SMC_HAS_ERRATA_011063)
            /* Check if transition is from RUN/VLPR to VLPS */
            /* From VLPR to VLPS, the only clock source is SIRC 8MHz and maximum SYS_CLOCK is 4 MHz, so PREDIV_SYS_CLK/BUS_CLK must be >= 2 */
            if ((targetPowerMode == kAPP_PowerModeVlps) && (currentPowerState == kSMC_PowerStateRun))
            {
                uint32_t rccr         = SCG->RCCR;
                uint32_t divcore      = ((rccr & SCG_RCCR_DIVCORE_MASK) >> SCG_RCCR_DIVCORE_SHIFT) + 1;
                uint32_t divbus       = ((rccr & SCG_RCCR_DIVBUS_MASK) >> SCG_RCCR_DIVBUS_SHIFT) + 1;
                uint32_t currentRatio = divcore * divbus;
                /* Check if PREDIV_SYS_CLK/BUS_CLK ratio >= 2 */
                if (currentRatio < 2)
                {
                    PRINTF(
                        "Because of ERR011063, PREDIV_SYS_CLK/BUS_CLK ratio must be >= 2 for RUN to VLPS or VLPR to "
                        "VLPS transition, otherwise the MCU may hang in an undetermined state, which can only be "
                        "recovered by a power-on reset event or a watchdog reset.\r\nThe current ratio is %d, please "
                        "adjust configuration value for SCG_RCCR[DIVCORE] and SCG_RCCR[DIVBUS]. Application will skip "
                        "this transition.",
                        currentRatio);
                    continue;
                }
            }
#endif

            /* If target mode is RUN/VLPR/HSRUN, don't need to set wakeup source. */
            if ((kAPP_PowerModeRun == targetPowerMode) || (kAPP_PowerModeHsrun == targetPowerMode) ||
                (kAPP_PowerModeVlpr == targetPowerMode))
            {
                needSetWakeup = false;
            }
            else
            {
                needSetWakeup = true;
            }

            if (needSetWakeup)
            {
                APP_GetWakeupConfig(targetPowerMode);
                APP_SetWakeupConfig(targetPowerMode);
            }

            callbackData0.originPowerState = currentPowerState;
            targetConfigIndex              = targetPowerMode - kAPP_PowerModeMin - 1;
            NOTIFIER_SwitchConfig(&powerModeHandle, targetConfigIndex, kNOTIFIER_PolicyAgreement);
            if (kAPP_PowerModeVlps == targetPowerMode)
            {
                PRINTF("\r\nEntered VLPS mode and woke up.\r\n");
            }
            PRINTF("\r\nNext loop\r\n");
        }
    }
}
