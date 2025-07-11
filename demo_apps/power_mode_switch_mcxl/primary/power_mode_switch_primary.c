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
#include "power_mode_switch_primary.h"
#include "fsl_mu.h"
#include "fsl_wuu.h"
#include "fsl_lptmr.h"
#include "fsl_cmc.h"
#include "fsl_smm.h"
#include "fsl_advc.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void APP_CopyCore1Image(void);
static uint32_t DEMO_GetTargetPowerTransition(void);
static power_low_power_mode_t DEMO_EnableWakeupSource(uint32_t powerTrans);
static void APP_EnableExtInternalWakeup(uint8_t firstMode, uint8_t secondMode, uint8_t thirdMode);
static void APP_EnableLptmrWakeup(uint8_t firstMode, uint8_t secondMode, uint8_t thirdMode);
static void APP_EnableRTCAlarm0Wakeup(uint8_t firstMode, uint8_t secondMode, uint8_t thirdMode);
static void APP_EnableRTCAlarm1Wakeup(uint8_t firstMode, uint8_t secondMode, uint8_t thirdMode);
static bool APP_GetWakeupReason(void);
static void APP_ClearPlsRAM(void);
/*******************************************************************************
 * Variables
 ******************************************************************************/
char *const g_modeDescArray[] = DEMO_POWER_MODE_DESC;

#if defined(__ICCARM__) /* IAR Workbench */
#pragma location = "rpmsg_sh_mem_section"
power_handle_t powerHandle;
#elif defined(__CC_ARM) || defined(__ARMCC_VERSION) /* Keil MDK */
power_handle_t powerHandle __attribute__((section("rpmsg_sh_mem_section")));
#elif defined(__GNUC__)
power_handle_t powerHandle __attribute__((section(".noinit.$rpmsg_sh_mem")));
#else
#error "RPMsg: Please provide your definition of rpmsg_lite_base[]!"
#endif

power_pd1_config_t pd1Config = {
    .mainWakeupSource      = kPower_WS_NONE,
    .mainRamArraysToRetain = kPower_MainDomainAllRams,
    .enableIVSMode         = false,
};

power_pd2_config_t pd2Config = {
    .mainWakeupSource      = kPower_WS_NONE,
    .aonWakeupSource       = kPower_WS_NONE,
    .aonRamArraysToRetain  = kPower_AonDomainAllRams,
    .mainRamArraysToRetain = kPower_MainDomainAllRams,
    .enableIVSMode         = false,
    .disableFRO10M         = false,
};

power_dpd1_config_t dpd1Config = {
    .mainWakeupSource      = kPower_WS_NONE,
    .mainRamArraysToRetain = kPower_MainDomainNoneRams,
    .disableBandgap        = true,
    .enableIVSMode         = false,
};

power_dpd2_config_t dpd2Config = {
    .mainWakeupSource      = kPower_WS_NONE,
    .aonWakeupSource       = kPower_WS_NONE,
    .aonRamArraysToRetain  = kPower_AonDomainNoneRams,
    .mainRamArraysToRetain = kPower_MainDomainNoneRams,
    .disableBandgap        = true,
    .enableIVSMode         = false,
    .switchToX32K          = true,
    .disableFRO10M         = false,
};

power_dpd3_config_t dpd3Config = {
    .wakeupSource = kPower_WS_NONE,
};

power_sd_config_t sdConfig = {
    .wakeupSource = kPower_WS_NONE,
};

void *powerConfigs[8U] = {
    NULL,
    NULL,
    (void *)&pd1Config,
    (void *)&pd2Config,
    (void *)&dpd1Config,
    (void *)&dpd2Config,
    (void *)&dpd3Config,
    (void *)&sdConfig,
};
/*******************************************************************************
 * Code
 ******************************************************************************/
void MU_A_RX_IRQHandler(void)
{
    uint32_t msg = MU_ReceiveMsg(APP_MU, APP_MU_CHANNEL);
    MU_ClearStatusFlags(APP_MU, kMU_Rx0FullFlag);
    Power_InterpretResponse(msg);
}

void APP_WAKEUP_BUTTON_ISR(void)
{
    GPIO_GpioClearInterruptFlags(APP_WAKEUP_BUTTON_GPIO, 1U << APP_WAKEUP_BUTTON_PIN);
    DisableIRQ(APP_WAKEUP_BUTTON_IRQ);
}

