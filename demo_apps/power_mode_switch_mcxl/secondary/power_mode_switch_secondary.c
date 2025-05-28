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
#if APP_ENABLE_DEBUG_LOG
#define APP_DEBUG_LOG PRINTF
#else
static inline int LOG_Disabled(void)
{
    return -1;
}
#define APP_DEBUG_LOG(...) LOG_Disabled()
#endif

static void APP_EnableLptmrWakeup(void);
#if 0
static void APP_ConfigTamperButton(void);
#endif
static bool APP_SecondaryCoreCallback(power_low_power_mode_t targetPowerMode, void *ptrPowerConfig, void *userData);
/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile bool tamperButtonTriggered = false;
char *const g_modeNameArray[]       = APP_POWER_MODE_NAME;
/*******************************************************************************
 * Code
 ******************************************************************************/

void MU_B_RX_IRQHandler(void)
{
    uint32_t msg = MU_ReceiveMsgNonBlocking(APP_MU, APP_MU_CHANNEL);
    MU_ClearStatusFlags(APP_MU, kMU_Rx0FullFlag);
    APP_DEBUG_LOG("MU B RX Hit with message:%x \r\n", msg);
    Power_MuMessageCallback(msg, APP_MU_CHANNEL);
}

void SMM_EXT_IRQHandler(void)
{
    APP_DEBUG_LOG("EXT_INT Occur!\r\n");
    SMM_ClearExternalIntFlag(AON__SMM);
    SMM_WakeupMainDomain(AON__SMM);
    APP_DEBUG_LOG("Finish Wakeup Main Domain!\r\n");
}

void GPIO00_AON_IRQHandler(void)
{
    PRINTF("TAMPER Button Pressed\r\n");
    GPIO_GpioClearInterruptFlags(APP_TAMPER_BUTTON_GPIO, 1UL << APP_TAMPER_BUTTON_PIN);
    BOARD_DeinitTamper_ButtonPins();
    tamperButtonTriggered = true;
}

void LPTMR_AON_IRQHandler(void)
{
    APP_DEBUG_LOG("LPTMR Occur!\r\n");
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
}

int main(void)
{
    smm_backup_reg_content_t backupReg;

    SMM_ReadFromBackupReg(AON__SMM, &backupReg);
    power_low_power_mode_t curPowerMode = kPower_Active;
    power_low_power_mode_t prePowerMode = kPower_DeepPowerDown2;
    uint32_t off                        = 0UL;

    if ((backupReg.word1 & 0xFFFFUL) != 0x5A5AUL)
    {
        /* Start from POR. */
        BOARD_InitHardware();
        EnableIRQ(MU_B_RX_IRQn);
        MU_EnableInterrupts(APP_MU, kMU_Rx0FullInterruptEnable);
        APP_DEBUG_LOG(
            "\r\n###########################  Power Mode Switch Demo Secondary Core Boot  "
            "###########################\r\n");
        Power_RegisterUserCallback(APP_SecondaryCoreCallback, NULL);
        APP_DEBUG_LOG("Finish register callback\r\n");
    }
    else
    {
        if (((backupReg.word1 & 0xFFFF0000UL) >> 16UL) == (uint32_t)kPower_DeepPowerDown2)
        {
            /* Wakeup from DPD2, current mode is DPD1. */
            BOARD_InitHardware();
            APP_DEBUG_LOG("Wakeup from DPD2.\r\n");
            Power_RestoreHandleOffset(backupReg.word2);
            EnableIRQ(MU_B_RX_IRQn);
            /* Enable transmit and receive interrupt */
            MU_EnableInterrupts(APP_MU, kMU_Rx0FullInterruptEnable);
            if (NVIC_GetPendingIRQ(LPTMR_AON_IRQn) != 0U)
            {
                EnableIRQ(LPTMR_AON_IRQn);
                NVIC_ClearPendingIRQ(LPTMR_AON_IRQn);
            }
        }
    }
    while (1)
    {
        if ((AON__CGU->CLK_CONFIG & CGU_CLK_CONFIG_FRO10M_EN_MASK) != 0UL)
        {
            AON__CGU->CLK_CONFIG &= ~CGU_CLK_CONFIG_FRO10M_EN_MASK;
        }

        if (Power_GetCurrentPowerMode(&curPowerMode) == kStatus_Success)
        {
            if (curPowerMode != prePowerMode)
            {
                APP_DEBUG_LOG("System Is In %s Mode!\r\n", g_modeNameArray[(uint8_t)curPowerMode]);
                prePowerMode = curPowerMode;
            }

            if (curPowerMode == kPower_DeepPowerDown1)
            {
                off             = Power_GetHandleOffset();
                backupReg.word1 = 0x5A5A | ((uint32_t)kPower_DeepPowerDown1 << 16U);
                backupReg.word2 = off;
                SMM_WriteToBackupReg(AON__SMM, &backupReg);
#if 0
                  APP_DEBUG_LOG("Press %s enter Deep Power Down2 Mode...\r\n", APP_TAMPER_BUTTON_NAME);
                  APP_ConfigTamperButton();
                  while(tamperButtonTriggered == false)
                  {}
                  tamperButtonTriggered = false;
#endif
                power_dpd1_transition_t nextTrans = Power_GetDeepPowerDown1NextTransition();
                if (nextTrans == kPower_Dpd1ToDpd2WakeToDpd1)
                {
                    APP_DEBUG_LOG("Please input any key!\r\n");
                    GETCHAR();
                    AON__SMM->PWDN_CONFIG &= ~SMM_PWDN_CONFIG_DPD_STRT_MASK;
                    APP_DEBUG_LOG("LPTMR Used To Wake System From DPD2 to DPD1 After 10s\r\n");
                    APP_EnableLptmrWakeup();
                    Power_EnableWakeupSource(kPower_WS_Aon_LptmrInt);
                    power_dpd2_config_t dpd2Config = {
                        .aonRamArraysToRetain  = kPower_AonDomainAllRams,
                        .disableBandgap        = true,
                        .mainRamArraysToRetain = 0U,
                        .enableIVSMode         = true,
                        .switchToX32K          = true,
                        .disableFRO10M         = false,
                        .wakeToDpd1            = true,
                    };

                    backupReg.word1 = 0x5A5AUL | (((uint32_t)kPower_DeepPowerDown2 << 16U));
                    backupReg.word2 = off;
                    SMM_WriteToBackupReg(AON__SMM, &backupReg);
                    if (Power_EnterDeepPowerDown2(&dpd2Config) == kStatus_Success)
                    {
                        APP_DEBUG_LOG("Wakeup From DPD2.\r\n");
                    }
                }
                else if (nextTrans == kPower_Dpd1ToDpd2WakeToActive)
                {
                    APP_DEBUG_LOG("Please input any key!\r\n");
                    GETCHAR();
                    AON__SMM->PWDN_CONFIG &= ~SMM_PWDN_CONFIG_DPD_STRT_MASK;
                    APP_DEBUG_LOG("LPTMR Used To Wake System From DPD2 to Active After 10s\r\n");
                    APP_EnableLptmrWakeup();
                    Power_EnableWakeupSource(kPower_WS_Both_LptmrInt);
                    power_dpd2_config_t dpd2Config = {
                        .aonRamArraysToRetain  = kPower_AonDomainNoneRams,
                        .disableBandgap        = true,
                        .mainRamArraysToRetain = 0U,
                        .enableIVSMode         = false,
                        .switchToX32K          = true,
                        .disableFRO10M         = false,
                        .wakeToDpd1            = false,
                    };
                    uint32_t off = Power_GetHandleOffset();

                    backupReg.word1 = 0x5A5AUL | (((uint32_t)kPower_DeepPowerDown2 << 16U));
                    backupReg.word2 = off;
                    SMM_WriteToBackupReg(AON__SMM, &backupReg);
                    if (Power_EnterDeepPowerDown2(&dpd2Config) == kStatus_Success)
                    {
                        APP_DEBUG_LOG("Wakeup From DPD2.\r\n");
                    }
                }
                else
                {
                    APP_DEBUG_LOG("Start to execute WFI\r\n");
                    __WFI();
                }
            }
        }
        else
        {
            APP_DEBUG_LOG("System Is In %s Mode, Target Power Mode Is %s Mode!!!\r\n",
                          g_modeNameArray[(uint8_t)curPowerMode], g_modeNameArray[(uint8_t)Power_GetTargetPowerMode()]);
            APP_DEBUG_LOG("Running with wrong mode!!!\r\n");
            return -1;
        }
    }
}

