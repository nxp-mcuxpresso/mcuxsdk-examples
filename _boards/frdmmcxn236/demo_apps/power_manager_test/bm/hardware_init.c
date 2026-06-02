/*
 * Copyright 2024-2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_cmc.h"
#include "fsl_pm_core.h"
#include "fsl_pm_device.h"
#include "fsl_lptmr.h"
#include "fsl_lpuart.h"
#include "fsl_port.h"
#include "fsl_spc.h"
#include "fsl_vbat.h"
#include "fsl_port.h"
#include "fsl_wuu.h"
/*${header:end}*/

/*${variable:start}*/
extern pm_handle_t g_pmHandle;
/*${variable:end}*/

/*${function:start}*/
AT_ALWAYS_ON_DATA(pm_wakeup_source_t g_wakeupSource);
AT_ALWAYS_ON_DATA(pm_wakeup_source_t g_lptmr0WakeupSource);

static void APP_BootClockFROHF48M(void)
{
    CLOCK_EnableClock(kCLOCK_Scg);

    /* FRO OSC setup - begin, attach FRO12M to MainClock for safety switching */
    CLOCK_AttachClk(kFRO12M_to_MAIN_CLK);

    /* Set the LDO_CORE VDD regulator to 1.0 V voltage level */
    spc_active_mode_core_ldo_option_t ldoOpt = {
      .CoreLDOVoltage       = kSPC_CoreLDO_MidDriveVoltage,
      .CoreLDODriveStrength = kSPC_CoreLDO_NormalDriveStrength,
    };
    SPC_SetActiveModeCoreLDORegulatorConfig(SPC0, &ldoOpt);

    /* The DCDC VDD regulator voltage level is 1.0V. */
    spc_active_mode_dcdc_option_t dcdcOpt = {
      .DCDCVoltage       = kSPC_DCDC_MidVoltage,
      .DCDCDriveStrength = kSPC_DCDC_NormalDriveStrength,
    };
    SPC_SetActiveModeDCDCRegulatorConfig(SPC0, &dcdcOpt);
    
    /* Configure Flash wait-states to support 1.0V voltage level and 48000000Hz frequency */;
    FMU0->FCTRL = (FMU0->FCTRL & ~((uint32_t)FMU_FCTRL_RWSC_MASK)) | (FMU_FCTRL_RWSC(0x1U));

    /* SRAM's read/write operating voltage timing margin is 1.0V. */
    spc_sram_voltage_config_t sramCfg = {
      .operateVoltage       = kSPC_sramOperateAt1P0V,
      .requestVoltageUpdate = true,
    };
    SPC_SetSRAMOperateVoltage(SPC0, &sramCfg);

    CLOCK_SetupFROHFClocking(48000000U);
    CLOCK_AttachClk(kFRO_HF_to_MAIN_CLK);
    CLOCK_SetClkDiv(kCLOCK_DivAhbClk, 1U);

    /* Set SystemCoreClock variable */
    SystemCoreClock = BOARD_BOOTCLOCKFROHF48M_CORE_CLOCK;
    
    SCG0->LDOCSR &= ~SCG_LDOCSR_LDOEN_MASK;
}

void BOARD_InitHardware(void)
{
    APP_BootClockFROHF48M();
      
    BOARD_InitPins();

    CLOCK_AttachClk(kFRO_HF_DIV_to_FLEXCOMM4);
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom4Clk, 4U);
    DbgConsole_Init(BOARD_DEBUG_UART_INSTANCE, BOARD_DEBUG_UART_BAUDRATE, BOARD_DEBUG_UART_TYPE, BOARD_DEBUG_UART_CLK_FREQ);

    /* Disable unused clocks. */
    CLOCK_DisableClock(kCLOCK_Rom);
    CLOCK_DisableClock(kCLOCK_PkcRam);
    CLOCK_DisableClock(kCLOCK_Pkc);
    CLOCK_DisableClock(kCLOCK_Gdet);
    CLOCK_DisableClock(kCLOCK_Port1);

    SPC_DisableActiveModeAnalogModules(SPC0, kSPC_controlUsb3vDet);
    SPC_DisableLowPowerModeAnalogModules(SPC0, kSPC_controlUsb3vDet);

    VBAT_EnableFRO16k(VBAT0, false);

    CMC_ConfigFlashMode(CMC0, true, false);
    CMC_EnableDebugOperation(CMC0, false);

    if ((SPC_GetRequestedLowPowerMode(SPC0) & (kSPC_PowerDownWithSysClockOff | kSPC_DeepPowerDownWithSysClockOff)) != 0UL)
    {
        SPC_ClearPeriphIOIsolationFlag(SPC0);
    }
    
    ConstructResourceInformation(rescArray);
}