void APP_EXT_INT_ISR(void)
{
    Power_ClearLpPowerSettings();
    GPIO_GpioClearInterruptFlags(APP_EXT_INT_GPIO, 1U << APP_EXT_INT_PIN);
    DisableIRQ(APP_EXT_INT_IRQ);
}

void LPTMR_AON_IRQHandler(void)
{
    Power_ClearLpPowerSettings();
    DisableIRQ(LPTMR_AON_IRQn);
    LPTMR_ClearStatusFlags(APP_LPTMR_BASE, kLPTMR_TimerCompareFlag);
    LPTMR_StopTimer(APP_LPTMR_BASE);
    LPTMR_DisableInterrupts(APP_LPTMR_BASE, kLPTMR_TimerInterruptEnable);
    __DSB();
    __ISB();
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
    uint32_t powerTrans;
    power_low_power_mode_t targetLpMode;
    BOARD_InitHardware();
    PRINTF(
        "\r\n###########################  Power Mode Switch Demo Primary Core Boot  ###########################\r\n");
    PRINTF("Normal Boot......\r\n");
    PRINTF("Core Clock Frequency: %d\r\n", CLOCK_GetCoreSysClkFreq());
    CMC_ConfigFlashMode(CMC, true, true, false);
    bool wakeupReset = APP_GetWakeupReason();
    smm_backup_reg_content_t backupReg;

    SMM_ReadFromBackupReg(AON__SMM, &backupReg);
    if ((wakeupReset == true) && (backupReg.word1 == (0x5A5A | ((uint32_t)kPower_DeepPowerDown1 << 16U))))
    {
        Power_RestoreHandleOffset(backupReg.word2);
        /* Wakeup from DPD1 mode. */
        backupReg.word1 = 0UL;
        backupReg.word2 = 0UL;
        SMM_WriteToBackupReg(AON__SMM, &backupReg);
    }
    else
    {
        backupReg.word1 = 0UL;
        backupReg.word2 = 0UL;
        SMM_WriteToBackupReg(AON__SMM, &backupReg);
        APP_ClearPlsRAM();
        APP_CopyCore1Image();
        APP_BootCore1();
        PRINTF("Start to communication with secondary core...\r\n");

        if (Power_CreateHandle(&powerHandle, APP_MU_CHANNEL) != kStatus_Success)
        {
            PRINTF("Fail to sync with secondary core!\r\n");
            return 0;
        }
    }

    while (1)
    {
        powerTrans   = DEMO_GetTargetPowerTransition();
        targetLpMode = DEMO_EnableWakeupSource(powerTrans);
        Power_EnterLowPowerMode(targetLpMode, powerConfigs[(uint8_t)targetLpMode]);
        Power_ClearTargetPowerMode();
        PRINTF("\r\n--------- Next Loop ---------\r\n");
    }
}

/*!
 * @brief Function to copy core1 image to execution address.
 */
static void APP_CopyCore1Image(void)
{
#ifdef CORE1_IMAGE_COPY_TO_RAM
    /* Calculate size of the image  - not required on MCUXpresso IDE. MCUXpresso copies the secondary core
       image to the target memory during startup automatically */
    uint32_t core1_image_size = get_core1_image_size();

    PRINTF("Copy Secondary core image to address: 0x%x, size: %d\r\n", CORE1_BOOT_ADDRESS, core1_image_size);

    /* Copy Secondary core application from FLASH to the target memory. */
#if defined(__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U)
    SCB_CleanInvalidateDCache_by_Addr((void *)CORE1_BOOT_ADDRESS, core1_image_size);
#endif
#ifdef CORE1_IMAGE_FLUSH_CACHE
    CORE1_IMAGE_FLUSH_CACHE(CORE1_BOOT_ADDRESS, core1_image_size);
#endif
    memcpy((void *)CORE1_BOOT_ADDRESS, (void *)CORE1_IMAGE_START, core1_image_size);
#if defined(__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U)
    SCB_CleanInvalidateDCache_by_Addr((void *)CORE1_BOOT_ADDRESS, core1_image_size);
#endif
#ifdef CORE1_IMAGE_FLUSH_CACHE
    CORE1_IMAGE_FLUSH_CACHE(CORE1_BOOT_ADDRESS, core1_image_size);
#endif
#endif
}

