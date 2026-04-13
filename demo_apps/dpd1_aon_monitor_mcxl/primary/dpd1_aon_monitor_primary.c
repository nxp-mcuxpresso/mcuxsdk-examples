/*
 * Copyright 2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * DPD1 AON Monitor — CM33 (primary core) application.
 *
 * Enters DPD1 with context saving enabled.  CM0+ remains active,
 * printing periodic heartbeat logs via AON UART.  When the user
 * sends any character on the AON UART, CM0+ triggers CM33 wakeup
 * using the ROM CGU-clobber workaround (save/restore CGU around
 * SMM_WakeupMainDomain).
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

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void APP_CopyCore1Image(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
#if defined(__ICCARM__) /* IAR Workbench */
#pragma location = "rpmsg_sh_mem_section"
power_handle_t powerHandle;
#elif defined(__CC_ARM) || defined(__ARMCC_VERSION) /* Keil MDK */
power_handle_t powerHandle __attribute__((section("rpmsg_sh_mem_section")));
#elif defined(__GNUC__)
power_handle_t powerHandle __attribute__((section(".noinit.$rpmsg_sh_mem")));
#else
#error "Please provide your definition of powerHandle section!"
#endif

power_dpd1_config_t dpd1Config = {
    .mainWakeupSource      = kPower_WS_NONE, /* CM0+ handles wakeup manually via SMM_WakeupMainDomain. */
    .fro16KOutputFreq      = kPMU_FRO16KOutput16KHz,
    .disableBandgap        = true,
    .enableIVSMode         = true,
    .disableFRO10M         = true,
    .mainRamArraysToRetain = kPower_MainDomainAllRams,
    .saveContext           = true,
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

int main(void)
{
    power_drv_config_t drvConfig = {
        .muChannelId = APP_MU_CHANNEL,
        .noSyncCM0P  = false,
    };

    BOARD_InitHardware();
    PRINTF(
        "\r\n###########################  DPD1 AON Monitor Demo — CM33 Boot  "
        "###########################\r\n");
    PRINTF("Core Clock: %d Hz\r\n", CLOCK_GetCoreSysClkFreq());

    CMC_ConfigFlashMode(CMC, true, true, true);

    APP_CopyCore1Image();
    APP_BootCore1();
    PRINTF("Syncing with CM0+...\r\n");
    if (Power_CreateHandle(&powerHandle, &drvConfig) != kStatus_Success)
    {
        PRINTF("Fail to sync with CM0+!\r\n");
        return 0;
    }
    PRINTF("Dual-core sync OK\r\n");

    dpd1Config.nextTrans = kPower_Dpd1ToActive;
    while (1)
    {
        PRINTF("Press any key to start the demo...\r\n");
        GETCHAR();

        AON__PMU->BGR_LVHV_DETECT_CTRL &= ~(0x2U);
        uint32_t dummyValue = AON__PMU->AWK_UP_TIME;
        AON__PMU->AWK_UP_TIME =
            (AON__PMU->AWK_UP_TIME & ~PMU_AWK_UP_TIME_WKUP_TIME_MASK) | PMU_AWK_UP_TIME_WKUP_TIME(dummyValue);
        SDK_DelayAtLeastUs(1000, CLOCK_GetCoreSysClkFreq());
        PRINTF("\r\nEntering DPD1 (context saving enabled) - CM0+ stays active...\r\n");
        PRINTF("Send any character on AON UART to wake up.\r\n\r\n");

        /* Enable AON LPUART RX interrupt at peripheral level.
         * CM0+ will enable the corresponding NVIC IRQ in its callback
         * and handle the actual interrupt to trigger wakeup. */
        LPUART_EnableInterrupts(APP_AON_LPUART, kLPUART_RxDataRegFullInterruptEnable);

        status_t status = Power_EnterLowPowerMode(kPower_DeepPowerDown1, &dpd1Config);
        if (status != kStatus_Fail)
        {
            /* Context-saving wakeup: CM33 resumes here.
             * Re-init hardware; CM0+ has already called Power_NotifyCM33ToRun
             * to unblock us (dualCoreSynced handshake). */
            BOARD_InitHardware();
            Power_DisableAllWakeupSources();
            CMC_EnableDebugOperation(CMC, false);
            EnableIRQ(MU_A_RX_IRQn);
            MU_EnableInterrupts(APP_MU, (kMU_Rx0FullInterruptEnable));

            PRINTF("\r\n*** Wakeup from DPD1 ***\r\n");
            PRINTF("\r\n--------- Next Loop --------- \r\n");
            CMC_ConfigFlashMode(CMC, true, true, true);
        }
        else
        {
            PRINTF("Fail to enter DPD1!\r\n");
            return -1;
        }
    }
}

static void APP_CopyCore1Image(void)
{
#ifdef CORE1_IMAGE_COPY_TO_RAM
    uint32_t core1_image_size = get_core1_image_size();

    PRINTF("Copy CM0+ image to 0x%x, size: %d\r\n", CORE1_BOOT_ADDRESS, core1_image_size);

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
