/*
 * Copyright 2025-2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "app.h"
#include "board.h"
#include "fsl_clock.h"
#include "fsl_debug_console.h"
#include "fsl_mu.h"
#include "fsl_power.h"
#include "fsl_smm.h"

#include "fsl_gpio.h"
#include "fsl_lptmr.h"
#include "fsl_port.h"

#include "pin_mux.h"

#include "fsl_advc.h"
#include "fsl_rtc.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* AON core clock frequency used during DPD2 clock recovery. */
#define APP_DPD2_CLOCK_FREQ_HZ 3000000U

#if APP_ENABLE_ADVC
/* ADVC optimal-voltage-done status: bit 9 of the ADVC status register. */
#define APP_ADVC_STATUS_REG        (*(volatile uint32_t *)0xA0097C00U)
#define APP_ADVC_OPTIMAL_DONE_MASK (1U << 9U)
#endif

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void APP_EnableLptmrWakeup(void);
static bool APP_SecondaryCoreCallback(power_low_power_mode_t targetPowerMode, void *ptrPowerConfig, void *userData);
static void APP_DPD2ClockRecovery(void);
static void APP_DPD2WakeupLog(void);
static void APP_ActiveOps(void);
static void APP_DeepPowerDown1Ops(void);
static void APP_DPD1ToActive(void);
static void APP_DPD1ToDPD2BackToDPD1(void);
static void APP_DPD1ToDPD2BackToActive(void);
static void APP_ClearPendingIRQs(void);
static void APP_PrepareDPD2Entry(const char *wakeDestLabel);
static void APP_PrepareAonForLowPower(void);
static void APP_RestoreAonAfterLowPower(void);
/*******************************************************************************
 * Variables
 ******************************************************************************/
char *const g_modeNameArray[]  = APP_POWER_MODE_NAME;
volatile bool g_MuBRxIsrHit    = false;
volatile bool g_DualCoreSynced = false;
uint32_t g_MuBRxMsg            = 0UL;
/*******************************************************************************
 * Code
 ******************************************************************************/

void MU_B_RX_IRQHandler(void)
{
    uint32_t msg = MU_ReceiveMsgNonBlocking(APP_MU, APP_MU_CHANNEL);

    MU_ClearStatusFlags(APP_MU, kMU_Rx0FullFlag);
    if (msg == 0UL)
    {
        /* Spurious interrupt: FIFO was empty (e.g. triggered by CM33 reset on DPD1
         * wakeup). syncCode of any valid message is always 0x5A, so 0 is invalid. */
        __ISB();
        __DSB();
        return;
    }
    if (Power_GetMuMessageType(msg) == kPower_MsgTypeSync)
    {
        PRINTF("Syncing with CM33\r\n");
        if (Power_MuSyncCallback(msg, APP_MU_CHANNEL) == kStatus_Success)
        {
#if APP_ENABLE_ADVC
            PRINTF("Enabling ADVC As Optimal Mode...\r\n");
            if (ADVC_Enable(kADVC_ModeOptimal, NULL) != kADVC_Stat_Ok)
            {
                PRINTF("Fail to enable ADVC!!!\r\n");
                assert(false);
            }
            uint32_t tmp          = AON__PMU->AWK_UP_TIME;
            AON__PMU->AWK_UP_TIME = tmp;
            SDK_DelayAtLeastUs(600, CLOCK_GetAonCoreSysClkFreq());
#endif
            Power_RegisterUserCallback(APP_SecondaryCoreCallback, NULL);
            g_DualCoreSynced = true;
        }
        else
        {
            PRINTF("Fail to sync with CM33\r\n");
            assert(false);
        }
    }
    else
    {
        g_MuBRxMsg    = msg;
        g_MuBRxIsrHit = true;
    }
    __ISB();
    __DSB();
}

