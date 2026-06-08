/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include <string.h>

#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"

#include "fsl_pm_core.h"
#include "fsl_pm_device.h"
#include "fsl_lptmr.h"
#include "app.h"
#include "fsl_debug_console.h"
#include "fsl_lpuart.h"
#include "fsl_port.h"
#include "fsl_cmc.h"
#include "fsl_spc.h"
#include "fsl_clock.h"
/*${header:end}*/

extern pm_handle_t g_pmHandle;

/*${function:start}*/

static void APP_WaitForSPCReady(void);
static void APP_SetSPCConfiguration(void);
static void APP_SetCMCConfiguration(void);

AT_ALWAYS_ON_DATA_INIT(pm_notify_element_t g_notify1) = {
    .notifyCallback = APP_UartControlCallback,
    .data           = NULL,
};

AT_ALWAYS_ON_DATA(pm_wakeup_source_t g_lptmr0WakeupSource);

void LPTMR0_IRQHandler(void)
{
    PM_TriggerWakeSourceService(&g_lptmr0WakeupSource);
}

/*
 * KW43 boots Core1 by default when the shared RF domain is released from reset.
 * Keep it in reset for the single-core power manager example.
 */
static inline void BOARD_HoldCore1InReset(void)
{
    RFMC->RF2P4GHZ_CTRL |= RFMC_RF2P4GHZ_CTRL_CPU_RST_MASK;
}

/*
 * Drive the RF 2.4 GHz (NBU) low-power entry that matches the host power state.
 * The NBU/Core1 is held in reset in this single-core example and no NBU firmware
 * runs to manage its own low power, so the application owns the radio low-power
 * entry. Programming the per-state LP_MODE via RFMC.RF2P4GHZ_CTRL lets Deep Sleep
 * and deeper modes reach the radio low-power state and wake/resume as documented.
 */
static void APP_ApplyRadioLowPowerMode(uint8_t powerState)
{
    uint32_t rfCtrl;
    uint32_t lpMode;

    switch (powerState)
    {
        case PM_LP_STATE_SLEEP:
            lpMode = 0x1U;
            break;
        case PM_LP_STATE_DEEP_SLEEP:
            lpMode = 0x3U;
            break;
        case PM_LP_STATE_POWER_DOWN:
            lpMode = 0x7U;
            break;
        case PM_LP_STATE_DEEP_POWER_DOWN:
            RF_CMC1->RAM_PWR = 0x000004FFU;
            lpMode = 0xFU;
            break;
        default:
            return;
    }

    rfCtrl = RFMC->RF2P4GHZ_CTRL & ~RFMC_RF2P4GHZ_CTRL_LP_MODE_MASK;
    RFMC->RF2P4GHZ_CTRL = rfCtrl | RFMC_RF2P4GHZ_CTRL_LP_MODE(lpMode);
    RFMC->RF2P4GHZ_CTRL |= RFMC_RF2P4GHZ_CTRL_LP_ENTER_MASK;
}

void BOARD_InitHardware(void)
{
    BOARD_HoldCore1InReset();
    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    if ((CMC_GetSystemResetStatus(APP_CMC) & kCMC_WakeUpReset) != 0UL)
    {
        /* Close ISO. */
        SPC_ClearPeriphIOIsolationFlag(APP_SPC);
    }

    APP_SetSPCConfiguration();
    APP_SetCMCConfiguration();
}

status_t APP_UartControlCallback(pm_event_type_t eventType, uint8_t powerState, void *data)
{
    if (eventType == kPM_EventEnteringSleep)
    {
        /* The application drives the NBU/radio low-power entry before CMC
         * low-power entry. */
        APP_ApplyRadioLowPowerMode(powerState);

        if (powerState >= PM_LP_STATE_POWER_DOWN)
        {
            PRINTF("KW43 A0 ROM resumes Power Down and Deep Power Down through normal boot.\r\n");
            PRINTF("Note: Wakeup cause a reset!\r\n");
        }
        /* De-init uart */
        PRINTF("De-init UART.\r\n");
        /* Wait for debug console output finished. */
        while (!(kLPUART_TransmissionCompleteFlag & LPUART_GetStatusFlags((LPUART_Type *)BOARD_DEBUG_UART_BASEADDR)))
        {
        }
        DbgConsole_Deinit();
        /*
         * Set pin for current leakage.
         * Debug console RX pin: Set to pinmux to disable.
         * Debug console TX pin: Don't need to change.
         */
        PORT_SetPinMux(DEBUG_CONSOLE_RX_PORT, DEBUG_CONSOLE_RX_PIN, kPORT_PinDisabledOrAnalog);
    }
    else
    {
        /* Re-init uart. */
        BOARD_InitBootPins();
        BOARD_BootClockRUN();
        BOARD_InitDebugConsole();
        if ((CMC_GetSystemResetStatus(APP_CMC) & kCMC_WakeUpReset) != 0UL)
        {
            /* Close ISO. */
            SPC_ClearPeriphIOIsolationFlag(APP_SPC);
        }
        PRINTF("Re-init UART.\r\n");
    }

    return kStatus_Success;
}

