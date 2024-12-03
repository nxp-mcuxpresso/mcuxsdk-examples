/*
 * Copyright 2022-2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_cmc.h"
#include "fsl_spc.h"
#include "fsl_clock.h"
#include "fsl_debug_console.h"
#include "power_mode_switch.h"
#include "app.h"
#include "board.h"
#include "fsl_lpuart.h"
#include "fsl_waketimer.h"
#include "fsl_wuu.h"
#include "fsl_gpio.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void APP_SetSPCConfiguration(void);
static void APP_SetVBATConfiguration(void);
static void APP_SetCMCConfiguration(void);
static void APP_InitWaketimer(void);

static app_wakeup_source_t APP_SelectWakeupSource(void);
static uint8_t APP_GetWakeupTimeout(void);
static void APP_GetWakeupConfig(app_power_mode_t targetMode);

static void APP_PowerPreSwitchHook(void);
static void APP_PowerPostSwitchHook(void);

static void APP_EnterSleepMode(void);
static void APP_EnterDeepSleepMode(void);
static void APP_EnterPowerDownMode(void);
static void APP_EnterDeepPowerDownMode(void);
static void APP_PowerModeSwitch(app_power_mode_t targetPowerMode);
static app_power_mode_t APP_GetTargetPowerMode(void);

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

    BOARD_InitHardware();

    APP_SetVBATConfiguration();
    APP_SetSPCConfiguration();
    APP_InitWaketimer();

#if !(defined(APP_NOT_SUPPORT_WAKEUP_BUTTON) && APP_NOT_SUPPORT_WAKEUP_BUTTON)
    EnableIRQ(APP_WUU_IRQN);
#endif /* APP_NOT_SUPPORT_WAKEUP_BUTTON */

    EnableIRQ(APP_WAKETIMER_IRQN);

    PRINTF("\r\nNormal Boot.\r\n");

    while (1)
    {
        if ((SPC_GetRequestedLowPowerMode(APP_SPC) & (kSPC_PowerDownWithSysClockOff | kSPC_DeepPowerDownWithSysClockOff)) != 0UL)
        {
            SPC_ClearPeriphIOIsolationFlag(APP_SPC);
        }

        /* Clear CORE domain's low power request flag. */
        SPC_ClearPowerDomainLowPowerRequestFlag(APP_SPC, APP_SPC_MAIN_POWER_DOMAIN);
        SPC_ClearLowPowerRequest(APP_SPC);

        /* Normal start. */
        APP_SetCMCConfiguration();

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
            APP_PowerPreSwitchHook();
            APP_PowerModeSwitch(targetPowerMode);
            APP_PowerPostSwitchHook();
        }

        PRINTF("\r\nNext loop.\r\n");
    }
}

static void APP_InitWaketimer(void)
{
    waketimer_config_t waketimer_config;
    WAKETIMER_GetDefaultConfig(&waketimer_config);
    WAKETIMER_Init(WAKETIMER0, &waketimer_config);
}

