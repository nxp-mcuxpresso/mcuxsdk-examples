/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "wifi_bt_config.h"
#include "pin_mux.h"
#include "fsl_gpio.h"
#include "wifi_config.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
extern uint32_t BOARD_USDHC1ClockConfiguration(void);
extern void BOARD_SDCardIoVoltageControl(sdmmc_operation_voltage_t voltage);
extern void BOARD_SDCardDAT3PullFunction(uint32_t status);

/*******************************************************************************
 * Variables
 ******************************************************************************/
/*!brief sdmmc dma buffer */
AT_NONCACHEABLE_SECTION_ALIGN(static uint32_t s_sdmmcHostDmaBuffer[BOARD_SDMMC_HOST_DMA_DESCRIPTOR_BUFFER_SIZE],
                              SDMMCHOST_DMA_DESCRIPTOR_BUFFER_ALIGN_SIZE);

static sd_detect_card_t s_cd;
static sd_io_voltage_t s_ioVoltage = {
    .type = BOARD_SDMMC_SD_IO_VOLTAGE_CONTROL_TYPE,
    .func = BOARD_SDCardIoVoltageControl,
};

static sdmmchost_t s_host;
static sdio_card_int_t s_sdioInt;


/*******************************************************************************
 * Code
 ******************************************************************************/
void BOARD_WIFI_BT_Enable(bool enable)
{
    if (enable)
    {
        /* Enable module */

        /* Enable power supply for M.2 */
        GPIO_PortSet(BOARD_INITPINSM2_M2_PWREN_PERIPHERAL, BOARD_INITPINSM2_M2_PWREN_PIN_MASK);
        vTaskDelay(pdMS_TO_TICKS(100));

        /* Set WL_RST to 1 */
        GPIO_PortSet(BOARD_INITPINSM2_WL_RST_PERIPHERAL, BOARD_INITPINSM2_WL_RST_PIN_MASK);
        vTaskDelay(pdMS_TO_TICKS(100));

    }
    else
    {
        /* Disable module */

        /* Set WL_RST to 0 */
        GPIO_PortClear(BOARD_INITPINSM2_WL_RST_PERIPHERAL, BOARD_INITPINSM2_WL_RST_PIN_MASK);

        /* Disable power supply for M.2 */
        GPIO_PortClear(BOARD_INITPINSM2_M2_PWREN_PERIPHERAL, BOARD_INITPINSM2_M2_PWREN_PIN_MASK);

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void BOARD_WIFI_BT_CardDetectInit(sd_cd_t cd, void *userData)
{
    /* install card detect callback */
    s_cd.cdDebounce_ms = BOARD_SDMMC_SD_CARD_DETECT_DEBOUNCE_DELAY_MS;
    s_cd.type          = kSD_DetectCardByHostDATA3;
    s_cd.callback      = cd;
    s_cd.userData      = userData;

    /* register DAT3 pull function switch function pointer */
    s_cd.dat3PullFunc = BOARD_SDCardDAT3PullFunction;
}

void BOARD_WIFI_BT_Config(void *card, sdio_int_t cardInt)
{
    assert(card);

    s_host.dmaDesBuffer                                        = s_sdmmcHostDmaBuffer;
    s_host.dmaDesBufferWordsNum                                = BOARD_SDMMC_HOST_DMA_DESCRIPTOR_BUFFER_SIZE;
    ((sdio_card_t *)card)->host                                = &s_host;
    ((sdio_card_t *)card)->host->hostController.base           = BOARD_SDMMC_SDIO_HOST_BASEADDR;
    ((sdio_card_t *)card)->host->hostController.sourceClock_Hz = BOARD_USDHC1ClockConfiguration();
    ((sdio_card_t *)card)->host->tuningType                    = BOARD_SDMMC_SD_TUNING_TYPE;

    ((sdio_card_t *)card)->usrParam.cd = &s_cd;
    ((sdio_card_t *)card)->usrParam.pwr       = NULL;
    ((sdio_card_t *)card)->usrParam.ioVoltage = &s_ioVoltage;
    if (cardInt != NULL)
    {
        s_sdioInt.cardInterrupt                 = cardInt;
        ((sdio_card_t *)card)->usrParam.sdioInt = &s_sdioInt;
    }

    BOARD_WIFI_BT_CardDetectInit(NULL, NULL);
    NVIC_SetPriority(BOARD_SDMMC_SDIO_HOST_IRQ, BOARD_SDMMC_SDIO_HOST_IRQ_PRIORITY);

    BOARD_WIFI_BT_Enable(false);
}
