/*
 * Copyright 2022, 2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "wifi_bt_config.h"
#include "pin_mux.h"
#include "fsl_gpio.h"
#include "wifi_config.h"
#if defined(SDIO_SPI_ENABLED)
#include "fsl_lpspi.h"
#endif

static sdio_card_int_t s_sdioInt;
#if defined(SDIO_ENABLED)
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#ifdef WIFI_BT_USE_USD_INTERFACE
#warning "uSD interface is not supported"
#endif

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
extern uint32_t BOARD_USDHC1ClockConfiguration(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
/*!brief sdmmc dma buffer */
AT_NONCACHEABLE_SECTION_ALIGN(static uint32_t s_sdmmcHostDmaBuffer[BOARD_SDMMC_HOST_DMA_DESCRIPTOR_BUFFER_SIZE],
                              SDMMCHOST_DMA_DESCRIPTOR_BUFFER_ALIGN_SIZE);

static sd_io_voltage_t s_ioVoltage = {
    .type = BOARD_SDMMC_SD_IO_VOLTAGE_CONTROL_TYPE,
    .func = NULL,
};
static sdmmchost_t s_host;

/*******************************************************************************
 * Code
 ******************************************************************************/
void BOARD_WIFI_BT_Enable(bool enable)
{
    if (enable)
    {
        /* Enable module */

        /* Set SDIO_RST to 1 */
        GPIO_PinWrite(BOARD_INITPINSWIFI_SDIO_RST_GPIO, BOARD_INITPINSWIFI_SDIO_RST_GPIO_PIN, 1U);
        vTaskDelay(pdMS_TO_TICKS(100));

        /* Set WL_RST to 1 */
        GPIO_PinWrite(BOARD_INITPINSWIFI_WL_RST_GPIO, BOARD_INITPINSWIFI_WL_RST_GPIO_PIN, 1U);
        vTaskDelay(pdMS_TO_TICKS(100));
    }
    else
    {
        /* Disable module */

        /* Set SDIO_RST to 0 */
        GPIO_PinWrite(BOARD_INITPINSWIFI_SDIO_RST_GPIO, BOARD_INITPINSWIFI_SDIO_RST_GPIO_PIN, 0U);
        vTaskDelay(pdMS_TO_TICKS(100));

        /* Set WL_RST to 0 */
        GPIO_PinWrite(BOARD_INITPINSWIFI_WL_RST_GPIO, BOARD_INITPINSWIFI_WL_RST_GPIO_PIN, 0U);
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
#elif defined(SDIO_SPI_ENABLED)
#define EXAMPLE_LPSPI_MASTER_BASEADDR         (LPSPI1)
#define EXAMPLE_LPSPI_MASTER_PCS_FOR_INIT     (kLPSPI_Pcs0)
#define EXAMPLE_LPSPI_MASTER_PCS_FOR_TRANSFER (kLPSPI_MasterPcs0)
#define LPSPI_MASTER_CLK_FREQ                 CLOCK_GetLPFlexCommClkFreq(1u)
#define TRANSFER_BAUDRATE 25000000U /*! Transfer baudrate - 25Mhz */

static status_t BOARD_SdioSpiInit(void)
{
    lpspi_master_config_t masterConfig;

    /* Master config */
    LPSPI_MasterGetDefaultConfig(&masterConfig);
    masterConfig.baudRate = TRANSFER_BAUDRATE;
    masterConfig.whichPcs = EXAMPLE_LPSPI_MASTER_PCS_FOR_INIT;
    masterConfig.pcsToSckDelayInNanoSec        = 1000000000U / (masterConfig.baudRate * 2U);
    masterConfig.lastSckToPcsDelayInNanoSec    = 1000000000U / (masterConfig.baudRate * 2U);
    masterConfig.betweenTransferDelayInNanoSec = 1000000000U / (masterConfig.baudRate * 2U);

    LPSPI_MasterInit(EXAMPLE_LPSPI_MASTER_BASEADDR, &masterConfig, LPSPI_MASTER_CLK_FREQ);

    return kStatus_Success;
}

static void BOARD_SdioSpiDeinit(void)
{
    LPSPI_Deinit(EXAMPLE_LPSPI_MASTER_BASEADDR);
}

static status_t BOARD_SdioSpiTransfer(sdmmchost_spi_transfer_t *xfer)
{
    lpspi_transfer_t masterXfer;

    /* Start master transfer, send data to slave */
    masterXfer.txData   = xfer->txData;
    masterXfer.rxData   = xfer->rxData;
    masterXfer.dataSize = xfer->dataSize;
    masterXfer.configFlags =
        EXAMPLE_LPSPI_MASTER_PCS_FOR_TRANSFER | kLPSPI_MasterPcsContinuous | kLPSPI_MasterByteSwap;

    /* Release CS */
    if ((xfer->flags & SD_SPI_FLAG_CS_RELEASE) != 0U)
    {
        goto release;
    }

    /* Assert CS */
    if ((xfer->flags & SD_SPI_FLAG_CS_ACTIVE_HIGH) != 0U)
    {
        GPIO_PortSet(GPIO0, 1U << 28U);
    }
    else
    {
        GPIO_PortClear(GPIO0, 1U << 28U);
    }

    LPSPI_MasterTransferBlocking(EXAMPLE_LPSPI_MASTER_BASEADDR, &masterXfer);

    /* Hold CS */
    if ((xfer->flags & SD_SPI_FLAG_CS_HOLD) != 0U)
    {
        return kStatus_Success;
    }

release:

    /* De-assert CS */
    if ((xfer->flags & SD_SPI_FLAG_CS_ACTIVE_HIGH) != 0U)
    {
        GPIO_PortClear(GPIO0, 1U << 28U);
    }
    else
    {
        GPIO_PortSet(GPIO0, 1U << 28U);
    }

    return kStatus_Success;
}

static sdmmchost_t s_host = {
    .spiInit = BOARD_SdioSpiInit,
    .spiDeinit = BOARD_SdioSpiDeinit,
    .spiTransfer = BOARD_SdioSpiTransfer
};

static bool BOARD_SdioCardGetDetectStatus(void)
{
    return true;
}

static sd_detect_card_t s_cd = {
    .type = kSD_DetectCardByGpioCD,
    .cardDetected = BOARD_SdioCardGetDetectStatus
};

static status_t BOARD_SdioCardInterruptEnable(bool enable)
{
    if (enable)
    {
        EnableIRQ(GPIO00_IRQn);
    }
    else
    {
        DisableIRQ(GPIO00_IRQn);
    }

    return kStatus_Success;
}

static status_t BOARD_SdioCardInterruptInit(void *cardInterrupt)
{
    gpio_pin_config_t sw_config = {
        kGPIO_DigitalInput,
        0,
    };

    GPIO_SetPinInterruptConfig(GPIO0, 10U, kGPIO_InterruptFallingEdge);
    GPIO_PinInit(GPIO0, 10U, &sw_config);
    NVIC_SetPriority(GPIO00_IRQn, 5U);
    EnableIRQ(GPIO00_IRQn);

    return kStatus_Success;
}

void GPIO00_IRQHandler(void)
{
    GPIO_GpioClearInterruptFlags(GPIO0, 1U << 10U);

    if (s_sdioInt.cardInterrupt != NULL)
    {
        s_sdioInt.cardInterrupt(s_sdioInt.userData);
    }

    SDK_ISR_EXIT_BARRIER;
}
#endif

void BOARD_WIFI_BT_Config(void *card, sdio_int_t cardInt)
{
    assert(card);
#if defined(SDIO_ENABLED)
    s_host.dmaDesBuffer         = s_sdmmcHostDmaBuffer;
    s_host.dmaDesBufferWordsNum = BOARD_SDMMC_HOST_DMA_DESCRIPTOR_BUFFER_SIZE;

    ((sdio_card_t *)card)->host                                = &s_host;
    ((sdio_card_t *)card)->host->hostController.base           = BOARD_SDMMC_SDIO_HOST_BASEADDR;
    ((sdio_card_t *)card)->host->hostController.sourceClock_Hz = BOARD_USDHC1ClockConfiguration();

    ((sdio_card_t *)card)->usrParam.ioStrength = NULL;
    ((sdio_card_t *)card)->usrParam.ioVoltage  = &s_ioVoltage;
    ((sdio_card_t *)card)->usrParam.maxFreq    = BOARD_SDMMC_SD_HOST_SUPPORT_SDR104_FREQ;
    if (cardInt != NULL)
    {
        s_sdioInt.cardInterrupt                 = cardInt;
        ((sdio_card_t *)card)->usrParam.sdioInt = &s_sdioInt;
    }

    NVIC_SetPriority(BOARD_SDMMC_SDIO_HOST_IRQ, BOARD_SDMMC_SDIO_HOST_IRQ_PRIORITY);
    BOARD_WIFI_BT_Enable(false);
#elif defined(SDIO_SPI_ENABLED)
    ((sdio_card_t *)card)->host = &s_host;
    ((sdio_card_t *)card)->usrParam.cd = &s_cd;
    ((sdio_card_t *)card)->usrParam.isSpi = true;

    if (cardInt != NULL)
    {
        s_sdioInt.cardInterrupt                 = cardInt;
        ((sdio_card_t *)card)->usrParam.sdioInt = &s_sdioInt;

        ((sdio_card_t *)card)->host->interruptEnable = BOARD_SdioCardInterruptEnable;
        ((sdio_card_t *)card)->host->interruptInit = BOARD_SdioCardInterruptInit;
    }
#endif
}
