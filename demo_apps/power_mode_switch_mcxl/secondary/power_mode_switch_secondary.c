/*
 * Copyright 2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_clock.h"
#include "fsl_power.h"
#include "fsl_mu.h"
#include "fsl_smm.h"

#include "fsl_lptmr.h"
#include "fsl_gpio.h"
#include "fsl_port.h"

#include "pin_mux.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void APP_EnableLptmrWakeup(void);
static bool APP_SecondaryCoreCallback(power_low_power_mode_t targetPowerMode, void *ptrPowerConfig, void *userData);
static void APP_DeepPowerDown1Ops(bool wakeFromDpd2);
static void APP_WakeupFromPD1(void);
/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile bool tamperButtonTriggered = false;
char *const g_modeNameArray[]       = APP_POWER_MODE_NAME;
volatile bool g_MuBRxIsrHit         = false;
uint32_t g_MuBRxMsg                 = 0UL;
/*******************************************************************************
 * Code
 ******************************************************************************/

void MU_B_RX_IRQHandler(void)
{
    uint32_t msg = MU_ReceiveMsgNonBlocking(APP_MU, APP_MU_CHANNEL);

    MU_ClearStatusFlags(APP_MU, kMU_Rx0FullFlag);
    PRINTF("MU B RX Hit with message:%x \r\n", msg);
    if (Power_GetMuMessageType(msg) == kPower_MsgTypeSync)
    {
        Power_MuSyncCallback(msg, APP_MU_CHANNEL);
    }
    else
    {
        g_MuBRxMsg    = msg;
        g_MuBRxIsrHit = true;
    }
    __ISB();
    __DSB();
}

void SMM_EXT_IRQHandler(void)
{
    SMM_ClearExternalIntFlag(AON__SMM);
}

void LPTMR_AON_IRQHandler(void)
{
    Power_ClearLpPowerSettings();
    DisableIRQ(LPTMR_AON_IRQn);
    LPTMR_ClearStatusFlags(APP_LPTMR_BASE, kLPTMR_TimerCompareFlag);
    LPTMR_StopTimer(APP_LPTMR_BASE);
    LPTMR_DisableInterrupts(APP_LPTMR_BASE, kLPTMR_TimerInterruptEnable);
    if ((AON__SMM->WKUP_STAT & (1UL << 13UL)) != 0UL)
    {
        (void)(AON__SMM->WKUP_STAT);
        AON__SMM->STAT |= (SMM_STAT_DPD_END_MASK | SMM_STAT_DPD_SEQ_END_MASK);
    }
    CLOCK_DisableClock(kCLOCK_GateAonLPTMR);
    RESET_SetPeripheralReset(kAonLPTMR_RST_SHIFT_RSTn);
    __DSB();
    __ISB();
    PRINTF("TMR\r\n");
}

void RTC_ALARM0_IRQHandler(void)
{
    Power_ClearLpPowerSettings();
    DisableIRQ(RTC_ALARM0_IRQn);
    AON__RTC_AON->CONFIG |= RTC_CONFIG_ALARM0_DIS_MASK;
    AON__RTC_AON->CONFIG &= ~RTC_CONFIG_EN_MASK;
    AON__RTC_AON->INT = RTC_INT_ALARM0_IF_MASK;
    AON__RTC_AON->INT &= ~RTC_INT_ALARM0_IE_MASK;
    __DSB();
    __ISB();
}

void RTC_ALARM1_IRQHandler(void)
{
    Power_ClearLpPowerSettings();
    DisableIRQ(RTC_ALARM1_IRQn);
    AON__RTC_AON->CONFIG |= RTC_CONFIG_ALARM1_DIS_MASK;
    AON__RTC_AON->CONFIG &= ~RTC_CONFIG_EN_MASK;
    AON__RTC_AON->INT = RTC_INT_ALARM1_IF_MASK;
    AON__RTC_AON->INT &= ~RTC_INT_ALARM1_IE_MASK;
    __DSB();
    __ISB();
}