void APP_InitWakeupSource(void)
{
    PM_InitWakeupSource(&g_lptmr0WakeupSource, PM_WSID_LPTMR0, APP_Lptmr0WakeupService, true);
    PM_RegisterTimerController(&g_pmHandle, APP_StartLptmr, APP_StopLptmr, NULL, NULL);
}

void APP_Lptmr0WakeupService(void)
{
    if (kLPTMR_TimerInterruptEnable & LPTMR_GetEnabledInterrupts(APP_LPTMR))
    {
        LPTMR_DisableInterrupts(APP_LPTMR, kLPTMR_TimerInterruptEnable);
        LPTMR_ClearStatusFlags(APP_LPTMR, kLPTMR_TimerCompareFlag);
        LPTMR_StopTimer(APP_LPTMR);
    }
}

void APP_StartLptmr(uint64_t timeOutTickes)
{
    const lptmr_config_t DEMO_LPTMR_config = {.timerMode            = kLPTMR_TimerModeTimeCounter,
                                              .pinSelect            = kLPTMR_PinSelectInput_0,
                                              .pinPolarity          = kLPTMR_PinPolarityActiveHigh,
                                              .enableFreeRunning    = false,
                                              .bypassPrescaler      = true,
                                              .prescalerClockSource = kLPTMR_PrescalerClock_0,
                                              .value                = kLPTMR_Prescale_Glitch_0};

    /*
     * KW43 routes the LPTMR prescaler clock via the per-IP clock mux.  Select
     * the 32 kHz source and keep the clock running in all low-power modes so
     * that the counter can still fire a WUU wakeup from Power Down.
     */
    CLOCK_SetIpSrc(kCLOCK_Lptmr0, kCLOCK_IpSrc32kClk);
    LPTMR_Init(APP_LPTMR, &DEMO_LPTMR_config);
    CLOCK_EnableClockLPMode(kCLOCK_Lptmr0, kCLOCK_IpClkControl_fun3);
    LPTMR_SetTimerPeriod(APP_LPTMR, (uint32_t)timeOutTickes);
    LPTMR_EnableInterrupts(APP_LPTMR, kLPTMR_TimerInterruptEnable);
    LPTMR_StartTimer(APP_LPTMR);
}

void APP_StopLptmr(void)
{
    LPTMR_StopTimer(APP_LPTMR);
}

uint32_t APP_GetWakeupTimeout(void)
{
    uint8_t timeout;
    uint32_t timeoutTicks;

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
            timeout -= '0';
            PRINTF("Will wakeup in %d seconds.\r\n", timeout);
            timeoutTicks = (APP_LPTMR_TICKS_PER_SECOND * timeout) - 1UL;
            return timeoutTicks;
        }
        PRINTF("Wrong value!\r\n");
    }
}

void APP_RegisterNotify(void)
{
    PM_RegisterNotify(kPM_NotifyGroup0, &g_notify1);
}

void APP_SetConstraints(uint8_t powerMode)
{
    switch (powerMode)
    {
        case PM_LP_STATE_SLEEP:
        {
            PM_SetConstraints(PM_LP_STATE_SLEEP, APP_SLEEP_CONSTRAINTS);
            break;
        }

        case PM_LP_STATE_DEEP_SLEEP:
        {
            PM_SetConstraints(PM_LP_STATE_DEEP_SLEEP, APP_DEEP_SLEEP_CONSTRAINTS);
            break;
        }

        case PM_LP_STATE_POWER_DOWN:
        {
            PM_SetConstraints(PM_LP_STATE_POWER_DOWN, APP_POWER_DOWN_CONSTRAINTS);
            break;
        }

        case PM_LP_STATE_DEEP_POWER_DOWN:
        {
            PM_SetConstraints(PM_LP_STATE_DEEP_POWER_DOWN, APP_DEEP_POWER_DOWN_CONSTRAINTS);
            break;
        }

        default:
        {
            /* This branch will never be hit. */
            break;
        }
    }
}