static power_low_power_mode_t DEMO_EnableWakeupSource(uint32_t powerTrans)
{
    bool wakeupSourceSupported = false;
    bool printExtIntWakeup     = false;
    bool printRtcWakeup        = false;
    bool printLptmrWakeup      = false;
    bool printButtonWakeup     = false;
    uint8_t firstMode          = (uint8_t)(powerTrans & 0xFUL);
    uint8_t secondMode         = (uint8_t)((powerTrans & 0xF0UL) >> 4UL);
    uint8_t thirdMode          = (uint8_t)((powerTrans & 0xF00UL) >> 8UL);

    if (firstMode == (uint8_t)kPower_DeepPowerDown2)
    {
        dpd2Config.wakeToDpd1 = false; /* In default wakeup to Active mode. */
    }
    if (firstMode == (uint8_t)kPower_DeepPowerDown1)
    {
        dpd1Config.nextTrans = kPower_Dpd1ToActive;
    }
    do
    {
        PRINTF("Please Select Wakeup Source...\r\n");
        if ((secondMode != (uint8_t)kPower_DeepPowerDown1))
        {
            if ((firstMode != (uint8_t)kPower_PowerDown1) && (firstMode != (uint8_t)kPower_PowerDown2))
            {
                /*In this transition, EXT_INT is used to wakeup from DPD1.*/
                PRINTF("\t[1]: %s button\r\n", APP_EXT_INT_BUTTON);
                printExtIntWakeup = true;
            }
        }
        if ((secondMode != (uint8_t)kPower_DeepPowerDown2) && (firstMode != (uint8_t)kPower_ShutDown))
        {
            /* In this transtion, timer is used to wakeup from DPD2. */
            PRINTF("\t[2]: RTC Alarm 0\r\n");
            PRINTF("\t[3]: RTC Alarm 1\r\n");
            printRtcWakeup = true;
            if (firstMode <= (uint8_t)kPower_DeepPowerDown2)
            {
                PRINTF("\t[4]: LPTMR\r\n");
                printLptmrWakeup = true;
            }
            if (firstMode <= (uint8_t)kPower_DeepSleep)
            {
                PRINTF("\t[5]: %s button\r\n", APP_WAKEUP_BUTTON);
                printButtonWakeup = true;
            }
        }
        PRINTF("\r\nWaiting for wakeup source select...\r\n");
        char ch = GETCHAR();
        switch (ch)
        {
            case '1': /* Ext_Int */
            {
                if (printExtIntWakeup)
                {
                    wakeupSourceSupported = true;
                    APP_EnableExtInternalWakeup(firstMode, secondMode, thirdMode);
                }
                break;
            }
            case '5': /* Wakeup Button */
            {
                if (printButtonWakeup)
                {
                    wakeupSourceSupported = true;
                    PRINTF("Please Press %s Button to wakeup!\r\n", APP_WAKEUP_BUTTON);
                    if (firstMode <= (uint8_t)kPower_DeepSleep)
                    {
                        BOARD_InitWakeupButtonAsGPIO();
                        EnableIRQ(APP_WAKEUP_BUTTON_IRQ);
                        GPIO_SetPinInterruptConfig(APP_WAKEUP_BUTTON_GPIO, APP_WAKEUP_BUTTON_PIN,
                                                   kGPIO_InterruptFallingEdge);
                    }
                    else
                    {
                        BOARD_InitWakeupButtonAsWUUPin();
                    }
                }
                break;
            }
            case '4':
            { /* LPTMR wakeup */
                if (printLptmrWakeup)
                {
                    wakeupSourceSupported = true;
                    APP_EnableLptmrWakeup(firstMode, secondMode, thirdMode);
                }
                break;
            }
            case '2':
            { /* RTC Alarm0 */
                if (printRtcWakeup)
                {
                    wakeupSourceSupported = true;
                    APP_EnableRTCAlarm0Wakeup(firstMode, secondMode, thirdMode);
                }
                break;
            }
            case '3':
            { /* RTC Alaram1 */
                if (printRtcWakeup)
                {
                    wakeupSourceSupported = true;
                    APP_EnableRTCAlarm1Wakeup(firstMode, secondMode, thirdMode);
                }
                break;
            }
            default:
            {
                PRINTF("Wrong Input! Please Retry\r\n");
                break;
            }
        }
    } while (wakeupSourceSupported == false);

    return (power_low_power_mode_t)firstMode;
}

