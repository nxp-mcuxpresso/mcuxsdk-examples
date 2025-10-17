/*
 * Copyright 2025 NXP
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

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void APP_EnableLptmrWakeup(void);
static bool APP_SecondaryCoreCallback(power_low_power_mode_t targetPowerMode, void *ptrPowerConfig, void *userData);
static void APP_ActiveOps(void);
static void APP_DeepPowerDown1Ops(void);
static void APP_DPD1ToActive(void);
static void APP_DPD1ToDPD2BackToDPD1(void);
static void APP_DPD1ToDPD2BackToActive(void);
static void APP_ClearPendingIRQs(void);
static void APP_WakeupFromDPD2(void);
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
    if (Power_GetMuMessageType(msg) == kPower_MsgTypeSync)
    {
        PRINTF("Syncing with CM33\r\n");
        if (Power_MuSyncCallback(msg, APP_MU_CHANNEL) == kStatus_Success)
        {
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
    Power_ClearLpPowerSettings();
}

void LPTMR_AON_IRQHandler(void)
{
    Power_ClearLpPowerSettings();
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
    Power_ClearLpPowerSettings();
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
    Power_ClearLpPowerSettings();
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
    /* Loop until recevice any request from CM33. */
    while (g_MuBRxIsrHit == false)
    {
        (void)Power_GetCurrentPowerMode(&curLpMode);
        if (curLpMode != kPower_Active)
        {
            return;
        }
    }

    /* Interpret request message. */
    Power_InterpretRequest(g_MuBRxMsg);
    g_MuBRxMsg    = 0UL;
    g_MuBRxIsrHit = false;
#if APP_ENABLE_CONTEXT_SAVING
    if (Power_GetPreviousPowerMode() == kPower_DeepPowerDown2)
    {
        PRINTF("Wakeup from DPD2 with context saving enabled\r\n");
        APP_WakeupFromDPD2();
        BOARD_InitHardware();
    }
#endif
}

static void APP_DeepPowerDown1Ops(void)
{
    if (Power_GetPreviousPowerMode() == kPower_DeepPowerDown2)
    {
        /* Wakeup from DPD2, now system is in DPD1. */
        __WFI();
    }
    else
    {
        power_dpd1_transition_t nextTrans = Power_GetDeepPowerDown1NextTransition();
        MU_DisableInterrupts(APP_MU, kMU_Rx0FullInterruptEnable);
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
    /* In DPD1, execute WFI to get lower power number. */
    __WFI();
}

static void APP_DPD1ToDPD2BackToDPD1(void)
{
    PRINTF("Please input any key!\r\n");
    GETCHAR();
    SMM_ClearAllLowPowerSequence(AON__SMM);
    PRINTF("LPTMR Used To Wake System From DPD2 to DPD1 After 10s\r\n");
    APP_EnableLptmrWakeup();

    /* Set configuration in DPD2 mode, want to back to DPD1 mode, so
    some Ram blocks in AON domain should be retained. */
    power_dpd2_config_t dpd2Config = {
        .aonRamArraysToRetain = kPower_AonDomainAllRams,
        .disableBandgap       = true,
#if (APP_ENABLE_CONTEXT_SAVING == 0)
        .mainRamArraysToRetain = 0U,
#else
        .mainRamArraysToRetain = kPower_MainDomainAllRams,
        .saveContext           = true,
#endif
        .enableIVSMode         = false,
        .switchToX32K          = true,
        .disableFRO10M         = false,
        .disableFRO2M          = false,
        .wakeToDpd1            = true,
        .aonWakeupSource       = kPower_WS_Aon_LptmrInt,
        .mainWakeupSource      = kPower_WS_NONE,
        .dpd2VddCoreAonVoltage = kPower_VddCoreAon_592mV,
    };
    if (Power_EnterDeepPowerDown2(&dpd2Config) == kStatus_Power_WakeupFromDPD2)
    {
        APP_WakeupFromDPD2();
#if APP_ENABLE_ADVC
        if (ADVC_IsEnabled() == false)
        {
            PRINTF("Re-enable ADVC\r\n");
            ADVC_Enable(kADVC_ModeOptimal, NULL);
        }
#endif
    }
    else
    {
        PRINTF("Fail to enter DPD2!\r\n");
        assert(false);
    }
}

static void APP_DPD1ToDPD2BackToActive(void)
{
    PRINTF("Please input any key!\r\n");
    GETCHAR();
    SMM_ClearAllLowPowerSequence(AON__SMM);
    PRINTF("LPTMR Used To Wake System From DPD2 to Active After 10s\r\n");
    APP_EnableLptmrWakeup();

    /* Set configuration in DPD2 mode, want to back to active, no need to
    retain ram blocks. */
    power_dpd2_config_t dpd2Config = {
#if (APP_ENABLE_CONTEXT_SAVING == 0)
        .aonRamArraysToRetain  = kPower_AonDomainNoneRams,
        .mainRamArraysToRetain = kPower_MainDomainNoneRams,
#else
        .aonRamArraysToRetain  = kPower_AonDomainAllRams,
        .mainRamArraysToRetain = kPower_MainDomainAllRams,
        .saveContext           = true,
#endif
        .enableIVSMode         = false,
        .disableBandgap        = true,
        .switchToX32K          = true,
        .disableFRO10M         = false,
        .disableFRO2M          = false,
        .wakeToDpd1            = false,
        .mainWakeupSource      = kPower_WS_Main_LptmrInt,
        .aonWakeupSource       = kPower_WS_Aon_LptmrInt,
        .dpd2VddCoreAonVoltage = kPower_VddCoreAon_592mV,
    };
    if (Power_EnterDeepPowerDown2(&dpd2Config) == kStatus_Power_WakeupFromDPD2)
    {
        APP_WakeupFromDPD2();
    }
    else
    {
        PRINTF("Fail to enter DPD2!\r\n");
        assert(false);
    }
}

static void APP_EnableLptmrWakeup(void)
{
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

static void APP_WakeupFromDPD2(void)
{
    Power_ClearLpPowerSettings();
    EnableIRQ(SMM_EXT_IRQn);
    EnableIRQ(MU_B_RX_IRQn);
    /* Enable transmit and receive interrupt */
    if (NVIC_GetPendingIRQ(LPTMR_AON_IRQn) != 0U)
    {
        EnableIRQ(LPTMR_AON_IRQn);
        NVIC_ClearPendingIRQ(LPTMR_AON_IRQn);
    }
}