int main(void)
{
    smm_backup_reg_content_t backupReg;

    SMM_ReadFromBackupReg(AON__SMM, &backupReg);
    power_low_power_mode_t curPowerMode = kPower_Active;
    power_low_power_mode_t prePowerMode = kPower_DeepPowerDown2;
    bool wakeupFromDpd2                 = false;

    if ((backupReg.word1 & 0xFFFFUL) != 0x5A5AUL)
    {
        /* Start from POR. */
        BOARD_InitHardware();
        PRINTF(
            "\r\n###########################  Power Mode Switch Demo Secondary Core Boot  "
            "###########################\r\n");
        EnableIRQ(MU_B_RX_IRQn);
        MU_EnableInterrupts(APP_MU, kMU_Rx0FullInterruptEnable);
        Power_RegisterUserCallback(APP_SecondaryCoreCallback, NULL);
    }
    else
    {
        if (((backupReg.word1 & 0xFFFF0000UL) >> 16UL) == (uint32_t)kPower_DeepPowerDown2)
        {
            /* Wakeup from DPD2, current mode is DPD1. */
            BOARD_InitHardware();
            PRINTF("Wakeup from DPD2.\r\n");
            Power_RestoreHandleOffset(backupReg.word2);
            EnableIRQ(MU_B_RX_IRQn);
            /* Enable transmit and receive interrupt */
            MU_EnableInterrupts(APP_MU, kMU_Rx0FullInterruptEnable);
            if (NVIC_GetPendingIRQ(LPTMR_AON_IRQn) != 0U)
            {
                EnableIRQ(LPTMR_AON_IRQn);
                NVIC_ClearPendingIRQ(LPTMR_AON_IRQn);
            }
            wakeupFromDpd2 = true;
        }
    }
    while (1)
    {
        if ((AON__CGU->CLK_CONFIG & CGU_CLK_CONFIG_FRO10M_EN_MASK) != 0UL)
        {
            AON__CGU->CLK_CONFIG &= ~CGU_CLK_CONFIG_FRO10M_EN_MASK;
        }
        if (g_MuBRxIsrHit == true)
        {
            Power_InterpretRequest(g_MuBRxMsg);
            g_MuBRxMsg    = 0UL;
            g_MuBRxIsrHit = false;
        }
        if (Power_GetCurrentPowerMode(&curPowerMode) == kStatus_Success)
        {
            if (curPowerMode != prePowerMode)
            {
                if (prePowerMode == kPower_PowerDown1)
                {
                    PRINTF("Wakeup From PD1\r\n");
                    APP_WakeupFromPD1();
                }
                PRINTF("System Is In %s Mode!\r\n", g_modeNameArray[(uint8_t)curPowerMode]);
                prePowerMode = curPowerMode;
            }
            if (curPowerMode == kPower_Active)
            {
                MU_EnableInterrupts(APP_MU, kMU_Rx0FullInterruptEnable);
            }
            if (curPowerMode == kPower_DeepPowerDown1)
            {
                APP_DeepPowerDown1Ops(wakeupFromDpd2);
            }
        }
        else
        {
            PRINTF("System Is In %s Mode, Target Power Mode Is %s Mode!!!\r\n", g_modeNameArray[(uint8_t)curPowerMode],
                   g_modeNameArray[(uint8_t)Power_GetTargetPowerMode()]);
            PRINTF("Running with wrong mode!!!\r\n");
            return -1;
        }
    }
}