static uint32_t DEMO_GetTargetPowerTransition(void)
{
    uint8_t ch;

    uint32_t inputPowerTrans;
    uint8_t index = 0U;

    do
    {
        PRINTF("\r\nSelect the desired operation...\r\n");
        for (uint32_t i = 0UL; i < APP_POWER_TRANS_COUNT; i++)
        {
            PRINTF("\tPress %c to start transition: %s\r\n", 'A' + i, g_modeTransArray[i]);
        }

        PRINTF("\r\nWaiting for selecting power transition...\r\n\r\n");

        ch = GETCHAR();

        if ((ch >= 'a') && (ch <= 'z'))
        {
            ch -= 'a' - 'A';
        }
        index = (ch - 'A');
        if (index >= APP_POWER_TRANS_COUNT)
        {
            PRINTF("Wrong Input!");
            continue;
        }
        else
        {
            inputPowerTrans = g_appPowerTrans[index];
            break;
        }
    } while (1);

    PRINTF("\r\n\t%s\r\n\r\n", g_modeDescArray[index]);

    return inputPowerTrans;
}

static void APP_EnableExtInternalWakeup(uint8_t firstMode, uint8_t secondMode, uint8_t thirdMode)
{
    PRINTF("Please Press %s Button to wakeup!\r\n", APP_EXT_INT_BUTTON);
    switch ((power_low_power_mode_t)firstMode)
    {
        case kPower_Sleep:
        case kPower_DeepSleep:
        {
            BOARD_InitExtIntButtonAsGPIO();
            EnableIRQ(APP_EXT_INT_IRQ);
            GPIO_SetPinInterruptConfig(APP_EXT_INT_GPIO, APP_EXT_INT_PIN, kGPIO_InterruptRisingEdge);
            break;
        }
        case kPower_DeepPowerDown1:
        {
            dpd1Config.mainWakeupSource = kPower_WS_Main_ExternalINTRiseEdge;
            if (secondMode == (uint8_t)kPower_DeepPowerDown2)
            {
                if (thirdMode == (uint8_t)kPower_DeepPowerDown1)
                {
                    dpd1Config.nextTrans = kPower_Dpd1ToDpd2WakeToDpd1;
                }
                else
                {
                    dpd1Config.nextTrans = kPower_Dpd1ToDpd2WakeToActive;
                }
            }
            break;
        }
        default:
        {
            Power_EnableWakeupSource(kPower_WS_Both_ExternalINTRiseEdge);
            break;
        }
    }
}

static void APP_EnableLptmrWakeup(uint8_t firstMode, uint8_t secondMode, uint8_t thirdMode)
{
    PRINTF("Will wakeup after 10s with LPTMR\r\n");

    switch ((power_low_power_mode_t)firstMode)
    {
        case kPower_PowerDown1:
        {
            pd1Config.mainWakeupSource = kPower_WS_Main_LptmrInt;
            break;
        }
        case kPower_PowerDown2:
        {
            pd2Config.aonWakeupSource  = kPower_WS_Aon_LptmrInt;
            pd2Config.mainWakeupSource = kPower_WS_Main_LptmrInt;
            break;
        }
        case kPower_DeepPowerDown1:
        {
            dpd1Config.mainWakeupSource = kPower_WS_Main_LptmrInt;
            break;
        }
        case kPower_DeepPowerDown2:
        {
            if (secondMode == (uint8_t)kPower_DeepPowerDown1)
            {
                dpd2Config.mainRamArraysToRetain = kPower_MainDomainNoneRams;
                dpd2Config.aonRamArraysToRetain  = kPower_AonDomainAllRams;
                dpd2Config.enableIVSMode         = true;
                dpd2Config.wakeToDpd1            = true;
                dpd2Config.aonWakeupSource       = kPower_WS_Aon_LptmrInt;
                dpd2Config.mainWakeupSource      = kPower_WS_Main_ExternalINTRiseEdge;
                dpd2Config.disableFRO10M         = false;
                dpd2Config.disableBandgap        = true;
                dpd2Config.switchToX32K          = true;
            }
            else
            {
                dpd2Config.aonWakeupSource      = kPower_WS_Aon_LptmrInt;
                dpd2Config.mainWakeupSource     = kPower_WS_Main_LptmrInt;
                dpd2Config.wakeToDpd1           = false;
                dpd2Config.aonRamArraysToRetain = kPower_AonDomainNoneRams;
            }
            break;
        }
        case kPower_DeepPowerDown3:
        {
            dpd3Config.wakeupSource = kPower_WS_Both_LptmrInt;
            break;
        }
        case kPower_ShutDown:
        {
            sdConfig.wakeupSource = kPower_WS_Both_LptmrInt;
            break;
        }

        default:
        {
            break;
        }
    }

    CLOCK_AttachClk(kFRO16K_to_AON_LPTMR);
    CLOCK_EnableClock(kCLOCK_GateAonLPTMR);
    RESET_ReleasePeripheralReset(kAonLPTMR_RST_SHIFT_RSTn);

    lptmr_config_t lptmrConfig;
    LPTMR_GetDefaultConfig(&lptmrConfig);
    LPTMR_Init(APP_LPTMR_BASE, &lptmrConfig);
    LPTMR_SetTimerPeriod(APP_LPTMR_BASE, USEC_TO_COUNT(LPTMR_USEC_COUNT, LPTMR_SOURCE_CLOCK));
    LPTMR_EnableInterrupts(APP_LPTMR_BASE, kLPTMR_TimerInterruptEnable);
    EnableIRQ(LPTMR_AON_IRQn);
    LPTMR_StartTimer(APP_LPTMR_BASE);
}