static bool APP_SecondaryCoreCallback(power_low_power_mode_t targetPowerMode, void *ptrPowerConfig, void *userData)
{
    APP_DEBUG_LOG("Primary Core Request to enter: %s\r\n", g_modeNameArray[(uint32_t)targetPowerMode]);
    if (targetPowerMode == kPower_DeepPowerDown2)
    {
        if (((power_dpd2_config_t *)ptrPowerConfig)->wakeToDpd1 == true)
        {
            uint32_t off = Power_GetHandleOffset();

            smm_backup_reg_content_t backupReg;
            backupReg.word1 = 0x5A5AUL | (((uint32_t)kPower_DeepPowerDown2 << 16U));
            backupReg.word2 = off;
            SMM_WriteToBackupReg(AON__SMM, &backupReg);
            Power_EnableWakeupSource(kPower_WS_Main_ExternalINTRiseEdge);
        }
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
    LPTMR_GetDefaultConfig(&lptmrConfig);
    lptmrConfig.prescalerClockSource = kLPTMR_PrescalerClock_1;
    LPTMR_Init(APP_LPTMR_BASE, &lptmrConfig);
    LPTMR_SetTimerPeriod(APP_LPTMR_BASE, USEC_TO_COUNT(LPTMR_USEC_COUNT, LPTMR_SOURCE_CLOCK));
    LPTMR_EnableInterrupts(APP_LPTMR_BASE, kLPTMR_TimerInterruptEnable);
    LPTMR_StartTimer(APP_LPTMR_BASE);
}

#if 0
static void APP_ConfigTamperButton(void)
{
    BOARD_InitTamper_ButtonPins();
    gpio_pin_config_t config = {
        .pinDirection = kGPIO_DigitalInput,
        .outputLogic  = 0,
    };

    GPIO_PinInit(APP_TAMPER_BUTTON_GPIO, APP_TAMPER_BUTTON_PIN, &config);
    GPIO_SetPinInterruptConfig(APP_TAMPER_BUTTON_GPIO, APP_TAMPER_BUTTON_PIN, kGPIO_InterruptFallingEdge);
    EnableIRQ(APP_TAMPER_BUTTON_IRQ);
    GPIO_EnableInterruptControlNonPrivilege(APP_TAMPER_BUTTON_GPIO, 1UL << 4UL);
}
#endif