static bool APP_SecondaryCoreCallback(power_low_power_mode_t targetPowerMode, void *ptrPowerConfig, void *userData)
{
    PRINTF("Primary Core Request to enter: %s\r\n", g_modeNameArray[(uint32_t)targetPowerMode]);
    if (targetPowerMode == kPower_DeepPowerDown2)
    {
        if (((power_dpd2_config_t *)ptrPowerConfig)->wakeToDpd1 == true)
        {
            uint32_t off = Power_GetHandleOffset();

            smm_backup_reg_content_t backupReg;
            backupReg.word1 = 0x5A5AUL | (((uint32_t)kPower_DeepPowerDown2 << 16U));
            backupReg.word2 = off;
            SMM_WriteToBackupReg(AON__SMM, &backupReg);
        }
    }

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

    (void)ptrPowerConfig;
    (void)userData;
    return true;
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

static void APP_WakeupFromPD1(void)
{
    if (NVIC_GetPendingIRQ(LPTMR_AON_IRQn) != 0U)
    {
        NVIC_ClearPendingIRQ(LPTMR_AON_IRQn);
    }
    if (NVIC_GetPendingIRQ(RTC_ALARM0_IRQn) != 0U)
    {
        NVIC_ClearPendingIRQ(RTC_ALARM0_IRQn);
    }
    if (NVIC_GetPendingIRQ(RTC_ALARM1_IRQn) != 0U)
    {
        NVIC_ClearPendingIRQ(RTC_ALARM1_IRQn);
    }
}

static void APP_DeepPowerDown1Ops(bool wakeFromDpd2)
{
    uint32_t off = Power_GetHandleOffset();
    smm_backup_reg_content_t backupReg;
    backupReg.word1 = 0x5A5A | ((uint32_t)kPower_DeepPowerDown1 << 16U);
    backupReg.word2 = off;
    SMM_WriteToBackupReg(AON__SMM, &backupReg);
    power_dpd1_transition_t nextTrans = Power_GetDeepPowerDown1NextTransition();
    if (nextTrans == kPower_Dpd1ToDpd2WakeToDpd1)
    {
        PRINTF("Please input any key!\r\n");
        GETCHAR();
        SMM_ClearAllLowPowerSequence(AON__SMM);
        PRINTF("LPTMR Used To Wake System From DPD2 to DPD1 After 10s\r\n");
        backupReg.word1 = 0x5A5AUL | (((uint32_t)kPower_DeepPowerDown2 << 16U));
        backupReg.word2 = off;
        SMM_WriteToBackupReg(AON__SMM, &backupReg);

        APP_EnableLptmrWakeup();
        power_dpd2_config_t dpd2Config = {
            .aonRamArraysToRetain  = kPower_AonDomainAllRams,
            .disableBandgap        = true,
            .mainRamArraysToRetain = 0U,
            .enableIVSMode         = true,
            .switchToX32K          = true,
            .disableFRO10M         = false,
            .wakeToDpd1            = true,
            .aonWakeupSource       = kPower_WS_Aon_LptmrInt,
            .mainWakeupSource      = kPower_WS_NONE,
        };
        if (Power_EnterDeepPowerDown2(&dpd2Config) == kStatus_Success)
        {
            PRINTF("Wakeup From DPD2.\r\n");
        }
    }
    else if (nextTrans == kPower_Dpd1ToDpd2WakeToActive)
    {
        PRINTF("Please input any key!\r\n");
        GETCHAR();
        SMM_ClearAllLowPowerSequence(AON__SMM);
        PRINTF("LPTMR Used To Wake System From DPD2 to Active After 10s\r\n");
        APP_EnableLptmrWakeup();
        power_dpd2_config_t dpd2Config = {
            .aonRamArraysToRetain  = kPower_AonDomainNoneRams,
            .disableBandgap        = true,
            .mainRamArraysToRetain = 0U,
            .enableIVSMode         = false,
            .switchToX32K          = true,
            .disableFRO10M         = false,
            .wakeToDpd1            = false,
            .mainWakeupSource      = kPower_WS_Main_LptmrInt,
            .aonWakeupSource       = kPower_WS_Aon_LptmrInt,
        };
        uint32_t off = Power_GetHandleOffset();

        backupReg.word1 = 0x5A5AUL | (((uint32_t)kPower_DeepPowerDown2 << 16U));
        backupReg.word2 = off;
        SMM_WriteToBackupReg(AON__SMM, &backupReg);
        if (Power_EnterDeepPowerDown2(&dpd2Config) == kStatus_Success)
        {
            PRINTF("Wakeup From DPD2.\r\n");
        }
    }
    else
    {
        if (wakeFromDpd2 == false)
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
        }
        PRINTF("Start to execute WFI\r\n");
        /* In DPD1, execute WFI to get lower power number. */
        __WFI();
    }
}
