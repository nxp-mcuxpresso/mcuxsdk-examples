/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
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
#include "fsl_vbat.h"

/*${header:end}*/

extern pm_handle_t g_pmHandle;

/*${function:start}*/

AT_ALWAYS_ON_DATA_INIT(pm_notify_element_t g_notify1) = {
    .notifyCallback = APP_UartControlCallback,
    .data           = NULL,
};

AT_ALWAYS_ON_DATA_INIT(pm_notify_element_t g_notify2) = {
    .notifyCallback = APP_EccReInitCallback,
    .data           = NULL,
};

AT_ALWAYS_ON_DATA(pm_wakeup_source_t g_lptmr0WakeupSource);

void LPTMR0_IRQHandler(void)
{
    PM_TriggerWakeSourceService(&g_lptmr0WakeupSource);
}

#define APP_SPC  (SPC0)
#define APP_VBAT (VBAT0)

void BOARD_InitHardware(void)
{
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    if ((CMC_GetSystemResetStatus(CMC0) & kCMC_WakeUpReset) != 0UL)
    {
        /* Close ISO. */
        SPC_ClearPeriphIOIsolationFlag(SPC0);
    }

    CMC_EnableDebugOperation(CMC0, false);

    static volatile status_t status = kStatus_Success;

    /* Disable LVDs and HVDs in Active mode. */
    SPC_EnableActiveModeCoreHighVoltageDetect(APP_SPC, false);
    SPC_EnableActiveModeCoreLowVoltageDetect(APP_SPC, false);
    SPC_EnableActiveModeSystemHighVoltageDetect(APP_SPC, false);
    SPC_EnableActiveModeSystemLowVoltageDetect(APP_SPC, false);
    SPC_EnableActiveModeIOHighVoltageDetect(APP_SPC, false);
    SPC_EnableActiveModeIOLowVoltageDetect(APP_SPC, false);

    while (SPC_GetBusyStatusFlag(APP_SPC))
    {
    }

    spc_active_mode_regulators_config_t activeModeRegulatorOption;
    activeModeRegulatorOption.bandgapMode                      = kSPC_BandgapEnabledBufferDisabled;
    activeModeRegulatorOption.lpBuff                           = false;
    activeModeRegulatorOption.DCDCOption.DCDCVoltage           = kSPC_DCDC_NormalVoltage; /* DCDC regulate to 1.5V. */
    activeModeRegulatorOption.DCDCOption.DCDCDriveStrength     = kSPC_DCDC_NormalDriveStrength;
    activeModeRegulatorOption.SysLDOOption.SysLDOVoltage       = kSPC_SysLDO_NormalVoltage;
    activeModeRegulatorOption.SysLDOOption.SysLDODriveStrength = kSPC_SysLDO_NormalDriveStrength;
    activeModeRegulatorOption.CoreLDOOption.CoreLDOVoltage =
        kSPC_CoreLDO_NormalVoltage; /* Core LDO regulate to 1.1V. */
#if defined(FSL_FEATURE_SPC_HAS_CORELDO_VDD_DS) && FSL_FEATURE_SPC_HAS_CORELDO_VDD_DS
    activeModeRegulatorOption.CoreLDOOption.CoreLDODriveStrength = kSPC_CoreLDO_NormalDriveStrength;
#endif /* FSL_FEATURE_SPC_HAS_CORELDO_VDD_DS */
    if (SPC_SetActiveModeDCDCRegulatorConfig(APP_SPC, &activeModeRegulatorOption.DCDCOption) != kStatus_Success)
    {
        PRINTF("Fail to set DCDC in Active Mode!\r\n");
    }
    while (SPC_GetBusyStatusFlag(APP_SPC))
    {
    }
    if (SPC_SetActiveModeSystemLDORegulatorConfig(APP_SPC, &activeModeRegulatorOption.SysLDOOption) != kStatus_Success)
    {
        PRINTF("Fail to set System LDO in Active Mode!\r\n ");
    }
    if (SPC_SetActiveModeBandgapModeConfig(APP_SPC, activeModeRegulatorOption.bandgapMode) != kStatus_Success)
    {
        PRINTF("Fail to set bandgap mode in Active Mode!\r\n");
    }
    status = SPC_SetActiveModeCoreLDORegulatorConfig(APP_SPC, &activeModeRegulatorOption.CoreLDOOption);
    if (status != kStatus_Success)
    {
        PRINTF("Fail to set Core LDO in Active mode!\r\n");
    }
    SPC_EnableActiveModeCMPBandgapBuffer(APP_SPC, activeModeRegulatorOption.lpBuff);

    spc_lowpower_mode_regulators_config_t lowPowerRegulatorOption;

    lowPowerRegulatorOption.lpIREF                             = false;
    lowPowerRegulatorOption.bandgapMode                        = kSPC_BandgapDisabled;
    lowPowerRegulatorOption.lpBuff                             = false;
    lowPowerRegulatorOption.CoreIVS                            = false;
    lowPowerRegulatorOption.DCDCOption.DCDCVoltage             = kSPC_DCDC_LowUnderVoltage;
    lowPowerRegulatorOption.DCDCOption.DCDCDriveStrength       = kSPC_DCDC_LowDriveStrength;
    lowPowerRegulatorOption.SysLDOOption.SysLDODriveStrength   = kSPC_SysLDO_LowDriveStrength;
    lowPowerRegulatorOption.CoreLDOOption.CoreLDOVoltage       = kSPC_CoreLDO_MidDriveVoltage;
    lowPowerRegulatorOption.CoreLDOOption.CoreLDODriveStrength = kSPC_CoreLDO_LowDriveStrength;

    status = SPC_SetLowPowerModeRegulatorsConfig(APP_SPC, &lowPowerRegulatorOption);
    if (status != kStatus_Success)
    {
        PRINTF("Fail to set regulators in Low Power Mode.");
        return;
    }

    SPC_SetLowPowerWakeUpDelay(APP_SPC, 0xFFFFU);

    VBAT_EnableBackupSRAMRegulator(APP_VBAT, false);
    VBAT_EnableFRO16k(APP_VBAT, false);
    while (VBAT_CheckFRO16kEnabled(APP_VBAT))
        ;

    /* Isolate some external power domain to save power. */
    SPC_SetExternalVoltageDomainsConfig(APP_SPC, 0x7U, 0x0U);

    /* Enable CORE VDD Voltage scaling. */
    SPC_EnableLowPowerModeCoreVDDInternalVoltageScaling(APP_SPC, true);
}

