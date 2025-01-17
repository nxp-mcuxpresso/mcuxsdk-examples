/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"
#include "app.h"
#include "fsl_gdma.h"
#include "fsl_debug_console.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define APP_XFER_DATA_LEN 64U

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void APP_InitGdma(void);
static void APP_InitData(void);
static void APP_GdmaLLI(void);
static bool APP_VerifyData(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
static uint8_t s_gdmaData0[APP_XFER_DATA_LEN];
static uint8_t s_gdmaData1[APP_XFER_DATA_LEN];
static uint8_t s_gdmaData2[APP_XFER_DATA_LEN];
static uint8_t s_gdmaData3[APP_XFER_DATA_LEN];
static gdma_handle_t s_gdmaHandle;
static volatile bool s_gdmaXferDone = false;

/*******************************************************************************
 * Code
 ******************************************************************************/
static void APP_GdmaCallback(gdma_handle_t *handle, void *userData, uint32_t interrupts)
{
    if (0UL != (interrupts & kGDMA_TransferDoneFlag))
    {
        s_gdmaXferDone = true;
    }
}

int main(void)
{
    BOARD_InitHardware();

    PRINTF("GDMA LLI example start\r\n");

    APP_InitGdma();

    APP_InitData();

    APP_GdmaLLI();

    /* Verify the result. */
    if (true == APP_VerifyData())
    {
        PRINTF("GDMA LLI example succeed\r\n");
    }
    else
    {
        PRINTF("GDMA LLI example failed\r\n");
    }

    while (1)
    {
    }
}

static void APP_InitGdma(void)
{
    GDMA_Init(APP_GDMA);

    GDMA_CreateHandle(&s_gdmaHandle, APP_GDMA, APP_GDMA_CH);

    GDMA_SetCallback(&s_gdmaHandle, APP_GdmaCallback, NULL);
}

static void APP_InitData(void)
{
    uint32_t i;

    /* s_gdmaData0 set to 0, 1, 2, 3, ... */
    for (i = 0; i < APP_XFER_DATA_LEN; i++)
    {
        s_gdmaData0[i] = (uint8_t)i;
    }

    /* s_gdmaData1, s_gdmaData2, s_gdmaData3 set to 0. */
    memset(s_gdmaData1, 0, APP_XFER_DATA_LEN);
    memset(s_gdmaData2, 0, APP_XFER_DATA_LEN);
    memset(s_gdmaData3, 0, APP_XFER_DATA_LEN);
}

static void APP_GdmaLLI(void)
{
    gdma_channel_xfer_config_t xferConfig = {0};

    static gdma_descriptor_t desc_1_2 = {0}; /* Copy s_gdmaData1 to s_gdmaData2 */
    static gdma_descriptor_t desc_2_3 = {0}; /* Copy s_gdmaData2 to s_gdmaData3 */

    xferConfig.srcAddr               = (uint32_t)s_gdmaData0;
    xferConfig.destAddr              = (uint32_t)s_gdmaData1;
    xferConfig.linkListAddr          = (uint32_t)(&desc_1_2);
    xferConfig.ahbProt               = kGDMA_ProtPrevilegedMode;
    xferConfig.srcBurstSize          = kGDMA_BurstSize1;
    xferConfig.destBurstSize         = kGDMA_BurstSize1;
    xferConfig.srcWidth              = kGDMA_TransferWidth1Byte;
    xferConfig.destWidth             = kGDMA_TransferWidth1Byte;
    xferConfig.srcAddrInc            = true;
    xferConfig.destAddrInc           = true;
    xferConfig.transferLen           = APP_XFER_DATA_LEN;
    xferConfig.enableLinkList        = true;
    xferConfig.enableDescInterrupt   = false;
    xferConfig.stopAfterDescFinished = false;

    desc_1_2.srcAddr = (uint32_t)s_gdmaData1;
    desc_1_2.dstAddr = (uint32_t)s_gdmaData2;
    desc_1_2.lli     = GDMA_DESC_LLI(&desc_2_3, false, false);
    desc_1_2.ctrl    = GDMA_DESC_CTRL(kGDMA_ProtPrevilegedMode, true, true, kGDMA_TransferWidth1Byte,
                                   kGDMA_TransferWidth1Byte, kGDMA_BurstSize1, kGDMA_BurstSize1, APP_XFER_DATA_LEN);

    desc_2_3.srcAddr = (uint32_t)s_gdmaData2;
    desc_2_3.dstAddr = (uint32_t)s_gdmaData3;
    desc_2_3.lli     = GDMA_DESC_LLI(0UL, true, false);
    desc_2_3.ctrl    = GDMA_DESC_CTRL(kGDMA_ProtPrevilegedMode, true, true, kGDMA_TransferWidth1Byte,
                                   kGDMA_TransferWidth1Byte, kGDMA_BurstSize1, kGDMA_BurstSize1, APP_XFER_DATA_LEN);

    GDMA_SubmitTransfer(&s_gdmaHandle, &xferConfig);

    s_gdmaXferDone = false;

    GDMA_StartTransfer(&s_gdmaHandle);

    /* Wait for transfer done. */
    while (!s_gdmaXferDone)
    {
    }
}

static bool APP_VerifyData(void)
{
    return ((0 == memcmp(s_gdmaData0, s_gdmaData1, APP_XFER_DATA_LEN)) &&
            (0 == memcmp(s_gdmaData0, s_gdmaData2, APP_XFER_DATA_LEN)) &&
            (0 == memcmp(s_gdmaData0, s_gdmaData3, APP_XFER_DATA_LEN)));
}