static void APP_SetSPCConfiguration(void)
{
    status_t status;

    spc_active_mode_regulators_config_t activeModeRegulatorOption;

    SPC_EnableSRAMLdo(APP_SPC, true);

    /* Disable analog modules that controlled by SPC in active mode. */
    SPC_DisableActiveModeAnalogModules(APP_SPC, (kSPC_controlUsb3vDet | kSPC_controlCmp0 | kSPC_controlCmp1 | kSPC_controlCmp0Dac | kSPC_controlCmp1Dac));

    /* Disable LVDs and HVDs in active mode first. */
    SPC_EnableActiveModeCoreLowVoltageDetect(APP_SPC, false);
    SPC_EnableActiveModeSystemHighVoltageDetect(APP_SPC, false);
    SPC_EnableActiveModeSystemLowVoltageDetect(APP_SPC, false);

    while (SPC_GetBusyStatusFlag(APP_SPC))
        ;

    activeModeRegulatorOption.bandgapMode = kSPC_BandgapEnabledBufferDisabled;
    /* DCDC output voltage mid voltage in active mode. */
    activeModeRegulatorOption.CoreLDOOption.CoreLDOVoltage       = APP_ACTIVE_CFG_LDO_VOLTAGE_LEVEL;
    activeModeRegulatorOption.CoreLDOOption.CoreLDODriveStrength = kSPC_CoreLDO_NormalDriveStrength;

    status = SPC_SetActiveModeRegulatorsConfig(APP_SPC, &activeModeRegulatorOption);
#if !(defined(FSL_FEATURE_MCX_SPC_HAS_NO_GLITCH_DETECT) && FSL_FEATURE_MCX_SPC_HAS_NO_GLITCH_DETECT)
    /* Disable Vdd Core Glitch detector in active mode. */
    SPC_DisableActiveModeVddCoreGlitchDetect(APP_SPC, true);
#endif
    if (status != kStatus_Success)
    {
        PRINTF("Fail to set regulators in Active mode.");
        return;
    }
    while (SPC_GetBusyStatusFlag(APP_SPC))
        ;

    /* Enable LVDs and HVDs in active mode. */
    SPC_EnableActiveModeCoreLowVoltageDetect(APP_SPC, true);
    SPC_EnableActiveModeSystemHighVoltageDetect(APP_SPC, true);
    SPC_EnableActiveModeSystemLowVoltageDetect(APP_SPC, true);

    /* Disable analog modules that controlled by SPC in low power mode. */
    SPC_DisableLowPowerModeAnalogModules(APP_SPC, (kSPC_controlUsb3vDet | kSPC_controlCmp0 | kSPC_controlCmp1 | kSPC_controlCmp0Dac | kSPC_controlCmp1Dac));
    
    /* Disable LVDs and HVDs in low power mode to save power. */
    SPC_EnableLowPowerModeCoreLowVoltageDetect(APP_SPC, false);
    SPC_EnableLowPowerModeSystemHighVoltageDetect(APP_SPC, false);
    SPC_EnableLowPowerModeSystemLowVoltageDetect(APP_SPC, false);
      
    spc_lowpower_mode_regulators_config_t lowPowerRegulatorOption;

    lowPowerRegulatorOption.lpIREF                             = false;
    lowPowerRegulatorOption.bandgapMode                        = kSPC_BandgapDisabled;
    lowPowerRegulatorOption.CoreLDOOption.CoreLDOVoltage       = kSPC_CoreLDO_MidDriveVoltage;
    lowPowerRegulatorOption.CoreLDOOption.CoreLDODriveStrength = kSPC_CoreLDO_LowDriveStrength;

    status = SPC_SetLowPowerModeRegulatorsConfig(APP_SPC, &lowPowerRegulatorOption);
#if !(defined(FSL_FEATURE_MCX_SPC_HAS_NO_GLITCH_DETECT) && FSL_FEATURE_MCX_SPC_HAS_NO_GLITCH_DETECT)
    /* Disable Vdd Core Glitch detector in low power mode. */
    SPC_DisableLowPowerModeVddCoreGlitchDetect(APP_SPC, true);
#endif
    if (status != kStatus_Success)
    {
        PRINTF("Fail to set regulators in Low Power Mode.");
        return;
    }
    while (SPC_GetBusyStatusFlag(APP_SPC))
        ;

    SPC_SetLowPowerWakeUpDelay(APP_SPC, APP_LDO_LPWKUP_DELAY);
}

static void APP_SetVBATConfiguration(void)
{
    /* Provide clock for the wake timer which is located in the system domain. */
    CLOCK_SetupFRO16KClocking(kCLKE_16K_SYSTEM);
}

static void APP_SetCMCConfiguration(void)
{
    /* Disable low power debug. */
    CMC_EnableDebugOperation(APP_CMC, false);
    /* Allow all power mode */
    CMC_SetPowerModeProtection(APP_CMC, kCMC_AllowAllLowPowerModes);

    /* Disable flash memory accesses and place flash memory in low-power state whenever the core clock
       is gated. And an attempt to access the flash memory will cause the flash memory to exit low-power
       state for the duration of the flash memory access. */
    CMC_ConfigFlashMode(APP_CMC, true, true, false);
}

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

static app_wakeup_source_t APP_SelectWakeupSource(void)
{
    char ch;
    PRINTF("Please select wakeup source:\r\n");

    PRINTF("\tPress %c to select TIMER as wakeup source;\r\n", kAPP_WakeupSourceWakeupTimer);
#if !(defined(APP_NOT_SUPPORT_WAKEUP_BUTTON) && APP_NOT_SUPPORT_WAKEUP_BUTTON)
    PRINTF("\tPress %c to select WAKE-UP-BUTTON as wakeup source;\r\n", kAPP_WakeupSourceButton);
#endif /* APP_NOT_SUPPORT_WAKEUP_BUTTON */

    PRINTF("Waiting for wakeup source select...\r\n");
    ch = GETCHAR();

    if ((ch >= 'a') && (ch <= 'z'))
    {
        ch -= 'a' - 'A';
    }

    return (app_wakeup_source_t)ch;
}