static status_t APP_UartControl(pm_event_type_t eventType, uint8_t powerState, void *data)
{
    if (eventType == kPM_EventEnteringSleep)
    {
        PRINTF("\r\n De-init UART.");
        while (!(kLPUART_TransmissionCompleteFlag & LPUART_GetStatusFlags((LPUART_Type *)BOARD_DEBUG_UART_BASEADDR)))
        {
        }
        CLOCK_DisableClock(kCLOCK_LPFlexComm4);
    }
    else
    {
        CLOCK_EnableClock(kCLOCK_LPFlexComm4);
        PRINTF("\r\n Re-init UART."); 
    }

    return kStatus_Success;
}

static status_t APP_EntryPowerModeInfoPrint(pm_event_type_t eventType, uint8_t powerState, void *data)
{
    if (eventType == kPM_EventEnteringSleep)
    {
        if (powerState == 3U)
        {
            PRINTF("\r\n Please note that exiting from deep power down will cause wakeup reset.");
        }
    }

    return kStatus_Success;
}

AT_ALWAYS_ON_DATA_INIT(pm_notify_element_t g_notify0) = {
    .notifyCallback = APP_EntryPowerModeInfoPrint,
    .data           = NULL,
};

AT_ALWAYS_ON_DATA_INIT(pm_notify_element_t g_notify1) = {
    .notifyCallback = APP_UartControl,
    .data           = NULL,
};

/*! @brief Wakeup source service function, should be executed if the corresponding wakeup event occurred. */
void APP_Lptmr0WakeupService(void)
{
    if (kLPTMR_TimerInterruptEnable & LPTMR_GetEnabledInterrupts(LPTMR0))
    {
        LPTMR_DisableInterrupts(LPTMR0, kLPTMR_TimerInterruptEnable);
        LPTMR_ClearStatusFlags(LPTMR0, kLPTMR_TimerCompareFlag);
        LPTMR_StopTimer(LPTMR0);
    }
}

void LPTMR0_IRQHandler(void)
{
    PM_TriggerWakeSourceService(&g_lptmr0WakeupSource);
}

/*! @brief Start lptmr before entering low power mode. */
void APP_StartLptmr(uint64_t timeOutTickes)
{
    const lptmr_config_t DEMO_LPTMR_config = {.timerMode            = kLPTMR_TimerModeTimeCounter,
                                              .pinSelect            = kLPTMR_PinSelectInput_0,
                                              .pinPolarity          = kLPTMR_PinPolarityActiveHigh,
                                              .enableFreeRunning    = false,
                                              .bypassPrescaler      = true,
                                              .prescalerClockSource = kLPTMR_PrescalerClock_1,
                                              .value                = kLPTMR_Prescale_Glitch_0};

    VBAT_UngateFRO16k(VBAT0, kCLOCK_Clk16KToVsys);
    LPTMR_Init(LPTMR0, &DEMO_LPTMR_config);
    LPTMR_SetTimerPeriod(LPTMR0, USEC_TO_COUNT(timeOutTickes, 16384UL));
    LPTMR_EnableInterrupts(LPTMR0, kLPTMR_TimerInterruptEnable);
    LPTMR_StartTimer(LPTMR0);
}