static bool APP_SecondaryCoreCallback(power_low_power_mode_t targetPowerMode, void *ptrPowerConfig, void *userData)
{
    PRINTF("Primary Core Request to enter: %s\r\n", g_modeNameArray[(uint32_t)targetPowerMode]);

    if (targetPowerMode == kPower_PowerDown2)
    {
        power_pd2_config_t pd2Config;
        memcpy(&pd2Config, ptrPowerConfig, sizeof(power_pd2_config_t));
        if (pd2Config.aonWakeupSource == kPower_WS_Aon_LptmrInt)
        {
            EnableIRQ(LPTMR_AON_IRQn);
        }
        else if (pd2Config.aonWakeupSource == kPower_WS_Aon_RtcAlarm0)
        {
            EnableIRQ(RTC_ALARM0_IRQn);
        }
        else if (pd2Config.aonWakeupSource == kPower_WS_Aon_RtcAlarm1)
        {
            EnableIRQ(RTC_ALARM1_IRQn);
        }
        else
        {
            PRINTF("Selected wakeup source do not supported!\r\n");
        }
        MU_DisableInterrupts(APP_MU, kMU_Rx0FullInterruptEnable);
    }

    if (targetPowerMode == kPower_DeepPowerDown2)
    {
        power_dpd2_config_t dpd2Config;
        memcpy(&dpd2Config, ptrPowerConfig, sizeof(power_dpd2_config_t));
        if (dpd2Config.wakeToDpd1 == true)
        {
            PRINTF("Press %s wakeup to Active Mode!\r\n", APP_EXT_INT_BUTTON);
        }
    }

    (void)ptrPowerConfig;
    (void)userData;
    return true;
}

void SMM_EXT_IRQHandler(void)
{
    SMM_ClearExternalIntFlag(AON__SMM);
}

void LPTMR_AON_IRQHandler(void)
{
    DisableIRQ(LPTMR_AON_IRQn);
    if (Power_GetPreviousPowerMode() != kPower_PowerDown2)
    {
        LPTMR_ClearStatusFlags(APP_LPTMR_BASE, kLPTMR_TimerCompareFlag);
        LPTMR_StopTimer(APP_LPTMR_BASE);
        LPTMR_DisableInterrupts(APP_LPTMR_BASE, kLPTMR_TimerInterruptEnable);
        CLOCK_DisableClock(kCLOCK_GateAonLPTMR);
        RESET_SetPeripheralReset(kAonLPTMR_RST_SHIFT_RSTn);
    }
    __DSB();
    __ISB();
}

void RTC_ALARM0_IRQHandler(void)
{
    DisableIRQ(RTC_ALARM0_IRQn);
    if (Power_GetPreviousPowerMode() != kPower_PowerDown2)
    {
        RTC_DisableInterrupts(APP_RTC_BASE, kRTC_Alarm0InterruptEnable);
        RTC_DisableAlarm(APP_RTC_BASE, kRTC_Alarm_0);
        RTC_StopTimer(APP_RTC_BASE);
        RTC_Deinit(APP_RTC_BASE);
    }
    __DSB();
    __ISB();
}

void RTC_ALARM1_IRQHandler(void)
{
    DisableIRQ(RTC_ALARM1_IRQn);
    if (Power_GetPreviousPowerMode() != kPower_PowerDown2)
    {
        RTC_DisableInterrupts(APP_RTC_BASE, kRTC_Alarm1InterruptEnable);
        RTC_DisableAlarm(APP_RTC_BASE, kRTC_Alarm_1);
        RTC_StopTimer(APP_RTC_BASE);
        RTC_Deinit(APP_RTC_BASE);
    }
    __DSB();
    __ISB();
}