status_t APP_EccReInitCallback(pm_event_type_t eventType, uint8_t powerState, void *data)
{
    if ((eventType == kPM_EventExitingSleep) && (powerState == PM_LP_STATE_DEEP_POWER_DOWN))
    {
        uint32_t ramAddress = 0x4000000UL;
        while (ramAddress < 0x4004000UL)
        {
            memset((void *)(uint32_t *)ramAddress, 0UL, sizeof(uint32_t));
            ramAddress = ramAddress + 4UL;
        }
        ramAddress = 0x20000000;
        while (ramAddress < 0x20010000)
        {
            memset((void *)(uint32_t *)ramAddress, 0UL, sizeof(uint32_t));
            ramAddress = ramAddress + 4UL;
        }
    }
    return kStatus_Success;
}

status_t APP_UartControlCallback(pm_event_type_t eventType, uint8_t powerState, void *data)
{
    if (eventType == kPM_EventEnteringSleep)
    {
        if (powerState >= PM_LP_STATE_POWER_DOWN)
        {
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

        /* Set NBU into Deep Sleep Mode */
        RFMC->RF2P4GHZ_CTRL =
            (RFMC->RF2P4GHZ_CTRL & (~RFMC_RF2P4GHZ_CTRL_LP_MODE_MASK)) | RFMC_RF2P4GHZ_CTRL_LP_MODE(0x3);
        RFMC->RF2P4GHZ_CTRL |= RFMC_RF2P4GHZ_CTRL_LP_ENTER_MASK;
    }
    else
    {
        /* Re-init uart. */
        BOARD_InitPins();
        BOARD_BootClockRUN();
        BOARD_InitDebugConsole();
        if ((CMC_GetSystemResetStatus(CMC0) & kCMC_WakeUpReset) != 0UL)
        {
            /* Close ISO. */
            SPC_ClearPeriphIOIsolationFlag(SPC0);
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
    if (kLPTMR_TimerInterruptEnable & LPTMR_GetEnabledInterrupts(LPTMR0))
    {
        LPTMR_DisableInterrupts(LPTMR0, kLPTMR_TimerInterruptEnable);
        LPTMR_ClearStatusFlags(LPTMR0, kLPTMR_TimerCompareFlag);
        LPTMR_StopTimer(LPTMR0);
    }
}

void APP_StartLptmr(uint64_t timeOutTickes)
{
    const lptmr_config_t DEMO_LPTMR_config = {.timerMode            = kLPTMR_TimerModeTimeCounter,
                                              .pinSelect            = kLPTMR_PinSelectInput_0,
                                              .pinPolarity          = kLPTMR_PinPolarityActiveHigh,
                                              .enableFreeRunning    = false,
                                              .bypassPrescaler      = true,
                                              .prescalerClockSource = kLPTMR_PrescalerClock_2,
                                              .value                = kLPTMR_Prescale_Glitch_0};

    LPTMR_Init(APP_LPTMR, &DEMO_LPTMR_config);
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
            timeoutTicks = (32000UL * timeout) - 1UL;
            return timeoutTicks;
        }
        PRINTF("Wrong value!\r\n");
    }
}

void APP_RegisterNotify(void)
{
    PM_RegisterNotify(kPM_NotifyGroup0, &g_notify1);
    PM_RegisterNotify(kPM_NotifyGroup2, &g_notify2);
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

/*${function:end}*/
