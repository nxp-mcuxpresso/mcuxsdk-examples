/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "wifi_bt_config.h"
#include "pin_mux.h"
#include "board.h"
#include "fsl_gpio.h"
#include "fsl_pcal6524.h"
#include "fsl_adapter_gpio.h"
#include "wifi_config.h"

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
extern uint32_t BOARD_USDHC1ClockConfiguration(void);
#if __CORTEX_M == 7
extern void BOARD_USDHC_Errata(void);
#endif

/*******************************************************************************
 * Variables
 ******************************************************************************/
AT_NONCACHEABLE_SECTION_ALIGN(static uint32_t s_sdmmcHostDmaBuffer[BOARD_SDMMC_HOST_DMA_DESCRIPTOR_BUFFER_SIZE],
                              SDMMCHOST_DMA_DESCRIPTOR_BUFFER_ALIGN_SIZE);
#if defined SDMMCHOST_ENABLE_CACHE_LINE_ALIGN_TRANSFER && SDMMCHOST_ENABLE_CACHE_LINE_ALIGN_TRANSFER
SDK_ALIGN(static uint8_t s_sdmmcCacheLineAlignBuffer[BOARD_SDMMC_DATA_BUFFER_ALIGN_SIZE * 2U],
          BOARD_SDMMC_DATA_BUFFER_ALIGN_SIZE);
#endif

static sd_io_voltage_t s_ioVoltage = {
    .type = BOARD_SDMMC_SD_IO_VOLTAGE_CONTROL_TYPE,
    .func = NULL,
};

static sdmmchost_t s_host;
static sdio_card_int_t s_sdioInt;

/*******************************************************************************
 * Code
 ******************************************************************************/

void BOARD_WIFI_BT_Enable(bool enable)
{
    pcal6524_handle_t handle;
    BOARD_InitPCAL6524(&handle);

    /* Configure WL_RST (P0_4), BT_RST (P0_5), PDn (P0_6) as outputs */
    PCAL6524_SetDirection(&handle,
                          (1UL << BOARD_PCAL6524_WL_RST) |
                          (1UL << BOARD_PCAL6524_BT_RST) |
                          (1UL << BOARD_PCAL6524_WL_DEV_WAKE) |
                          (1UL << BOARD_PCAL6524_PDn),
                          kPCAL6524_Output);

    if (enable)
    {
        /* Power up and release resets */
        PCAL6524_SetPins(&handle, (1UL << BOARD_PCAL6524_PDn));
        vTaskDelay(pdMS_TO_TICKS(100));

        /* De-assert WL_RST# (set HIGH = release WiFi from reset) */
        PCAL6524_SetPins(&handle, (1UL << BOARD_PCAL6524_WL_RST));
        vTaskDelay(pdMS_TO_TICKS(100));

        /* De-assert BT_RST# (set HIGH = release BT from reset) */
        PCAL6524_SetPins(&handle, (1UL << BOARD_PCAL6524_BT_RST));
        vTaskDelay(pdMS_TO_TICKS(100));
    }
    else
    {
        /* Assert WL_RST# (set LOW = hold WiFi in reset) */
        PCAL6524_ClearPins(&handle, (1UL << BOARD_PCAL6524_WL_RST));

        /* Assert BT_RST# (set LOW = hold BT in reset) */
        PCAL6524_ClearPins(&handle, (1UL << BOARD_PCAL6524_BT_RST));

        /* Assert PDn (set LOW = power down module) */
        PCAL6524_ClearPins(&handle, (1UL << BOARD_PCAL6524_PDn));

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void BOARD_WIFI_BT_Config(void *card, sdio_int_t cardInt)
{
    assert(card);

    s_host.dmaDesBuffer         = s_sdmmcHostDmaBuffer;
    s_host.dmaDesBufferWordsNum = BOARD_SDMMC_HOST_DMA_DESCRIPTOR_BUFFER_SIZE;
#if ((defined __DCACHE_PRESENT) && __DCACHE_PRESENT) || (defined FSL_FEATURE_HAS_L1CACHE && FSL_FEATURE_HAS_L1CACHE)
    s_host.enableCacheControl = BOARD_SDMMC_HOST_CACHE_CONTROL;
#endif
#if defined SDMMCHOST_ENABLE_CACHE_LINE_ALIGN_TRANSFER && SDMMCHOST_ENABLE_CACHE_LINE_ALIGN_TRANSFER
    s_host.cacheAlignBuffer     = s_sdmmcCacheLineAlignBuffer;
    s_host.cacheAlignBufferSize = BOARD_SDMMC_DATA_BUFFER_ALIGN_SIZE * 2U;
#endif

    ((sdio_card_t *)card)->host                                = &s_host;
    ((sdio_card_t *)card)->host->hostController.base           = BOARD_SDMMC_SDIO_HOST_BASEADDR;
    ((sdio_card_t *)card)->host->hostController.sourceClock_Hz = BOARD_USDHC1ClockConfiguration();

    ((sdio_card_t *)card)->usrParam.cd         = NULL;  /* No card detect — module always present */
    ((sdio_card_t *)card)->usrParam.pwr        = NULL;
    ((sdio_card_t *)card)->usrParam.ioStrength = NULL;
    ((sdio_card_t *)card)->usrParam.ioVoltage  = &s_ioVoltage;
    ((sdio_card_t *)card)->usrParam.maxFreq    = BOARD_SDMMC_SD_HOST_SUPPORT_SDR104_FREQ;
    if (cardInt != NULL)
    {
        s_sdioInt.cardInterrupt                 = cardInt;
        ((sdio_card_t *)card)->usrParam.sdioInt = &s_sdioInt;
    }

    NVIC_SetPriority(BOARD_SDMMC_SDIO_HOST_IRQ, BOARD_SDMMC_SDIO_HOST_IRQ_PRIORITY);
#if __CORTEX_M == 7
    BOARD_USDHC_Errata();
#endif

#if !defined(COEX_APP_SUPPORT) || (defined(COEX_APP_SUPPORT) && !defined(CONFIG_WIFI_IND_DNLD))
    BOARD_WIFI_BT_Enable(false);
#endif
}