void APP_ReleaseConstraints(uint8_t powerMode)
{
    switch (powerMode)
    {
        case PM_LP_STATE_SLEEP:
        {
            PM_ReleaseConstraints(PM_LP_STATE_SLEEP, APP_SLEEP_CONSTRAINTS);
            break;
        }
        case PM_LP_STATE_DEEP_SLEEP:
        {
            PM_ReleaseConstraints(PM_LP_STATE_DEEP_SLEEP, APP_DEEP_SLEEP_CONSTRAINTS);
            break;
        }
        case PM_LP_STATE_POWER_DOWN:
        {
            PM_ReleaseConstraints(PM_LP_STATE_POWER_DOWN, APP_POWER_DOWN_CONSTRAINTS);
            break;
        }
        case PM_LP_STATE_DEEP_POWER_DOWN:
        {
            PM_ReleaseConstraints(PM_LP_STATE_DEEP_POWER_DOWN, APP_DEEP_POWER_DOWN_CONSTRAINTS);
            break;
        }
        default:
        {
            /* This branch should never be hit. */
            break;
        }
    }
}

static void APP_WaitForSPCReady(void)
{
    while (SPC_GetBusyStatusFlag(APP_SPC))
    {
    }
}

/*!
 * @brief In active mode, all HVDs/LVDs are disabled, DCDC regulated to 1.5V, Core LDO regulated to 1.1V;
 * In low power modes, all HVDs/LVDs are disabled, bandgap is disabled, DCDC regulated to 1.25V, Core LDO regulated to
 * 1.0V.
 *
 * @note KW43 boots with LP_CFG Core LDO voltage set to a level different from
 * kSPC_CoreLDO_MidDriveVoltage, so a single SPC_SetLowPowerModeRegulatorsConfig
 * call that also drops Drive Strength to Low is rejected by the SPC driver
 * interlock (LP Core LDO voltage can only change while Drive Strength is
 * Normal -- see fsl_spc.c:1051). The LP Core LDO transition is therefore
 * staged in two steps: first set the target voltage with Normal drive
 * strength, then the full LP regulator config takes the Low-drive path.
 *
 * The DCDC and LP regulator writes also use a short retry loop so a transient
 * kStatus_SPC_Busy does not fail the configuration.
 */
