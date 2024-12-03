/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "fsl_sdio.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* define data buffer size */
#define DATA_BUFFER_SIZE             (512)
#define SDIOSLV_TRANSFER_COUNT_ADDR  (0x0c)
#define SDIOSLV_TRANSFER_DATA_WINDOW (0x00U)
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void cardInterrupt(void *userData);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*! @brief Card descriptor */
sdio_card_t g_sdio;

/* define the tuple list */
static const uint32_t g_funcTupleList[2U] = {
    SDIO_TPL_CODE_FUNCID,
    SDIO_TPL_CODE_FUNCE,
};

/* @brief decription about the read/write buffer
 * The address of the read/write buffer should align to the specific DMA data buffer address align value if
 * DMA transfer is used, otherwise the buffer address is not important.
 * At the same time buffer address/size should be aligned to the cache line size if cache is supported.
 */
/*! @brief Data written to the card */
SDK_ALIGN(uint8_t g_dataRead[SDK_SIZEALIGN(DATA_BUFFER_SIZE, SDMMC_DATA_BUFFER_ALIGN_CACHE)],
          MAX(SDMMC_DATA_BUFFER_ALIGN_CACHE, SDMMCHOST_DMA_BUFFER_ADDR_ALIGN));
/*! @brief Data read from the card */
SDK_ALIGN(uint8_t g_dataBlockRead[SDK_SIZEALIGN(DATA_BUFFER_SIZE, SDMMC_DATA_BUFFER_ALIGN_CACHE)],
          MAX(SDMMC_DATA_BUFFER_ALIGN_CACHE, SDMMCHOST_DMA_BUFFER_ADDR_ALIGN));
/*! @brief SDMMC host detect card configuration */
static const sdmmchost_detect_card_t s_sdioCardDetect = {
#ifndef BOARD_SD_DETECT_TYPE
    .cdType = kSDMMCHOST_DetectCardByGpioCD,
#else
    .cdType = BOARD_SD_DETECT_TYPE,
#endif
    .cdTimeOut_ms = (~0U),
};

static const sdmmchost_card_int_t s_sdioCardInt = {
    .userData      = NULL,
    .cardInterrupt = cardInterrupt,
};

static volatile bool isCardInt = false;
/*******************************************************************************
 * Code
 ******************************************************************************/
static void cardInterrupt(void *userData)
{
    isCardInt = true;
    SDMMCHOST_DISABLE_SDIO_INT(SD_HOST_BASEADDR);
}

/*!
 * @brief Main function
 */
