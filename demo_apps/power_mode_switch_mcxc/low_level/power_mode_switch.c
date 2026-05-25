/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "app.h"
#include "board.h"
#include "fsl_cmc.h"
#include "fsl_spc.h"
#include "fsl_wuu.h"
#include "fsl_gpio.h"
#include "fsl_clock.h"
#include "fsl_lptmr.h"
#include "fsl_lpuart.h"
#include "fsl_debug_console.h"
#include "power_mode_switch.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
__WEAK void APP_PowerPreSwitchHook(app_power_mode_t targetPowerMode);
__WEAK void APP_PowerPostSwitchHook(void);

static app_power_mode_t APP_GetTargetPowerMode(void);
static void APP_GetWakeupConfig(app_power_mode_t targetMode);
static void APP_PowerModeSwitch(app_power_mode_t targetPowerMode);
static void APP_ClearAllWakeupConfig(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
char *const g_modeNameArray[] = APP_POWER_MODE_NAME;
char *const g_modeDescArray[] = APP_POWER_MODE_DESC;

/*******************************************************************************
 * Code
 ******************************************************************************/
int main(void)
{
    uint32_t freq;
    app_power_mode_t targetPowerMode;
    bool needSetWakeup = false;
    bool wakeUpReset;

    wakeUpReset = ((CMC_GetStickySystemResetStatus(APP_CMC) & (uint32_t)CMC_SRS_WAKEUP_MASK) != 0UL);
    if (wakeUpReset)
    {
        /* Wakeup from Deep Power Down mode? => Clears peripherals and I/O pads isolation flags. */
        SPC_ClearPeriphIOIsolationFlag(APP_SPC);
    }

    BOARD_InitHardware();

    if (wakeUpReset)
    {
        APP_ClearAllWakeupConfig();
        CMC_ClearStickySystemResetStatus(APP_CMC, (uint32_t)CMC_SRS_WAKEUP_MASK);
    }

    EnableIRQ(NonMaskableInt_IRQn);

    PRINTF("\r\nNormal Boot.\r\n");

    while (1)
    {
        if (kCMC_CoreClockGated == CMC_GetCoreClockGatedStatus(APP_CMC))
        {
              CMC_ClearCoreClockGatedStatus(APP_CMC);
        }

        freq = CLOCK_GetFreq(kCLOCK_CoreSysClk);

        PRINTF("\r\n###########################    Power Mode Switch Demo    ###########################\r\n");
        PRINTF("    Core Clock = %dHz \r\n", freq);
        PRINTF("    Power mode: Active\r\n");
        targetPowerMode = APP_GetTargetPowerMode();

        if ((targetPowerMode > kAPP_PowerModeMin) && (targetPowerMode < kAPP_PowerModeMax))
        {
            /* If target mode is Active mode, don't need to set wakeup source. */
            if (targetPowerMode == kAPP_PowerModeActive)
            {
                needSetWakeup = false;
            }
            else
            {
                needSetWakeup = true;
            }
        }

        /* Print description of selected power mode. */
        PRINTF("\r\n");

        if (needSetWakeup)
        {
            APP_GetWakeupConfig(targetPowerMode);
            APP_PowerPreSwitchHook(targetPowerMode);
            APP_PowerModeSwitch(targetPowerMode);
            APP_PowerPostSwitchHook();
        }

        PRINTF("\r\nNext loop.\r\n");
    }
}

/************************** Wakeup Configuration *******************************/
/*! @brief Select wakeup source.  */
static app_wakeup_source_t APP_SelectWakeupSource(void)
{
    char ch;

    PRINTF("Please select wakeup source:\r\n");
    PRINTF("\tPress %c to select TIMER as wakeup source;\r\n", kAPP_WakeupSourceTimer);
    PRINTF("\tPress %c to select WAKE-UP-BUTTON as wakeup source;\r\n", kAPP_WakeupSourceButton);

    PRINTF("Waiting for wakeup source select...\r\n");
    ch = GETCHAR();

    if ((ch >= 'a') && (ch <= 'z'))
    {
        ch -= 'a' - 'A';
    }

    return (app_wakeup_source_t)ch;
}

/*! @brief  Get input from user about wakeup timeout */
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

/*! @brief WakeUp Timer configuration. */
static void APP_WakeUpTimerConfig(uint8_t timeOutValue)
{
    lptmr_config_t lptmr_config;
    LPTMR_GetDefaultConfig(&lptmr_config);
    lptmr_config.prescalerClockSource = kLPTMR_PrescalerClock_1;
    LPTMR_Init(LPTMR0, &lptmr_config);

    LPTMR_ClearStatusFlags(APP_WUU_WAKEUP_TIMER, kLPTMR_TimerCompareFlag);
    IRQ_ClearPendingIRQ(APP_WUU_WAKEUP_TIMER_IRQN);
    LPTMR_SetTimerPeriod(APP_WUU_WAKEUP_TIMER, (APP_WUU_WAKEUP_TIMER_CLOCK_SOURCE * timeOutValue) - 1U);
    LPTMR_EnableInterrupts(APP_WUU_WAKEUP_TIMER, kLPTMR_TimerInterruptEnable);
    EnableIRQ(APP_WUU_WAKEUP_TIMER_IRQN);

    LPTMR_StartTimer(APP_WUU_WAKEUP_TIMER);
}

/*! @brief Set Non-Maskable Interrupt source. */
static inline void SystemNonMaskableInterruptSourceSet(IRQn_Type id)
{
    SYSCON->NMISRC = ((SYSCON->NMISRC & ~(SYSCON_NMISRC_IRQCPU0_MASK | SYSCON_NMISRC_NMIENCPU0_MASK)) |
                      (SYSCON_NMISRC_IRQCPU0((uint32_t)id) | SYSCON_NMISRC_NMIENCPU0_MASK));
    CMC_EnableNonMaskablePinInterrupt(APP_CMC, true);
}

/*! @brief Get wakeup timeout and wakeup source. */
static void APP_GetWakeupConfig(app_power_mode_t targetMode)
{
    app_wakeup_source_t wakeupSource;
    uint8_t timeOutValue;
    char *isoDomains = NULL;
    wakeupSource = APP_SelectWakeupSource();

    switch (wakeupSource)
    {
        case kAPP_WakeupSourceTimer:
        {
            PRINTF("Timer Selected As Wakeup Source!\r\n");

            timeOutValue = APP_GetWakeupTimeout();
            PRINTF("Will wakeup in %d seconds.\r\n", timeOutValue);

            WUU_SetInternalWakeUpModulesConfig(APP_WUU, APP_WUU_WAKEUP_TIMER_IDX,
                                               kWUU_InternalModuleInterrupt);
            APP_WakeUpTimerConfig(timeOutValue);

            SystemNonMaskableInterruptSourceSet(APP_WUU_WAKEUP_TIMER_IRQN);

            if (targetMode > kAPP_PowerModeSleep)
            {
                SPC_SetExternalVoltageDomainsConfig(APP_SPC, APP_SPC_WAKEUP_TIMER_LPISO_VALUE,
                                                    APP_SPC_WAKEUP_TIMER_ISO_VALUE);
                isoDomains = APP_SPC_WAKEUP_TIMER_ISO_DOMAINS;
                PRINTF("Isolate power domains: %s\r\n", isoDomains);
            }
            break;
        }

        case kAPP_WakeupSourceButton:
        {
            PRINTF("Wakeup Button Selected As Wakeup Source.\r\n");

            wuu_external_wakeup_pin_config_t wakeupButtonConfig;
            wakeupButtonConfig.edge  = kWUU_ExternalPinFallingEdge;
            wakeupButtonConfig.event = kWUU_ExternalPinInterrupt;
            wakeupButtonConfig.mode  = kWUU_ExternalPinActiveAlways;
            WUU_ClearExternalWakeUpPinsFlag(APP_WUU, (1UL << (uint32_t)APP_WUU_WAKEUP_BUTTON_IDX));
            IRQ_ClearPendingIRQ(APP_WUU_IRQN);
            WUU_SetExternalWakeUpPinsConfig(APP_WUU, APP_WUU_WAKEUP_BUTTON_IDX, &wakeupButtonConfig);
            EnableIRQ(APP_WUU_IRQN);

            SystemNonMaskableInterruptSourceSet(APP_WUU_IRQN);

            PRINTF("Please press %s to wakeup.\r\n", APP_WUU_WAKEUP_BUTTON_NAME);

            if (targetMode > kAPP_PowerModeSleep)
            {
                SPC_SetExternalVoltageDomainsConfig(APP_SPC, APP_SPC_WAKEUP_BUTTON_LPISO_VALUE,
                                                    APP_SPC_WAKEUP_BUTTON_ISO_VALUE);
                isoDomains = APP_SPC_WAKEUP_BUTTON_ISO_DOMAINS;
                PRINTF("Isolate power domains: %s\r\n", isoDomains);
            }
            break;
        }

        default:
            assert(false);
            break;
    }
}

/*! @brief Clear wakeup timer configuration. */
static void APP_ClearWakeupTimerConfig(void)
{
    DisableIRQ(APP_WUU_WAKEUP_TIMER_IRQN);

    LPTMR_DisableInterrupts(APP_WUU_WAKEUP_TIMER, kLPTMR_TimerInterruptEnable);
    LPTMR_ClearStatusFlags(APP_WUU_WAKEUP_TIMER, kLPTMR_TimerCompareFlag);
    LPTMR_StopTimer(APP_WUU_WAKEUP_TIMER);

    WUU_ClearInternalWakeUpModulesConfig(APP_WUU, APP_WUU_WAKEUP_TIMER_IDX, kWUU_InternalModuleInterrupt);
    IRQ_ClearPendingIRQ(APP_WUU_WAKEUP_TIMER_IRQN);
}

/*! @brief Clear wakeup button configuration. */
static void APP_ClearWakeupButtonConfig(void)
{
    const uint32_t wakeupButtonMask = (1UL << (uint32_t)APP_WUU_WAKEUP_BUTTON_IDX);

    DisableIRQ(APP_WUU_IRQN);

    WUU_ClearExternalWakeupPinsConfig(APP_WUU, APP_WUU_WAKEUP_BUTTON_IDX);
    WUU_ClearExternalWakeUpPinsFlag(APP_WUU, wakeupButtonMask);
    IRQ_ClearPendingIRQ(APP_WUU_IRQN);
}

/*! @brief Clear Non-Maskable Interrupt source. */
static inline void SystemNonMaskableInterruptSourceClear(void)
{
    SYSCON->NMISRC &= ~(SYSCON_NMISRC_IRQCPU0_MASK | SYSCON_NMISRC_NMIENCPU0_MASK);
    CMC_EnableNonMaskablePinInterrupt(APP_CMC, false);
}

/*! @brief Clear all wakeup configurations. */
static void APP_ClearAllWakeupConfig(void)
{
    APP_ClearWakeupTimerConfig();
    APP_ClearWakeupButtonConfig();
    SystemNonMaskableInterruptSourceClear();
}

/*! @brief WakeUp Timer interrupt handler. */
void APP_WUU_WAKEUP_TIMER_IRQ_HANDLER(void)
{
    if ((LPTMR_GetStatusFlags(APP_WUU_WAKEUP_TIMER) & (uint32_t)kLPTMR_TimerCompareFlag) != 0UL)
    {
        LPTMR_DisableInterrupts(APP_WUU_WAKEUP_TIMER, kLPTMR_TimerInterruptEnable);
        LPTMR_ClearStatusFlags(APP_WUU_WAKEUP_TIMER, kLPTMR_TimerCompareFlag);
        LPTMR_StopTimer(APP_WUU_WAKEUP_TIMER);
    }
}

/*! @brief WakeUp Button interrupt handler. */
void APP_WUU_IRQ_HANDLER(void)
{
    const uint32_t wakeupButtonMask = (1UL << (uint32_t)APP_WUU_WAKEUP_BUTTON_IDX);

    DisableIRQ(APP_WUU_IRQN);
    if ((WUU_GetExternalWakeUpPinsFlag(APP_WUU) & wakeupButtonMask) != 0UL)
    {
        WUU_ClearExternalWakeUpPinsFlag(APP_WUU, wakeupButtonMask);
    }
}

/*! @brief Non-Maskable Interrupt handler. */
void NMI_Handler(void)
{
    IRQn_Type nmiSource = (IRQn_Type)((SYSCON->NMISRC & SYSCON_NMISRC_IRQCPU0_MASK) >> SYSCON_NMISRC_IRQCPU0_SHIFT);

    switch (nmiSource)
    {
        case APP_WUU_WAKEUP_TIMER_IRQN:
            APP_WUU_WAKEUP_TIMER_IRQ_HANDLER();
            WUU_ClearInternalWakeUpModulesConfig(APP_WUU, APP_WUU_WAKEUP_TIMER_IDX, kWUU_InternalModuleInterrupt);
            IRQ_ClearPendingIRQ(APP_WUU_WAKEUP_TIMER_IRQN);
            break;

        case APP_WUU_IRQN:
            APP_WUU_IRQ_HANDLER();
            WUU_ClearExternalWakeupPinsConfig(APP_WUU, APP_WUU_WAKEUP_BUTTON_IDX);
            IRQ_ClearPendingIRQ(APP_WUU_IRQN);
            break;

        default:
            break;
    }

    SystemNonMaskableInterruptSourceClear();
}
/************************** Wakeup Configuration *******************************/

/*************************** Power Mode Switch ********************************/
/*! @brief Get target low power mode. */
static app_power_mode_t APP_GetTargetPowerMode(void)
{
    uint8_t ch;

    app_power_mode_t inputPowerMode;

    do
    {
        PRINTF("\r\nSelect the desired operation \n\r\n");
        for (app_power_mode_t modeIndex = kAPP_PowerModeActive; modeIndex <= kAPP_PowerModeDeepPowerDown; modeIndex++)
        {
            PRINTF("\tPress %c to enter: %s mode\r\n", modeIndex,
                   g_modeNameArray[(uint8_t)(modeIndex - kAPP_PowerModeActive)]);
        }

        PRINTF("\r\nWaiting for power mode select...\r\n\r\n");

        ch = GETCHAR();

        if ((ch >= 'a') && (ch <= 'z'))
        {
            ch -= 'a' - 'A';
        }
        inputPowerMode = (app_power_mode_t)ch;

        if ((inputPowerMode > kAPP_PowerModeDeepPowerDown) || (inputPowerMode < kAPP_PowerModeActive))
        {
            PRINTF("Wrong Input!");
        }
    } while (inputPowerMode > kAPP_PowerModeDeepPowerDown);

    PRINTF("\t%s\r\n", g_modeDescArray[(uint8_t)(inputPowerMode - kAPP_PowerModeActive)]);

    return inputPowerMode;
}

/*! @brief Do low power mode switch. */
static void APP_PowerModeSwitch(app_power_mode_t targetPowerMode)
{
    cmc_power_domain_config_t config;

    if (targetPowerMode != kAPP_PowerModeActive)
    {
        switch (targetPowerMode)
        {
            case kAPP_PowerModeSleep:
                config.clock_mode  = kCMC_GateCoreClock;
                config.main_domain = kCMC_ActiveOrSleepMode;
                break;
            case kAPP_PowerModeDeepSleep:
                config.clock_mode  = kCMC_GateAllSystemClocksEnterLowPowerMode;
                config.main_domain = kCMC_DeepSleepMode;
                break;
            case kAPP_PowerModePowerDown:
                config.clock_mode  = kCMC_GateAllSystemClocksEnterLowPowerMode;
                config.main_domain = kCMC_PowerDownMode;
                break;
            case kAPP_PowerModeDeepPowerDown:
                config.clock_mode  = kCMC_GateAllSystemClocksEnterLowPowerMode;
                config.main_domain = kCMC_DeepPowerDown;
                break;
            default:
                assert(false);
                break;
        }

        CMC_SetPowerModeProtection(APP_CMC, kCMC_AllowAllLowPowerModes);
        CMC_EnterLowPowerMode(APP_CMC, &config);
    }
}

__WEAK void APP_PowerPreSwitchHook(app_power_mode_t targetPowerMode)
{
    (void)targetPowerMode;
}

__WEAK void APP_PowerPostSwitchHook(void)
{
}

/*************************** Power Mode Switch ********************************/