static void APP_DPD2ClockRecovery(void)
{
    /* Stage 1: switch AON CPU clock back to FRO, let ADVC re-lock. */
#if APP_ENABLE_ADVC
    ADVC_PreVoltageChangeRequest(APP_DPD2_CLOCK_FREQ_HZ);
#endif
    AON__CGU->CLK_CONFIG |= CGU_CLK_CONFIG_ULPIRC_EN_MASK | CGU_CLK_CONFIG_LPIRC_EN_MASK;
    CLOCK_SetupFROAonClocking(APP_DPD2_CLOCK_FREQ_HZ);
    CLOCK_AttachClk(kFROdiv1_to_AON_CPU);
    AON__CGU->CLK_CONFIG &= ~CGU_CLK_CONFIG_ROOT_AUX_CLK_EN_MASK;
#if APP_ENABLE_ADVC
    ADVC_PostVoltageChangeRequest();

    /* Wait until ADVC has found the optimal voltage point. */
    while ((APP_ADVC_STATUS_REG & APP_ADVC_OPTIMAL_DONE_MASK) == 0UL)
    {
    }
#endif
}

static void APP_DPD2WakeupLog(void)
{
    APP_DPD2ClockRecovery();
    PRINTF("Wakeup from DPD2 with context saving enabled\r\n");
    PRINTF("Current AON Core Freq: %d\r\n", CLOCK_GetAonCoreSysClkFreq());
}

int main(void)
{
    power_low_power_mode_t curPowerMode = kPower_Active;
    power_low_power_mode_t prePowerMode = kPower_DeepPowerDown2;

    /* Start from POR. */
    BOARD_InitHardware();

    PRINTF(
        "\r\n###########################  Power Mode Switch Demo Secondary "
        "Core Boot  "
        "###########################\r\n");
    EnableIRQ(MU_B_RX_IRQn);
    MU_EnableInterrupts(APP_MU, kMU_Rx0FullInterruptEnable);

    /* Loop until dual core synced. */
    while (g_DualCoreSynced == false)
    {
    }
    g_DualCoreSynced = false;

#if (APP_ENABLE_ADVC && (!APP_ENABLE_CONTEXT_SAVING))
    /* When ADVC enabled but context saving disabled, wake up from DPD2 cause
      reset, invoke this function to clear pending IRQ. */
    APP_ClearPendingIRQs();
#endif /* APP_ENABLE_ADVC && (!APP_ENABLE_CONTEXT_SAVING) */
#if (APP_ENABLE_CONTEXT_SAVING == 0)
    Power_ClearLpPowerSettings();
#endif
    while (1)
    {
        if (Power_GetCurrentPowerMode(&curPowerMode) == kStatus_Success)
        {
            if (curPowerMode == kPower_Active)
            {
                PRINTF("System Is In Active Mode!\r\n");
                APP_ActiveOps();
            }
            else if (curPowerMode == kPower_DeepPowerDown1)
            {
                /* In DPD1 mode, waiting for input from AON_LPUART to do more
                 * operations. */
                PRINTF("System Is In Deep Power Down1 Mode!\r\n");
                APP_DeepPowerDown1Ops();
            }
            else
            {
                if (prePowerMode != curPowerMode)
                {
                    PRINTF("System Is In %s Mode!\r\n", g_modeNameArray[(uint8_t)curPowerMode]);
                }
                prePowerMode = curPowerMode;
            }
        }
    }
}