/* Get wakeup timeout and wakeup source. */
static void APP_GetWakeupConfig(app_power_mode_t targetMode)
{
    app_wakeup_source_t wakeupSource;
    uint8_t timeOutValue;
    char *isoDomains = NULL;

    wakeupSource = APP_SelectWakeupSource();

    switch (wakeupSource)
    {
        case kAPP_WakeupSourceWakeupTimer:
        {
            PRINTF("Wakeup Timer Selected As Wakeup Source!\r\n");
            timeOutValue = APP_GetWakeupTimeout();
            PRINTF("Will wakeup in %d seconds.\r\n", timeOutValue);
            /* In case of target mode is powerdown/deep power down mode. */
            WUU_SetInternalWakeUpModulesConfig(APP_WUU, APP_WUU_WAKEUP_TIMER_IDX, kWUU_InternalModuleInterrupt);
            WAKETIMER_StartTimer(WAKETIMER0, timeOutValue * 1000);

            if (targetMode > kAPP_PowerModeSleep)
            {
                /* Isolate some power domains that are not used in low power modes, note that in order to save
                 * more power, it is better to not power the isolated domains.*/
                SPC_SetExternalVoltageDomainsConfig(APP_SPC, APP_SPC_WAKEUP_TIMER_LPISO_VALUE, APP_SPC_WAKEUP_TIMER_ISO_VALUE);
                isoDomains = APP_SPC_WAKEUP_TIMER_ISO_DOMAINS;
                PRINTF("Isolate power domains: %s\r\n", isoDomains);
            }
            break;
        }
#if !(defined(APP_NOT_SUPPORT_WAKEUP_BUTTON) && APP_NOT_SUPPORT_WAKEUP_BUTTON)
        case kAPP_WakeupSourceButton:
        {
            PRINTF("Wakeup Button Selected As Wakeup Source.\r\n");
            /* Set WUU to detect on rising edge for all power modes. */
            wuu_external_wakeup_pin_config_t wakeupButtonConfig;

            wakeupButtonConfig.edge  = kWUU_ExternalPinFallingEdge;
            wakeupButtonConfig.event = kWUU_ExternalPinInterrupt;
            wakeupButtonConfig.mode  = kWUU_ExternalPinActiveAlways;
            WUU_SetExternalWakeUpPinsConfig(APP_WUU, APP_WUU_WAKEUP_BUTTON_IDX, &wakeupButtonConfig);
            PRINTF("Please press %s to wakeup.\r\n", APP_WUU_WAKEUP_BUTTON_NAME);
            if (targetMode > kAPP_PowerModeSleep)
            {
                /* Isolate some power domains that are not used in low power modes, note that in order to save
                 * more power, it is better to not power the isolated domains.*/
                SPC_SetExternalVoltageDomainsConfig(APP_SPC, APP_SPC_WAKEUP_BUTTON_LPISO_VALUE, APP_SPC_WAKEUP_BUTTON_ISO_VALUE);
                isoDomains = APP_SPC_WAKEUP_BUTTON_ISO_DOMAINS;
                PRINTF("Isolate power domains: %s\r\n", isoDomains);
            }
            break;
        }
#endif /* APP_NOT_SUPPORT_WAKEUP_BUTTON */
        default:
            assert(false);
            break;
    }
}

/*!
 * Get input from user about wakeup timeout
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

static void APP_PowerPreSwitchHook(void)
{
    /* Wait for debug console output finished. */
    while (!(kLPUART_TransmissionCompleteFlag & LPUART_GetStatusFlags((LPUART_Type *)BOARD_DEBUG_UART_BASEADDR)))
    {
    }
    APP_DeinitDebugConsole();
}

static void APP_PowerPostSwitchHook(void)
{
#if !(defined(APP_POST_INIT_CLOCK) && APP_POST_INIT_CLOCK)
    BOARD_BootClockFRO48M();
#else
    APP_POST_INIT_CLOCK_FUNCTION();
#endif
    APP_InitDebugConsole();
}

static void APP_PowerModeSwitch(app_power_mode_t targetPowerMode)
{
    if (targetPowerMode != kAPP_PowerModeActive)
    {
        switch (targetPowerMode)
        {
            case kAPP_PowerModeSleep:
                APP_EnterSleepMode();
                break;
            case kAPP_PowerModeDeepSleep:
                APP_EnterDeepSleepMode();
                break;
            case kAPP_PowerModePowerDown:
                APP_EnterPowerDownMode();
                break;
            case kAPP_PowerModeDeepPowerDown:
                APP_EnterDeepPowerDownMode();
                break;
            default:
                assert(false);
                break;
        }
    }
}

static void APP_EnterSleepMode(void)
{
    cmc_power_domain_config_t config;

    config.clock_mode  = kCMC_GateCoreClock;
    config.main_domain = kCMC_ActiveOrSleepMode;

    CMC_EnterLowPowerMode(APP_CMC, &config);
}

static void APP_EnterDeepSleepMode(void)
{
    cmc_power_domain_config_t config;

    config.clock_mode  = kCMC_GateAllSystemClocksEnterLowPowerMode;
    config.main_domain = kCMC_DeepSleepMode;

    CMC_EnterLowPowerMode(APP_CMC, &config);
}

static void APP_EnterPowerDownMode(void)
{
    cmc_power_domain_config_t config;

    config.clock_mode  = kCMC_GateAllSystemClocksEnterLowPowerMode;
    config.main_domain = kCMC_PowerDownMode;

    CMC_EnterLowPowerMode(APP_CMC, &config);
}

static void APP_EnterDeepPowerDownMode(void)
{
    cmc_power_domain_config_t config;

    config.clock_mode  = kCMC_GateAllSystemClocksEnterLowPowerMode;
    config.main_domain = kCMC_DeepPowerDown;

    CMC_EnterLowPowerMode(APP_CMC, &config);
}
