/*
 * Copyright 2026 NXP
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
#include "fsl_cmc.h"
#include "fsl_smm.h"
#include "fsl_lpuart.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define APP_POWER_MODE_COUNT 6U

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void APP_CopyCore1Image(void);
static power_low_power_mode_t APP_GetTargetPowerMode(void);
static void APP_ConfigureWakeupSource(power_low_power_mode_t mode);

/*******************************************************************************
 * Variables
 ******************************************************************************/
static const char *const g_modeTransArray[APP_POWER_MODE_COUNT] = {
    "Active --> Sleep --> Active",
    "Active --> Deep Sleep --> Active",
    "Active --> Power Down1 --> Active",
    "Active --> Power Down2 --> Active",
    "Active --> Deep Power Down1 --> Active",
    "Active --> Deep Power Down2 --> Active",
};

static const power_low_power_mode_t g_modeMap[APP_POWER_MODE_COUNT] = {
    kPower_Sleep, kPower_DeepSleep, kPower_PowerDown1, kPower_PowerDown2, kPower_DeepPowerDown1, kPower_DeepPowerDown2,
};

#if defined(__ICCARM__)
#pragma location = "rpmsg_sh_mem_section"
power_handle_t powerHandle;
#elif defined(__CC_ARM) || defined(__ARMCC_VERSION)
power_handle_t powerHandle __attribute__((section("rpmsg_sh_mem_section")));
#elif defined(__GNUC__)
power_handle_t powerHandle __attribute__((section(".noinit.$rpmsg_sh_mem")));
#else
#error "Please provide your definition of powerHandle placement!"
#endif

static power_pd1_config_t pd1Config = {
    .mainWakeupSource = kPower_WS_Main_Lpuart0,
    .enableIVSMode    = false,
    .fro16KOutputFreq = kPMU_FRO16KOutput16KHz,
};

static power_pd2_config_t pd2Config = {
    .mainWakeupSource  = kPower_WS_Main_Lpuart0,
    .aonWakeupSource   = kPower_WS_Aon_Lpuart0,
    .enableIVSMode     = false,
    .disableFRO10M     = false,
    .fro16KOutputFreq  = kPMU_FRO16KOutput16KHz,
    .vddCoreAonVoltage = kPower_VddCoreAon_725mV,
};

static power_dpd1_config_t dpd1Config = {
    .mainWakeupSource      = kPower_WS_Main_Lpuart0,
    .fro16KOutputFreq      = kPMU_FRO16KOutput16KHz,
    .disableBandgap        = true,
    .enableIVSMode         = true,
    .disableFRO10M         = false,
    .mainRamArraysToRetain = kPower_MainDomainNoneRams,
    .saveContext           = false,
    .nextTrans             = kPower_Dpd1ToActive,
};

static power_dpd2_config_t dpd2Config = {
    .mainWakeupSource      = kPower_WS_Main_Lpuart0,
    .aonWakeupSource       = kPower_WS_Aon_Lpuart0,
    .enableIVSMode         = true,
    .aonRamArraysToRetain  = kPower_AonDomainNoneRams,
    .mainRamArraysToRetain = kPower_MainDomainNoneRams,
    .fro16KOutputFreq      = kPMU_FRO16KOutput16KHz,
    .disableBandgap        = true,
    .switchToX32K          = true,
    .disableFRO10M         = false,
    .saveContext           = false,
    .disableFRO3M          = true,
    .wakeToDpd1            = false,
    .dpd2VddCoreAonVoltage = kPower_VddCoreAon_601_5mV,
};