static void APP_ActiveOps(void)
{
    power_low_power_mode_t curLpMode;
    MU_EnableInterrupts(APP_MU, kMU_Rx0FullInterruptEnable);
    APP_ClearPendingIRQs();

    /* Enable MU_B_RX interrupt for dual core message. */
    EnableIRQ(MU_B_RX_IRQn);

    /* Loop until receive any request from CM33. */
    while (g_MuBRxIsrHit == false)
    {
        (void)Power_GetCurrentPowerMode(&curLpMode);
        if (curLpMode != kPower_Active)
        {
            return;
        }
    }

    PRINTF("Start Interpret Request\r\n");
    /* Interpret request message. */
    DisableIRQ(MU_B_RX_IRQn);
    uint32_t key = DisableGlobalIRQ();
    Power_InterpretRequest(g_MuBRxMsg);
    g_MuBRxMsg    = 0UL;
    g_MuBRxIsrHit = false;
    EnableGlobalIRQ(key);
    EnableIRQ(MU_B_RX_IRQn);
#if APP_ENABLE_CONTEXT_SAVING
    /* Wakeup with context saving enabled. */
    if (Power_GetPreviousPowerMode() == kPower_DeepPowerDown2)
    {
        APP_DPD2WakeupLog();

        power_low_power_mode_t curMode;
        Power_GetCurrentPowerMode(&curMode);

        if (curMode == kPower_DeepPowerDown1)
        {
            /* Wakeup from DPD2, current is in DPD1. */
            /* Clear pending interrupts, we just wakeup by interrupt. */
            PRINTF("DPD1 Mode\r\n");
            APP_ClearPendingIRQs();

            /* Enable SMM_EXT interrupt, allow wakeup from DPD1 to Active. */
            EnableIRQ(SMM_EXT_IRQn);
            AON__SMM->PWDN_CONFIG &= ~SMM_PWDN_CONFIG_Q_TMT_EN_MASK;
        }
        else
        {
            /* Wakeup from DPD2, current is in Active, notify CM33 to execute. */
            assert(curMode == kPower_Active);
            APP_ClearPendingIRQs();
            EnableIRQ(SMM_EXT_IRQn);
            AON__SMM->PWDN_CONFIG &= ~SMM_PWDN_CONFIG_Q_TMT_EN_MASK;
            Power_NotifyCM33ToRun();
        }
    }
#endif
}

static void APP_DeepPowerDown1Ops(void)
{
    if (Power_GetPreviousPowerMode() == kPower_DeepPowerDown2)
    {
        APP_PrepareAonForLowPower();
        __WFI();
        APP_RestoreAonAfterLowPower();
#if APP_ENABLE_CONTEXT_SAVING
        /* DPD1->Active complete.  CM33 is now spinning on MSB_BCKP1;
         * write the sync token so it can resume from Power_EnterDeepPowerDown1. */
        Power_NotifyCM33ToRun();
#endif
    }
    else
    {
        power_dpd1_transition_t nextTrans = Power_GetDeepPowerDown1NextTransition();
        MU_DisableInterrupts(APP_MU, kMU_Rx0FullInterruptEnable);
        DisableIRQ(MU_B_RX_IRQn);
        switch (nextTrans)
        {
            case kPower_Dpd1ToActive:
            {
                APP_DPD1ToActive();
                break;
            }
            case kPower_Dpd1ToDpd2WakeToDpd1:
            {
                APP_DPD1ToDPD2BackToDPD1();
                break;
            }
            case kPower_Dpd1ToDpd2WakeToActive:
            {
                APP_DPD1ToDPD2BackToActive();
                break;
            }
            default:
            {
                assert(false);
            }
        }
    }
}

/* Shut down AON peripherals that are not needed during DPD1 WFI to minimize
 * leakage current.  Must be called AFTER the last PRINTF and BEFORE __WFI. */
static void APP_PrepareAonForLowPower(void)
{
    /* Zero all AON PORT0 pin configs while PORT clock is still running,
     * preventing leakage through floating digital inputs. */
    for (uint8_t i = 0U; i < 27U; i++)
    {
        AON__PORT0->PCR[i] = 0UL;
    }

    /* DbgConsole_Deinit + UART reset + PORT/GPIO clock disable. */
    BOARD_DeinitDebugConsole();

    /* BOARD_DeinitDebugConsole does not gate the UART clock — do it here. */
    CLOCK_DisableClock(kCLOCK_GateAonUART);
}

/* Restore AON peripherals after DPD1 WFI wakeup so that PRINTF works again.
 * CM0+ does NOT reboot on DPD1->Active, so this is needed unconditionally. */