/*! @brief stop low power timer after waking up from low power mode. */
void APP_StopLptmr(void)
{
    LPTMR_StopTimer(LPTMR0);
}

/*! @brief Init wakeuup source, insert it to the wakeup source list, if the wakeup source is the lptmr,
 *  also need to register the timer controller to the power manager handle.
 */
void APP_InitWakeupSource(void)
{
    PM_InitWakeupSource(&g_lptmr0WakeupSource, PM_WSID_LPTMR0, APP_Lptmr0WakeupService, true);
    PM_RegisterTimerController(&g_pmHandle, APP_StartLptmr, APP_StopLptmr, NULL, NULL);
}

uint32_t APP_GetWakeupTimeout(void)
{
    uint8_t timeout;
    uint32_t timeoutTicks;

    while (1)
    {
        PRINTF("\r\n Select the wake up timeout in seconds.");
        PRINTF("\r\n The allowed range is 1s - 9s.");
        PRINTF("\r\n Eg. enter 5 to wake up in 5 seconds.");
        PRINTF("\r\n Waiting for input timeout value...");

        timeout = GETCHAR();
        if ((timeout > '0') && (timeout <= '9'))
        {
            timeout -= '0';
            PRINTF("\r\n Will wakeup in %d seconds.", timeout);
            timeoutTicks = (timeout * 1000000UL);
            return timeoutTicks;
        }
        PRINTF("\r\n Wrong value!");
    }
}

void APP_RegisterNotify(void)
{
    if (kStatus_PMSuccess != PM_RegisterNotify(kPM_NotifyGroup0, &g_notify0))
    {
        assert(false);
        PRINTF("\r\n Register notify0 failed");
    }

    if (kStatus_PMSuccess != PM_RegisterNotify(kPM_NotifyGroup1, &g_notify1))
    {
        assert(false);
        PRINTF("\r\n Register notify1 failed");
    }
}


void APP_SetConstraints(uint8_t powerMode)
{
    switch (powerMode)
    {
        case 0: /* sleep mode */
        {
            PM_SetConstraints(PM_LP_STATE_SLEEP, APP_SLEEP_CONSTRAINTS);
            break;
        }

        case 1: /* deep sleep mode */
        {
            PM_SetConstraints(PM_LP_STATE_DEEP_SLEEP, APP_DEEP_SLEEP_CONSTRAINTS);
            break;
        }

        case 2: /* power down mode */
        {
            PM_SetConstraints(PM_LP_STATE_POWER_DOWN, APP_POWER_DOWN_CONSTRAINTS);
            break;
        }

        case 3: /* deep power down mode */
        {
            PM_SetConstraints(PM_LP_STATE_DEEP_POWER_DOWN, APP_DEEP_POWER_DOWN_CONSTRAINTS);
            break;
        }

        default:
        {
            /* This branch will never be hit. */
            assert(false);
            break;
        }
    }
}

void APP_ReleaseConstraints(uint8_t powerMode)
{
    switch (powerMode)
    {
        case 0: /* sleep mode */
        {
            PM_ReleaseConstraints(PM_LP_STATE_SLEEP, APP_SLEEP_CONSTRAINTS);
            break;
        }
        case 1: /* deep sleep mode */
        {
            PM_ReleaseConstraints(PM_LP_STATE_DEEP_SLEEP, APP_DEEP_SLEEP_CONSTRAINTS);
            break;
        }
        case 2: /* power down mode */
        {
            PM_ReleaseConstraints(PM_LP_STATE_POWER_DOWN, APP_POWER_DOWN_CONSTRAINTS);
            break;
        }
        case 3: /* deep power down mode */
        {
            PM_ReleaseConstraints(PM_LP_STATE_DEEP_POWER_DOWN, APP_DEEP_POWER_DOWN_CONSTRAINTS);
            break;
        }
        default:
        {
            /* This branch should never be hit. */
            assert(false);
            break;
        }
    }
}

/*${function:end}*/