int main(void)
{
    sdio_card_t *card      = &g_sdio;
    uint8_t pendingInt     = 0U;
    uint32_t transferCount = 0;

    BOARD_InitHardware();

    PRINTF("SDIO card simple example.\r\n");

    card->host.base           = SD_HOST_BASEADDR;
    card->host.sourceClock_Hz = SD_HOST_CLK_FREQ;
    /* card detect type */
    card->usrParam.cd      = &s_sdioCardDetect;
    card->usrParam.cardInt = &s_sdioCardInt;
    /* SD host init function */
    if (SDIO_HostInit(card) != kStatus_Success)
    {
        PRINTF("\r\nSDIO host init fail\r\n");
        return -1;
    }

    PRINTF("\r\nPlease insert a card into board.\r\n");
    /* power off card */
    SDIO_PowerOffCard(card->host.base, card->usrParam.pwr);

    if (SDIO_WaitCardDetectStatus(SD_HOST_BASEADDR, &s_sdioCardDetect, true) == kStatus_Success)
    {
        PRINTF("\r\nCard inserted.\r\n");
        /* reset host once card re-plug in */
        SDIO_HostReset(&(card->host));
        /* power on the card */
        SDIO_PowerOnCard(card->host.base, card->usrParam.pwr);
    }
    else
    {
        PRINTF("\r\nCard detect fail.\r\n");
        return -1;
    }

    /* Init SDIO card. */
    if (kStatus_Success != SDIO_CardInit(card))
    {
        PRINTF("\r\nSDIO card init failed.\r\n");
        return kStatus_Fail;
    }

    if (card->ioTotalNumber > 0U)
    {
        /* read function capability through GetCardCapability function first */
        if (kStatus_Success != SDIO_GetCardCapability(card, kSDIO_FunctionNum1))
        {
            return kStatus_Fail;
        }

        PRINTF("\r\nRead function CIS, in direct way\r\n");
        if (kStatus_Success != SDIO_ReadCIS(card, kSDIO_FunctionNum1, g_funcTupleList, 2U))
        {
            return kStatus_Fail;
        }

        PRINTF("\r\nRead function CIS, in extended way, non-block mode, non-word aligned size\r\n");

        if (kStatus_Success != SDIO_IO_Read_Extended(card, kSDIO_FunctionNum0, card->ioFBR[0].ioPointerToCIS,
                                                     g_dataRead, 31U, SDIO_EXTEND_CMD_OP_CODE_MASK))
        {
            return kStatus_Fail;
        }
        PRINTF("\r\nRead function CIS, in extended way, block mode, non-word aligned size\r\n");

        /* set block size to a non-word aligned size for test */
        if (kStatus_Success != SDIO_SetBlockSize(card, kSDIO_FunctionNum0, 31U))
        {
            return kStatus_Fail;
        }

        if (kStatus_Success != SDIO_IO_Read_Extended(card, kSDIO_FunctionNum0, card->ioFBR[0].ioPointerToCIS,
                                                     g_dataBlockRead, 1U,
                                                     SDIO_EXTEND_CMD_OP_CODE_MASK | SDIO_EXTEND_CMD_BLOCK_MODE_MASK))
        {
            return kStatus_Fail;
        }

        if (memcmp(g_dataRead, g_dataBlockRead, 31U))
        {
            PRINTF("\r\nThe read content isn't consistent.\r\n");
        }
        else
        {
            PRINTF("\r\nThe read content is consistent.\r\n");
        }
    }

    if (kStatus_Success != SDIO_SetBlockSize(card, kSDIO_FunctionNum1, 32U))
    {
        return kStatus_Fail;
    }

    isCardInt = false;
    SDMMCHOST_ENABLE_SDIO_INT(card->host.base);

    if (kStatus_Success != SDIO_EnableAsyncInterrupt(card, true))
    {
        return kStatus_Fail;
    }

    if (SDIO_EnableIO(card, kSDIO_FunctionNum1, true) != kStatus_Success)
    {
        PRINTF("Enable IO failed\r\n");
        return -1;
    }

    if (SDIO_SelectIO(card, kSDIO_FunctionNum1) != kStatus_Success)
    {
        PRINTF("Select IO failed\r\n");
        return -1;
    }

    if (SDIO_EnableIOInterrupt(card, kSDIO_FunctionNum1, true) != kStatus_Success)
    {
        PRINTF("Enable IO interrupt failed\r\n");
        return -1;
    }

    g_dataBlockRead[0]   = 0x11;
    g_dataBlockRead[256] = 0x66;

    while (!isCardInt)
    {
    }

    if (kStatus_Success != SDIO_IO_RW_Direct(card, kSDIO_IORead, kSDIO_FunctionNum1, 8, 0xF, &pendingInt))
    {
        return kStatus_Fail;
    }

    if (pendingInt & 1U)
    {
        /* load transfer count */
        if (kStatus_Success != SDIO_IO_Read_Extended(card, kSDIO_FunctionNum1, SDIOSLV_TRANSFER_COUNT_ADDR,
                                                     (uint8_t *)&transferCount, 4, SDIO_EXTEND_CMD_OP_CODE_MASK))
        {
            return kStatus_Fail;
        }

        memset(g_dataBlockRead, 0U, sizeof(g_dataBlockRead));
        if (kStatus_Success != SDIO_IO_Read_Extended(card, kSDIO_FunctionNum1, SDIOSLV_TRANSFER_DATA_WINDOW,
                                                     g_dataBlockRead, transferCount / 32U,
                                                     SDIO_EXTEND_CMD_BLOCK_MODE_MASK))
        {
            return kStatus_Fail;
        }
    }

    if (kStatus_Success != SDIO_IO_Write_Extended(card, kSDIO_FunctionNum1, SDIOSLV_TRANSFER_DATA_WINDOW,
                                                  g_dataBlockRead, transferCount / 32U,
                                                  SDIO_EXTEND_CMD_BLOCK_MODE_MASK))
    {
        return kStatus_Fail;
    }

    while (true)
    {
    }
}