static void APP_RestoreAonAfterLowPower(void)
{
    CLOCK_EnableClock(kCLOCK_GateAonPORT);
    CLOCK_EnableClock(kCLOCK_GateAonGPIO);
    CLOCK_EnableClock(kCLOCK_GateAonUART);
    RESET_ReleasePeripheralReset(kAonUART_RST_SHIFT_RSTn);

    BOARD_InitDEBUG_UARTPins();
    BOARD_InitDebugConsole();
}

static void APP_DPD1ToActive(void)
{
    power_dpd1_config_t dpd1Config;
    Power_GetPowerModeConfig(&dpd1Config);
    if (dpd1Config.mainWakeupSource == kPower_WS_Main_LptmrInt)
    {
        EnableIRQ(LPTMR_AON_IRQn);
    }
    else if (dpd1Config.mainWakeupSource == kPower_WS_Main_RtcAlarm0)
    {
        EnableIRQ(RTC_ALARM0_IRQn);
    }
    else if (dpd1Config.mainWakeupSource == kPower_WS_Main_RtcAlarm1)
    {
        EnableIRQ(RTC_ALARM1_IRQn);
    }
    PRINTF("Start to execute WFI\r\n");

    APP_PrepareAonForLowPower();
    __WFI();
    APP_RestoreAonAfterLowPower();

#if APP_ENABLE_CONTEXT_SAVING
    /* Wakeup from DPD1, current is in Active, notify CM33 to execute. */
    Power_NotifyCM33ToRun();
#endif
}

/* Shared preamble for DPD1->DPD2 transitions: prompt user, clear SMM
 * sequences, configure LPTMR as the DPD2 wakeup source, then start it. */
static void APP_PrepareDPD2Entry(const char *wakeDestLabel)
{
    PRINTF("Please input any key!\r\n");
    GETCHAR();
    SMM_ClearAllLowPowerSequence(AON__SMM);
    PRINTF("LPTMR Used To Wake System From DPD2 to %s After 10s\r\n", wakeDestLabel);
    APP_EnableLptmrWakeup();
}

static void APP_DPD1ToDPD2BackToDPD1(void)
{
    APP_PrepareDPD2Entry("DPD1");
    /* Set configuration in DPD2 mode, want to back to DPD1 mode, so
    some Ram blocks in AON domain should be retained. */
    power_dpd2_config_t dpd2Config = {
        .aonRamArraysToRetain = kPower_AonDomainAllRams,
        .disableBandgap       = true,
#if (APP_ENABLE_CONTEXT_SAVING == 0)
        .mainRamArraysToRetain = 0U,
        .saveContext           = false,
        .enableIVSMode         = true,
#else
        .mainRamArraysToRetain = kPower_MainDomainAllRams,
        .saveContext           = true,
        .enableIVSMode         = false,
#endif
        .switchToX32K          = true,
        .disableFRO10M         = true,
        .disableFRO3M          = false,
        .wakeToDpd1            = true,
        .aonWakeupSource       = kPower_WS_Aon_LptmrInt,
        .mainWakeupSource      = kPower_WS_NONE,
        .dpd2VddCoreAonVoltage = kPower_VddCoreAon_601_5mV,
        .fro16KOutputFreq      = kPMU_FRO16KOutput16KHz,
    };
    if (Power_EnterDeepPowerDown2(&dpd2Config) == kStatus_Power_WakeupFromDPD2)
    {
        APP_DPD2WakeupLog();
        /* Enable LPTMR IRQn in NVIC to handle the pending LPTMR interrupt. */
        EnableIRQ(LPTMR_AON_IRQn);

        /* Enable SMM_EXT interrupt to support wakeup from DPD1 to Active. */
        EnableIRQ(SMM_EXT_IRQn);
        AON__SMM->PWDN_CONFIG &= ~SMM_PWDN_CONFIG_Q_TMT_EN_MASK;
    }
    else
    {
        PRINTF("Fail to enter DPD2!\r\n");
        assert(false);
    }
}