static void APP_EnableRTCAlarm0Wakeup(uint8_t firstMode, uint8_t secondMode, uint8_t thirdMode)
{
    PRINTF("Will wakeup after 10s with RTC Alarm0\r\n");

    switch ((power_low_power_mode_t)firstMode)
    {
        case kPower_PowerDown1:
        {
            pd1Config.mainWakeupSource = kPower_WS_Main_RtcAlarm0;
            break;
        }
        case kPower_PowerDown2:
        {
            pd2Config.mainWakeupSource = kPower_WS_Main_RtcAlarm0;
            pd2Config.aonWakeupSource  = kPower_WS_Aon_RtcAlarm0;
            break;
        }
        case kPower_DeepPowerDown1:
        {
            dpd1Config.mainWakeupSource = kPower_WS_Main_RtcAlarm0;
            break;
        }
        case kPower_DeepPowerDown2:
        {
            if (secondMode == (uint8_t)kPower_DeepPowerDown1)
            {
                dpd2Config.aonRamArraysToRetain = kPower_AonDomainAllRams;
                dpd2Config.enableIVSMode        = true;
                dpd2Config.wakeToDpd1           = true;
                dpd2Config.aonWakeupSource      = kPower_WS_Aon_RtcAlarm0;
                dpd2Config.mainWakeupSource     = kPower_WS_Main_ExternalINTRiseEdge;
            }
            else
            {
                dpd2Config.wakeToDpd1           = false;
                dpd2Config.aonRamArraysToRetain = kPower_AonDomainNoneRams;
                dpd2Config.aonWakeupSource      = kPower_WS_Aon_RtcAlarm0;
                dpd2Config.mainWakeupSource     = kPower_WS_Main_RtcAlarm0;
            }
            break;
        }
        case kPower_DeepPowerDown3:
        {
            dpd3Config.wakeupSource = kPower_WS_Both_RtcAlarm0;
            break;
        }

        case kPower_ShutDown:
        {
            sdConfig.wakeupSource = kPower_WS_Both_RtcAlarm0;
            break;
        }
        default:
        {
            break;
        }
    }

    AON__RTC_AON->CNT_H = 0x0000;
    AON__RTC_AON->CNT_M = 0x0000;
    AON__RTC_AON->CNT_L = 0x0000;
    /* Enable RTC alarm 0 */
    AON__RTC_AON->ALARM_H   = RTC_ALARM_H_ALARM_REP(1) | RTC_ALARM_H_ALARM_NUM(0);
    AON__RTC_AON->ALARM_MID = 0x0000;
    AON__RTC_AON->ALARM_LOW = RTC_ALARM0_TIME_SEC * 100;
    SDK_DelayAtLeastUs(6400, SystemCoreClock);
    AON__RTC_AON->CONFIG &= ~RTC_CONFIG_ALARM0_DIS_MASK;
    AON__RTC_AON->INT |= RTC_INT_ALARM0_IE_MASK;

    SDK_DelayAtLeastUs(6400, SystemCoreClock);
    EnableIRQ(RTC_ALARM0_IRQn);
    AON__RTC_AON->CONFIG |= RTC_CONFIG_EN_MASK | RTC_CONFIG_FREE_RUNNING_MASK;
}

