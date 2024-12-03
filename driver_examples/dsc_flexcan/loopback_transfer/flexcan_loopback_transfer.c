/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "fsl_flexcan.h"
#include "board.h"
#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Define the default Data Length Code (DLC) of TX/RX frame. The correspondence between DLC value and frame valid data
 * in bytes is as follows (classic CAN DLC available range is 0 ~ 8, CAN FD DLC available range is 0 ~ 15):
 *      DLC      Valid data in bytes
 *      0 ~ 8    0 ~ 8
 *      9        12
 *      10       16
 *      11       20
 *      12       24
 *      13       32
 *      14       48
 *      15       64
 */
#ifndef DLC
#if (defined(USE_CANFD) && USE_CANFD)
#define DLC (15U)
#else
#define DLC (8U)
#endif
#endif
/* Fix MISRA_C-2012 Rule 17.7. */
#define LOG_INFO (void)PRINTF
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile bool g_bIsTxComplete = false;
volatile bool g_bIsRxComplete = false;
flexcan_handle_t g_sFlexcanHandle;
flexcan_mb_transfer_t g_sTxXfer, g_sRxXfer;
#if (defined(USE_CANFD) && USE_CANFD)
flexcan_fd_frame_t g_sTxFrame, g_sRxFrame;
#else
flexcan_frame_t g_sTxFrame, g_sRxFrame;
#endif

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief FlexCAN Call Back function
 */
static void flexcan_callback(flexcan_handle_t *handle, status_t status, uint32_t result, void *userData)
{
    switch (status)
    {
        /* Process FlexCAN Rx event. */
        case kStatus_FLEXCAN_RxIdle:
            if (RX_MESSAGE_BUFFER_NUM == result)
            {
                g_bIsRxComplete = true;
            }
            break;

        /* Process FlexCAN Tx event. */
        case kStatus_FLEXCAN_TxIdle:
            if (TX_MESSAGE_BUFFER_NUM == result)
            {
                g_bIsTxComplete = true;
            }
            break;

        default:
            break;
    }
}

static void DEMO_InitCan(void)
{
    /* Get default FlexCAN module configuration base on the selected CAN engine clock source. */
    flexcan_config_t sFlexcanConfig;
    FLEXCAN_GetDefaultConfig(&sFlexcanConfig, EXAMPLE_CAN_CLK_FREQ);
#if (defined(USE_CANFD) && USE_CANFD)
    /* Get default CAN FD part configuration. */
    flexcan_fd_config_t sFDConfig;
    FLEXCAN_GetFDDefaultConfig(&sFlexcanConfig, &sFDConfig);
#endif

    /* Enable loop back feature */
    sFlexcanConfig.bEnableLoopBack = true;
    /* Change CAN engine clock source if needed */
#if defined(EXAMPLE_CAN_CLK_SOURCE)
    sFlexcanConfig.eClkSrc = EXAMPLE_CAN_CLK_SOURCE;
#endif

    /* Init Flexcan, note that when set sFlexcanConfig.bEnableTimingCalc to false, FLEXCAN_Init() can accept user's
     * precise bit timing values in sFlexcanConfig.sTimingConfig and sFDConfig.sTimingConfig. */
    FLEXCAN_Init(EXAMPLE_CAN, &sFlexcanConfig);
}

/*!
 * @brief Main function
 */