static void APP_DPD1ToDPD2BackToActive(void)
{
    APP_PrepareDPD2Entry("Active");

    /* Set configuration in DPD2 mode, want to back to active, no need to
    retain ram blocks. */
    power_dpd2_config_t dpd2Config = {
#if (APP_ENABLE_CONTEXT_SAVING == 0)
        .aonRamArraysToRetain  = kPower_AonDomainNoneRams,
        .mainRamArraysToRetain = kPower_MainDomainNoneRams,
        .saveContext           = false,
        .enableIVSMode         = true,
#else
        .aonRamArraysToRetain  = kPower_AonDomainAllRams,
        .mainRamArraysToRetain = kPower_MainDomainAllRams,
        .saveContext           = true,
        .enableIVSMode         = false,
#endif
        .disableBandgap        = true,
        .switchToX32K          = true,
        .disableFRO10M         = true,
        .disableFRO3M          = false,
        .wakeToDpd1            = false,
        .mainWakeupSource      = kPower_WS_Main_LptmrInt,
        .aonWakeupSource       = kPower_WS_Aon_LptmrInt,
        .dpd2VddCoreAonVoltage = kPower_VddCoreAon_601_5mV,
        .fro16KOutputFreq      = kPMU_FRO16KOutput16KHz,
    };
    if (Power_EnterDeepPowerDown2(&dpd2Config) == kStatus_Power_WakeupFromDPD2)
    {
        APP_DPD2WakeupLog();
        /* Enable LPTMR IRQn in NVIC to handle the pending LPTMR interrupt. */
        EnableIRQ(LPTMR_AON_IRQn);
#if APP_ENABLE_CONTEXT_SAVING
        /* Wakeup from DPD2, current is in Active, notify CM33 to execute. */
        Power_NotifyCM33ToRun();
#endif
    }
    else
    {
        PRINTF("Fail to enter DPD2!\r\n");
        assert(false);
    }
}

static void APP_EnableLptmrWakeup(void)
{
#if APP_ENABLE_ADVC
    if (ADVC_PreVoltageChangeRequest(APP_DPD2_CLOCK_FREQ_HZ) != kADVC_Stat_Ok)
    {
        PRINTF("Pre Voltage Change Request Fail\r\n");
        assert(false);
    }
#endif
    CLOCK_AttachClk(kFRO16K_to_AON_LPTMR);
    CLOCK_EnableClock(kCLOCK_GateAonLPTMR);
    RESET_ReleasePeripheralReset(kAonLPTMR_RST_SHIFT_RSTn);

    lptmr_config_t lptmrConfig;
    EnableIRQ(LPTMR_AON_IRQn);
    LPTMR_GetDefaultConfig(&lptmrConfig);
    lptmrConfig.prescalerClockSource = kLPTMR_PrescalerClock_1;
    LPTMR_Init(APP_LPTMR_BASE, &lptmrConfig);
    LPTMR_SetTimerPeriod(APP_LPTMR_BASE, USEC_TO_COUNT(LPTMR_USEC_COUNT, LPTMR_SOURCE_CLOCK));
    LPTMR_EnableInterrupts(APP_LPTMR_BASE, kLPTMR_TimerInterruptEnable);
    LPTMR_StartTimer(APP_LPTMR_BASE);

#if APP_ENABLE_ADVC
    if (ADVC_PostVoltageChangeRequest() != kADVC_Stat_Ok)
    {
        PRINTF("Post Voltage Change Request Fail\r\n");
        assert(false);
    }
#endif
}

static void APP_ClearPendingIRQs(void)
{
    if (NVIC_GetPendingIRQ(LPTMR_AON_IRQn) != 0U)
    {
        EnableIRQ(LPTMR_AON_IRQn);
    }
    if (NVIC_GetPendingIRQ(RTC_ALARM0_IRQn) != 0U)
    {
        EnableIRQ(RTC_ALARM0_IRQn);
    }
    if (NVIC_GetPendingIRQ(RTC_ALARM1_IRQn) != 0U)
    {
        EnableIRQ(RTC_ALARM1_IRQn);
    }
}