static void APP_EnableRTCAlarm1Wakeup(uint8_t firstMode, uint8_t secondMode, uint8_t thirdMode)
{
    PRINTF("Will wakeup after 15s with RTC Alarm1\r\n");

    switch ((power_low_power_mode_t)firstMode)
    {
        case kPower_PowerDown1:
        {
            pd1Config.mainWakeupSource = kPower_WS_Main_RtcAlarm1;
            break;
        }
        case kPower_PowerDown2:
        {
            pd2Config.mainWakeupSource = kPower_WS_Main_RtcAlarm1;
            pd2Config.aonWakeupSource  = kPower_WS_Aon_RtcAlarm1;
            break;
        }
        case kPower_DeepPowerDown1:
        {
            dpd1Config.mainWakeupSource = kPower_WS_Main_RtcAlarm1;
            break;
        }
        case kPower_DeepPowerDown2:
        {
            if (secondMode == (uint8_t)kPower_DeepPowerDown1)
            {
                dpd2Config.aonRamArraysToRetain = kPower_AonDomainAllRams;
                dpd2Config.enableIVSMode        = true;
                dpd2Config.wakeToDpd1           = true;
                dpd2Config.aonWakeupSource      = kPower_WS_Aon_RtcAlarm1;
                dpd2Config.mainWakeupSource     = kPower_WS_Main_ExternalINTRiseEdge;
            }
            else
            {
                dpd2Config.wakeToDpd1           = false;
                dpd2Config.aonRamArraysToRetain = kPower_AonDomainNoneRams;
                dpd2Config.aonWakeupSource      = kPower_WS_Aon_RtcAlarm1;
                dpd2Config.mainWakeupSource     = kPower_WS_Main_RtcAlarm1;
            }
            break;
        }
        case kPower_DeepPowerDown3:
        {
            dpd3Config.wakeupSource = kPower_WS_Both_RtcAlarm1;
            break;
        }
        case kPower_ShutDown:
        {
            sdConfig.wakeupSource = kPower_WS_Both_RtcAlarm1;
            break;
        }
        default:
        {
            break;
        }
    }

    /* Enable RTC alarm 1 */
    AON__RTC_AON->ALARM_H   = RTC_ALARM_H_ALARM_REP(1) | RTC_ALARM_H_ALARM_NUM(1);
    AON__RTC_AON->ALARM_MID = 0x0000;
    AON__RTC_AON->ALARM_LOW = RTC_ALARM1_TIME_SEC * 100;
    SDK_DelayAtLeastUs(6400, SystemCoreClock);
    AON__RTC_AON->CONFIG &= ~RTC_CONFIG_ALARM1_DIS_MASK;
    AON__RTC_AON->INT |= RTC_INT_ALARM1_IE_MASK;

    SDK_DelayAtLeastUs(6400, SystemCoreClock);
    EnableIRQ(RTC_ALARM1_IRQn);
    AON__RTC_AON->CONFIG |= RTC_CONFIG_EN_MASK | RTC_CONFIG_FREE_RUNNING_MASK;
}

static bool APP_GetWakeupReason(void)
{
    uint32_t resetReasons = CMC_GetStickySystemResetStatus(CMC);
    PRINTF("Reset Reasons: %x\r\n", resetReasons);

    if (resetReasons == CMC_SSRS_WAKEUP_MASK)
    {
        if (NVIC_GetPendingIRQ(LPTMR_AON_IRQn) != 0U)
        {
            PRINTF("Wakeup Source: LPTMR\r\n");
            EnableIRQ(LPTMR_AON_IRQn);
            NVIC_ClearPendingIRQ(LPTMR_AON_IRQn);
        }
        else if (NVIC_GetPendingIRQ(RTC_ALARM0_IRQn) != 0U)
        {
            PRINTF("Wakeup Source: RTC Alarm0\r\n");
            EnableIRQ(RTC_ALARM0_IRQn);
            NVIC_ClearPendingIRQ(RTC_ALARM0_IRQn);
        }
        else if (NVIC_GetPendingIRQ(RTC_ALARM1_IRQn) != 0U)
        {
            PRINTF("Wakeup Source: RTC Alarm1\r\n");
            EnableIRQ(RTC_ALARM1_IRQn);
            NVIC_ClearPendingIRQ(RTC_ALARM1_IRQn);
        }
        else
        {
            PRINTF("Wakeup Source: EXT_INT\r\n");
        }

        return true;
    }

    return false;
}

static void APP_ClearPlsRAM(void)
{
    (void)memset((void *)0xA1000000UL, 0U, 32 * 1024);
}