static void *powerConfigs[8U] = {
    NULL,                /* Sleep */
    NULL,                /* DeepSleep */
    (void *)&pd1Config,  /* PD1 */
    (void *)&pd2Config,  /* PD2 */
    (void *)&dpd1Config, /* DPD1 */
    (void *)&dpd2Config, /* DPD2 */
    NULL,                /* DPD3 - not used */
    NULL,                /* SD   - not used */
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

/* AON LPUART ISR on CM33 — used to wake from Sleep/DeepSleep. */
void APP_AON_LPUART_IRQ_HANDLER(void)
{
    if (LPUART_GetStatusFlags(APP_AON_LPUART) & (uint32_t)kLPUART_RxDataRegFullFlag)
    {
        (void)LPUART_ReadByte(APP_AON_LPUART);
    }
    LPUART_ClearStatusFlags(APP_AON_LPUART, (uint32_t)kLPUART_RxOverrunFlag);
    DisableIRQ(APP_AON_LPUART_IRQn);
}

int main(void)
{
    power_low_power_mode_t targetMode;
    power_drv_config_t drvConfig = {
        .muChannelId = APP_MU_CHANNEL,
        .noSyncCM0P  = false,
    };

    BOARD_InitHardware();
    PRINTF(
        "\r\n###########################  AON LPUART Wakeup Demo (CM33)  "
        "###########################\r\n");
    PRINTF("Normal Boot......\r\n");
    PRINTF("Core Clock Frequency: %d\r\n", CLOCK_GetCoreSysClkFreq());

    CMC_ConfigFlashMode(CMC, true, true, true);
    APP_CopyCore1Image();
    APP_BootCore1();
    PRINTF("Start to communication with secondary core...\r\n");
    if (Power_CreateHandle(&powerHandle, &drvConfig) != kStatus_Success)
    {
        PRINTF("Fail to sync with secondary core!\r\n");
        return 0;
    }

    while (1)
    {
        targetMode = APP_GetTargetPowerMode();
        APP_ConfigureWakeupSource(targetMode);

        PRINTF("Send any character on AON LPUART to wakeup!\r\n");

        status_t status = Power_EnterLowPowerMode(targetMode, powerConfigs[(uint8_t)targetMode]);
        if (status != kStatus_Fail)
        {
            BOARD_InitHardware();
            Power_ClearTargetPowerMode();
            Power_DisableAllWakeupSources();
            CMC_EnableDebugOperation(CMC, false);

            EnableIRQ(MU_A_RX_IRQn);
            MU_EnableInterrupts(APP_MU, (kMU_Rx0FullInterruptEnable));

            PRINTF("Wakeup Successfully\r\n");
            CMC_ConfigFlashMode(CMC, true, true, true);
        }
        else
        {
            PRINTF("Fail to enter selected low power mode!\r\n");
            return -1;
        }
        PRINTF("\r\n--------- Next Loop ---------\r\n");
    }
}

static void APP_CopyCore1Image(void)
{
#ifdef CORE1_IMAGE_COPY_TO_RAM
    uint32_t core1_image_size = get_core1_image_size();

    PRINTF("Copy Secondary core image to address: 0x%x, size: %d\r\n", CORE1_BOOT_ADDRESS, core1_image_size);

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

static power_low_power_mode_t APP_GetTargetPowerMode(void)
{
    char ch;
    uint8_t index;

    do
    {
        PRINTF("\r\nSelect power mode (AON LPUART wakeup)...\r\n");
        for (uint32_t i = 0UL; i < APP_POWER_MODE_COUNT; i++)
        {
            PRINTF("\tPress %c: %s\r\n", 'A' + i, g_modeTransArray[i]);
        }
        PRINTF("\r\nWaiting for selection...\r\n");

        ch = GETCHAR();
        if ((ch >= 'a') && (ch <= 'z'))
        {
            ch -= 'a' - 'A';
        }
        index = (uint8_t)(ch - 'A');
        if (index >= APP_POWER_MODE_COUNT)
        {
            PRINTF("Wrong Input!\r\n");
            continue;
        }
        break;
    } while (1);

    PRINTF("\r\n\t%s\r\n\r\n", g_modeTransArray[index]);
    return g_modeMap[index];
}

static void APP_ConfigureWakeupSource(power_low_power_mode_t mode)
{
    /* Enable AON LPUART RX interrupt at the peripheral level for ALL modes.
     * Without RIE set, the LPUART never asserts its interrupt line when a
     * character arrives and the SMM wakeup controller cannot detect it.
     * For Sleep/DeepSleep, the same interrupt also feeds CM33's NVIC. */
    LPUART_EnableInterrupts(APP_AON_LPUART, kLPUART_RxDataRegFullInterruptEnable);
    NVIC_ClearPendingIRQ(APP_AON_LPUART_IRQn);
    EnableIRQ(APP_AON_LPUART_IRQn);
    (void)mode;
}