static void APP_SetSPCConfiguration(void)
{
    status_t status;
    uint32_t retry;
    spc_active_mode_regulators_config_t activeModeRegulatorOption;
    spc_lowpower_mode_regulators_config_t lowPowerRegulatorOption;

    (void)memset(&activeModeRegulatorOption, 0, sizeof(activeModeRegulatorOption));
    (void)memset(&lowPowerRegulatorOption, 0, sizeof(lowPowerRegulatorOption));

    APP_WaitForSPCReady();

    /* Disable LVDs and HVDs in Active mode. */
    SPC_EnableActiveModeCoreHighVoltageDetect(APP_SPC, false);
    SPC_EnableActiveModeCoreLowVoltageDetect(APP_SPC, false);
    SPC_EnableActiveModeVDD1P8LowVoltageDetect(APP_SPC, false);
    SPC_EnableActiveModeIOHighVoltageDetect(APP_SPC, false);
    SPC_EnableActiveModeIOLowVoltageDetect(APP_SPC, false);

    APP_WaitForSPCReady();

    activeModeRegulatorOption.bandgapMode                        = kSPC_BandgapEnabledBufferDisabled;
    activeModeRegulatorOption.DCDCOption.DCDCVoltage             = kSPC_DCDC_NormalVoltage; /* DCDC regulate to 1.5V. */
    activeModeRegulatorOption.DCDCOption.DCDCDriveStrength       = kSPC_DCDC_NormalDriveStrength;
    activeModeRegulatorOption.CoreLDOOption.CoreLDOVoltage       = kSPC_CoreLDO_NormalVoltage; /* Core LDO regulate to 1.1V. */
    activeModeRegulatorOption.CoreLDOOption.CoreLDODriveStrength = kSPC_CoreLDO_NormalDriveStrength;

    status = kStatus_SPC_Busy;
    for (retry = 0U; (retry < 8U) && (status == kStatus_SPC_Busy); retry++)
    {
        APP_WaitForSPCReady();
        status = SPC_SetActiveModeDCDCRegulatorConfig(APP_SPC, &activeModeRegulatorOption.DCDCOption);
    }
    if (status != kStatus_Success)
    {
        PRINTF("Fail to set DCDC in Active Mode!\r\n");
    }
    APP_WaitForSPCReady();
    if (SPC_SetActiveModeBandgapModeConfig(APP_SPC, activeModeRegulatorOption.bandgapMode) != kStatus_Success)
    {
        PRINTF("Fail to set bandgap mode in Active Mode!\r\n");
    }
    status = SPC_SetActiveModeCoreLDORegulatorConfig(APP_SPC, &activeModeRegulatorOption.CoreLDOOption);
    if (status != kStatus_Success)
    {
        PRINTF("Fail to set Core LDO in Active mode!\r\n");
    }

    APP_WaitForSPCReady();

    /* KW43 (MW30): LP IREF is a shared PMC current reference that biases
     * both OSC32K and the VDD_SYS_LV always-on domain. Disabling
     * LP_CFG[LP_IREFEN] in DPDOWN would corrupt VDD_SYS_LV AON, so keep
     * LP_IREF enabled. (In every non-DPDOWN LP mode the hardware forces
     * LP_IREFEN to 1 anyway) */
    lowPowerRegulatorOption.lpIREF                             = true;
    lowPowerRegulatorOption.bandgapMode                        = kSPC_BandgapDisabled;
    lowPowerRegulatorOption.CoreIVS                            = false;
    lowPowerRegulatorOption.DCDCOption.DCDCVoltage             = kSPC_DCDC_LowUnderVoltage;
    lowPowerRegulatorOption.DCDCOption.DCDCDriveStrength       = kSPC_DCDC_LowDriveStrength;
    lowPowerRegulatorOption.CoreLDOOption.CoreLDOVoltage       = kSPC_CoreLDO_MidDriveVoltage;
    lowPowerRegulatorOption.CoreLDOOption.CoreLDODriveStrength = kSPC_CoreLDO_LowDriveStrength;

    /*
     * Stage the LP Core LDO transition in two steps so the SPC driver's
     * "voltage can only change while Drive Strength is Normal" interlock
     * does not reject it. First set the target voltage with Normal drive
     * strength (allowed path); the full LP regulator config below then
     * takes the Low-drive path because preVoltage already matches. The
     * staged call's bandgap setting is overwritten by the final LP
     * regulator config, so this does not change the final LP bandgap state.
     */
    spc_lowpower_mode_core_ldo_option_t stagedLowPowerCoreLDOOption = lowPowerRegulatorOption.CoreLDOOption;
    stagedLowPowerCoreLDOOption.CoreLDODriveStrength                = kSPC_CoreLDO_NormalDriveStrength;

    APP_WaitForSPCReady();
    status = SPC_SetLowPowerModeBandgapmodeConfig(APP_SPC, kSPC_BandgapEnabledBufferDisabled);
    if (status != kStatus_Success)
    {
        PRINTF("Fail to stage bandgap for Low Power Mode, status = 0x%x.\r\n", status);
        return;
    }

    APP_WaitForSPCReady();
    status = SPC_SetLowPowerModeCoreLDORegulatorConfig(APP_SPC, &stagedLowPowerCoreLDOOption);
    if (status != kStatus_Success)
    {
        PRINTF("Fail to stage Core LDO for Low Power Mode, status = 0x%x.\r\n", status);
        return;
    }

    status = kStatus_SPC_Busy;
    for (retry = 0U; (retry < 8U) && (status == kStatus_SPC_Busy); retry++)
    {
        APP_WaitForSPCReady();
        status = SPC_SetLowPowerModeRegulatorsConfig(APP_SPC, &lowPowerRegulatorOption);
    }

    if (status != kStatus_Success)
    {
        PRINTF("Fail to set regulators in Low Power Mode, status = 0x%x.\r\n", status);
        return;
    }

    APP_WaitForSPCReady();
    SPC_SetLowPowerWakeUpDelay(APP_SPC, 0xFFFFU);

    /* Enable CORE VDD Voltage scaling in low-power mode. */
    SPC_EnableLowPowerModeCoreVDDInternalVoltageScaling(APP_SPC, true);
}

static void APP_SetCMCConfiguration(void)
{
    CMC_SetPowerModeProtection(APP_CMC, kCMC_AllowAllLowPowerModes);
    CMC_LockPowerModeProtectionSetting(APP_CMC);
    CMC_EnableDebugOperation(APP_CMC, false);
    CMC_ConfigFlashMode(APP_CMC, false, true, false);
}

/*${function:end}*/
