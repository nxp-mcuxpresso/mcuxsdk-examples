/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "wifi_bt_config.h"
#include "pin_mux.h"
#include "fsl_gpio.h"
#include "fsl_pcal6524.h"
#include "board.h"
#include "wifi_config.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#if defined(WIFI_IW612_BOARD_RD_USD) || defined(WIFI_IW612_BOARD_MURATA_2EL_USD) ||          \
    defined(WIFI_IW611_BOARD_MURATA_2DL_USD) || defined(WIFI_AW611_BOARD_UBX_JODY_W5_USD) || \
    defined(WIFI_IW416_BOARD_AW_AM510_USD) || defined(WIFI_IW610_BOARD_MURATA_2LL_USD)
#define CONTROLLER_RESET_GPIO HSP__GPIO_4
#define CONTROLLER_RESET_PIN  12U
#endif

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
#ifdef WIFI_BT_USE_M2_INTERFACE
extern uint32_t BOARD_USDHC1ClockConfiguration(void);
#endif
/*******************************************************************************
 * Variables
 ******************************************************************************/
#ifdef WIFI_BT_USE_M2_INTERFACE
/*!brief sdmmc dma buffer */
AT_NONCACHEABLE_SECTION_ALIGN(static uint32_t s_sdmmcHostDmaBuffer[BOARD_SDMMC_HOST_DMA_DESCRIPTOR_BUFFER_SIZE],
                              SDMMCHOST_DMA_DESCRIPTOR_BUFFER_ALIGN_SIZE);
#if defined SDMMCHOST_ENABLE_CACHE_LINE_ALIGN_TRANSFER && SDMMCHOST_ENABLE_CACHE_LINE_ALIGN_TRANSFER
/* two cache line length for sdmmc host driver maintain unalign transfer */
SDK_ALIGN(static uint8_t s_sdmmcCacheLineAlignBuffer[BOARD_SDMMC_DATA_BUFFER_ALIGN_SIZE * 2U],
          BOARD_SDMMC_DATA_BUFFER_ALIGN_SIZE);
#endif

static sd_detect_card_t s_cd;
static sd_io_voltage_t s_ioVoltage = {
    .type = BOARD_SDMMC_SD_IO_VOLTAGE_CONTROL_TYPE,
    .func = NULL,
};
static sdmmchost_t s_host;
static sdio_card_int_t s_sdioInt;
#endif

/*******************************************************************************
 * Code
 ******************************************************************************/
void BOARD_WIFI_BT_Enable(bool enable)
{
    if (enable)
    {
        /* Enable module */

#ifdef WIFI_BT_USE_M2_INTERFACE
        pcal6524_handle_t *handle = BOARD_GetPCAL6524Handle();
        /* Staged M.2 reset release (mirrors frdmimxrt1152): SDIO_RST (WIFI_RST_B), then WL_RST,
         * then BT_RST, each driven HIGH with a 100ms settle. No PDn on this board (rail always-on). */
        (void)PCAL6524_SetDirection(handle,
                                    (1UL << BOARD_PCAL6524_WIFI_RST_B) | (1UL << BOARD_PCAL6524_WL_RST) |
                                        (1UL << BOARD_PCAL6524_BT_RST),
                                    kPCAL6524_Output);
        (void)PCAL6524_SetPins(handle, 1UL << BOARD_PCAL6524_WIFI_RST_B);
        vTaskDelay(pdMS_TO_TICKS(100));
        (void)PCAL6524_SetPins(handle, 1UL << BOARD_PCAL6524_WL_RST);
        vTaskDelay(pdMS_TO_TICKS(100));
        (void)PCAL6524_SetPins(handle, 1UL << BOARD_PCAL6524_BT_RST);
        vTaskDelay(pdMS_TO_TICKS(100));
#elif defined(WIFI_BT_USE_USD_INTERFACE)
        /* Enable power supply for SD */
        GPIO_PinWrite(CONTROLLER_RESET_GPIO, CONTROLLER_RESET_PIN, 1);
        vTaskDelay(pdMS_TO_TICKS(100));
#endif /* WIFI_BT_USE_M2_INTERFACE */
    }
    else
    {
        /* Disable module */

#ifdef WIFI_BT_USE_M2_INTERFACE
        pcal6524_handle_t *handle = BOARD_GetPCAL6524Handle();

        /* Hold the module in reset: WL_RST, BT_RST, SDIO_RST (WIFI_RST_B) all driven LOW. */
        (void)PCAL6524_SetDirection(handle,
                                    (1UL << BOARD_PCAL6524_WIFI_RST_B) | (1UL << BOARD_PCAL6524_WL_RST) |
                                        (1UL << BOARD_PCAL6524_BT_RST),
                                    kPCAL6524_Output);
        (void)PCAL6524_ClearPins(handle, 1UL << BOARD_PCAL6524_WL_RST);
        (void)PCAL6524_ClearPins(handle, 1UL << BOARD_PCAL6524_BT_RST);
        (void)PCAL6524_ClearPins(handle, 1UL << BOARD_PCAL6524_WIFI_RST_B);
#elif defined(WIFI_BT_USE_USD_INTERFACE)
        /* Disable power supply for SD */
        GPIO_PinWrite(CONTROLLER_RESET_GPIO, CONTROLLER_RESET_PIN, 0);
#endif /* WIFI_BT_USE_M2_INTERFACE */

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void BOARD_WIFI_BT_Config(void *card, sdio_int_t cardInt)
{
#ifdef WIFI_BT_USE_M2_INTERFACE
    assert(card);

    s_host.dmaDesBuffer         = s_sdmmcHostDmaBuffer;
    s_host.dmaDesBufferWordsNum = BOARD_SDMMC_HOST_DMA_DESCRIPTOR_BUFFER_SIZE;
#if ((defined __DCACHE_PRESENT) && __DCACHE_PRESENT) || (defined FSL_FEATURE_HAS_L1CACHE && FSL_FEATURE_HAS_L1CACHE)
    s_host.enableCacheControl   = BOARD_SDMMC_HOST_CACHE_CONTROL;
#endif
#if defined SDMMCHOST_ENABLE_CACHE_LINE_ALIGN_TRANSFER && SDMMCHOST_ENABLE_CACHE_LINE_ALIGN_TRANSFER
    s_host.cacheAlignBuffer     = s_sdmmcCacheLineAlignBuffer;
    s_host.cacheAlignBufferSize = BOARD_SDMMC_DATA_BUFFER_ALIGN_SIZE * 2U;
#endif

    ((sdio_card_t *)card)->host                                = &s_host;
    ((sdio_card_t *)card)->host->hostController.base           = BOARD_SDMMC_SDIO_HOST_BASEADDR;
    /* Clock must come from USDHC1 (same controller as the SDIO host base above). */
    ((sdio_card_t *)card)->host->hostController.sourceClock_Hz = BOARD_USDHC1ClockConfiguration();

    memset(&s_cd, 0, sizeof(sd_detect_card_t));
    ((sdio_card_t *)card)->usrParam.cd         = &s_cd;
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

#elif defined(WIFI_BT_USE_USD_INTERFACE)
    BOARD_SDIO_Config(card, NULL, BOARD_SDMMC_SDIO_HOST_IRQ_PRIORITY, cardInt);
#endif

#if !defined(COEX_APP_SUPPORT) || (defined(COEX_APP_SUPPORT) && !defined(CONFIG_WIFI_IND_DNLD))
    BOARD_WIFI_BT_Enable(false);
#endif
}