int main(void)
{
    flexcan_rx_mb_config_t sRxMbConfig;

    /* Initialize board hardware. */
    BOARD_InitHardware();

    LOG_INFO("\r\n==FlexCAN loopback transfer example -- Start.==\r\n\r\n");

    DEMO_InitCan();

    /* Setup Rx Message Buffer. */
    sRxMbConfig.eFormat = kFLEXCAN_FrameFormatStandard;
    sRxMbConfig.eType   = kFLEXCAN_FrameTypeData;
    sRxMbConfig.u32Id   = FLEXCAN_ID_STD(0x123);

#if (defined(USE_CANFD) && USE_CANFD)
    FLEXCAN_SetFDRxMbConfig(EXAMPLE_CAN, RX_MESSAGE_BUFFER_NUM, &sRxMbConfig, true);
#else
    FLEXCAN_SetRxMbConfig(EXAMPLE_CAN, RX_MESSAGE_BUFFER_NUM, &sRxMbConfig, true);
#endif

/* Setup Tx Message Buffer. */
#if (defined(USE_CANFD) && USE_CANFD)
    FLEXCAN_SetFDTxMbConfig(EXAMPLE_CAN, TX_MESSAGE_BUFFER_NUM, true);
#else
    FLEXCAN_SetTxMbConfig(EXAMPLE_CAN, TX_MESSAGE_BUFFER_NUM, true);
#endif

    /* Create FlexCAN handle structure and set call back function. */
    FLEXCAN_TransferCreateHandle(EXAMPLE_CAN, &g_sFlexcanHandle, flexcan_callback, NULL);

    /* Start receive data through Rx Message Buffer. */
    g_sRxXfer.u8MsgBufIdx = (uint8_t)RX_MESSAGE_BUFFER_NUM;
#if (defined(USE_CANFD) && USE_CANFD)
    g_sRxXfer.psFrameFD = &g_sRxFrame;
    (void)FLEXCAN_TransferFDReceiveNonBlocking(&g_sFlexcanHandle, &g_sRxXfer);
#else
    g_sRxXfer.psFrame = &g_sRxFrame;
    (void)FLEXCAN_TransferReceiveNonBlocking(&g_sFlexcanHandle, &g_sRxXfer);
#endif

    /* Prepare Tx Frame for sending. */
    g_sTxFrame.bitFormat  = (uint8_t)kFLEXCAN_FrameFormatStandard;
    g_sTxFrame.bitType    = (uint8_t)kFLEXCAN_FrameTypeData;
    g_sTxFrame.bitsId     = FLEXCAN_ID_STD(0x123);
    g_sTxFrame.bitsLength = (uint8_t)DLC;
#if (defined(USE_CANFD) && USE_CANFD)
    g_sTxFrame.bitEdl = 1U;
    g_sTxFrame.bitBrs = 1U;
#endif
#if (defined(USE_CANFD) && USE_CANFD)
    uint8_t i = 0;
    for (i = 0; i < ((DLC_LENGTH_DECODE(DLC) + 3U) / 4U); i++)
    {
        g_sTxFrame.u32DataWord[i] = i;
    }
#else
    g_sTxFrame.u32DataWord0 = CAN_WORD0_DATA_BYTE_0(0x11) | CAN_WORD0_DATA_BYTE_1(0x22) | CAN_WORD0_DATA_BYTE_2(0x33) |
                              CAN_WORD0_DATA_BYTE_3(0x44);
    g_sTxFrame.u32DataWord1 = CAN_WORD1_DATA_BYTE_4(0x55) | CAN_WORD1_DATA_BYTE_5(0x66) | CAN_WORD1_DATA_BYTE_6(0x77) |
                              CAN_WORD1_DATA_BYTE_7(0x88);
#endif

    LOG_INFO("Send message from MB%d to MB%d\r\n", TX_MESSAGE_BUFFER_NUM, RX_MESSAGE_BUFFER_NUM);
#if (defined(USE_CANFD) && USE_CANFD)
    for (i = 0; i < ((DLC_LENGTH_DECODE(DLC) + 3U) / 4U); i++)
    {
        LOG_INFO("tx word%d = 0x%lx\r\n", i, g_sTxFrame.u32DataWord[i]);
    }
#else
    LOG_INFO("tx word0 = 0x%lx\r\n", g_sTxFrame.u32DataWord0);
    LOG_INFO("tx word1 = 0x%lx\r\n", g_sTxFrame.u32DataWord1);
#endif

    /* Send data through Tx Message Buffer. */
    g_sTxXfer.u8MsgBufIdx = (uint8_t)TX_MESSAGE_BUFFER_NUM;
#if (defined(USE_CANFD) && USE_CANFD)
    g_sTxXfer.psFrameFD = &g_sTxFrame;
    (void)FLEXCAN_TransferFDSendNonBlocking(&g_sFlexcanHandle, &g_sTxXfer);
#else
    g_sTxXfer.psFrame = &g_sTxFrame;
    (void)FLEXCAN_TransferSendNonBlocking(&g_sFlexcanHandle, &g_sTxXfer);
#endif

    /* Waiting for Rx Message finish. */
    while ((!g_bIsRxComplete) || (!g_bIsTxComplete))
    {
    };

    LOG_INFO("\r\nReceived message from MB%d\r\n", RX_MESSAGE_BUFFER_NUM);
#if (defined(USE_CANFD) && USE_CANFD)
    for (i = 0; i < ((DLC_LENGTH_DECODE(DLC) + 3U) / 4U); i++)
    {
        LOG_INFO("rx word%d = 0x%lx\r\n", i, g_sRxFrame.u32DataWord[i]);
    }
#else
    LOG_INFO("rx word0 = 0x%lx\r\n", g_sRxFrame.u32DataWord0);
    LOG_INFO("rx word1 = 0x%lx\r\n", g_sRxFrame.u32DataWord1);
#endif

    LOG_INFO("\r\n==FlexCAN loopback transfer example -- Finish.==\r\n");

    while (true)
    {
    }
}
