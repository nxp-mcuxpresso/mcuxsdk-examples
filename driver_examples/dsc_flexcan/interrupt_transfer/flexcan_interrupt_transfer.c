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

typedef enum _demo_can_node_type
{
    kDEMO_NodeA = 0U,
    kDEMO_NodeB = 1U,
} demo_can_node_type_t;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
flexcan_handle_t g_sFlexcanHandle;
volatile bool g_bIsTxComplete = false;
volatile bool g_bIsRxComplete = false;
volatile bool g_bIsWakenUp    = false;
demo_can_node_type_t g_eNodeType;
flexcan_mb_transfer_t g_sTxXfer, g_sRxXfer;
#if (defined(USE_CANFD) && USE_CANFD)
flexcan_fd_frame_t g_sFrame;
#else
flexcan_frame_t g_sFrame;
#endif
uint32_t g_u32TxIdentifier;
uint32_t g_u32RxIdentifier;

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
        case kStatus_FLEXCAN_RxIdle:
            if (RX_MESSAGE_BUFFER_NUM == result)
            {
                g_bIsRxComplete = true;
            }
            break;

        case kStatus_FLEXCAN_TxIdle:
            if (TX_MESSAGE_BUFFER_NUM == result)
            {
                g_bIsTxComplete = true;
            }
            break;

        case kStatus_FLEXCAN_WakeUp:
            g_bIsWakenUp = true;
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

    /* Change CAN engine clock source if needed */
#if defined(EXAMPLE_CAN_CLK_SOURCE)
    sFlexcanConfig.eClkSrc = EXAMPLE_CAN_CLK_SOURCE;
#endif

    /* Init Flexcan, note that when set sFlexcanConfig.bEnableTimingCalc to false, FLEXCAN_Init() can accept user's
     * precise bit timing values in sFlexcanConfig.sTimingConfig and sFDConfig.sTimingConfig. */
    FLEXCAN_Init(EXAMPLE_CAN, &sFlexcanConfig);

    /* Create FlexCAN handle structure and set call back function. */
    FLEXCAN_TransferCreateHandle(EXAMPLE_CAN, &g_sFlexcanHandle, flexcan_callback, NULL);

    /* Set Rx Masking mechanism. */
    FLEXCAN_SetRxMbGlobalMask(EXAMPLE_CAN, FLEXCAN_RX_MB_STD_MASK(g_u32RxIdentifier, 0, 0));
}

static void DEMO_DecideNodeType(void)
{
    uint8_t u8NodeType;

    do
    {
        LOG_INFO("Please select local node as A or B:\r\n");
        LOG_INFO("Note: Node B should start first.\r\n");
        LOG_INFO("Node:");
        u8NodeType = GETCHAR();
        LOG_INFO("%c", u8NodeType);
        LOG_INFO("\r\n");
    } while ((u8NodeType != 'A') && (u8NodeType != 'B') && (u8NodeType != 'a') && (u8NodeType != 'b'));

    /* Select mailbox ID. */
    if ((u8NodeType == 'A') || (u8NodeType == 'a'))
    {
        g_u32TxIdentifier = 0x321;
        g_u32RxIdentifier = 0x123;
        g_eNodeType       = kDEMO_NodeA;
    }
    else
    {
        g_u32TxIdentifier = 0x123;
        g_u32RxIdentifier = 0x321;
        g_eNodeType       = kDEMO_NodeB;
    }
}

static void DEMO_RunNodeA(void)
{
    GETCHAR();

    g_sFrame.bitsId     = FLEXCAN_ID_STD(g_u32TxIdentifier);
    g_sFrame.bitFormat  = (uint8_t)kFLEXCAN_FrameFormatStandard;
    g_sFrame.bitType    = (uint8_t)kFLEXCAN_FrameTypeData;
    g_sFrame.bitsLength = (uint8_t)DLC;
#if (defined(USE_CANFD) && USE_CANFD)
    g_sFrame.bitEdl = 1U;
    g_sFrame.bitBrs = 1U;
#endif
    g_sTxXfer.u8MsgBufIdx = (uint8_t)TX_MESSAGE_BUFFER_NUM;
#if (defined(USE_CANFD) && USE_CANFD)
    g_sTxXfer.psFrameFD = &g_sFrame;
    (void)FLEXCAN_TransferFDSendNonBlocking(&g_sFlexcanHandle, &g_sTxXfer);
#else
    g_sTxXfer.psFrame = &g_sFrame;
    (void)FLEXCAN_TransferSendNonBlocking(&g_sFlexcanHandle, &g_sTxXfer);
#endif

    while (!g_bIsTxComplete)
    {
    };
    g_bIsTxComplete = false;

    /* Start receive data through Rx Message Buffer. */
    g_sRxXfer.u8MsgBufIdx = (uint8_t)RX_MESSAGE_BUFFER_NUM;
#if (defined(USE_CANFD) && USE_CANFD)
    g_sRxXfer.psFrameFD = &g_sFrame;
    (void)FLEXCAN_TransferFDReceiveNonBlocking(&g_sFlexcanHandle, &g_sRxXfer);
#else
    g_sRxXfer.psFrame = &g_sFrame;
    (void)FLEXCAN_TransferReceiveNonBlocking(&g_sFlexcanHandle, &g_sRxXfer);
#endif

    /* Wait until Rx MB full. */
    while (!g_bIsRxComplete)
    {
    };
    g_bIsRxComplete = false;

    LOG_INFO("Rx MB ID: 0x%3lx, Rx MB data: 0x%x, Time stamp: %u\r\n", g_sFrame.bitsId >> CAN_ID_STD_SHIFT,
             g_sFrame.u8DataByte0, g_sFrame.bitsTimestamp);
    LOG_INFO("Press any key to trigger the next transmission!\r\n\r\n");
    g_sFrame.u8DataByte0++;
    g_sFrame.u8DataByte1 = 0x55;
}

static void DEMO_RunNodeB(void)
{
    /* Before this , should first make node B enter STOP mode after FlexCAN
     * initialized with enableSelfWakeup=true and Rx MB configured, then A
     * sends g_sFrame N which wakes up node B. A will continue to send g_sFrame N
     * since no acknowledgement, then B received the second g_sFrame N(In the
     * application it seems that B received the g_sFrame that woke it up which
     * is not expected as stated in the reference manual, but actually the
     * output in the terminal B received is the same second g_sFrame N). */
    if (g_bIsWakenUp)
    {
        LOG_INFO("B has been waken up!\r\n\r\n");
    }

    /* Start receive data through Rx Message Buffer. */
    g_sRxXfer.u8MsgBufIdx = (uint8_t)RX_MESSAGE_BUFFER_NUM;
#if (defined(USE_CANFD) && USE_CANFD)
    g_sRxXfer.psFrameFD = &g_sFrame;
    (void)FLEXCAN_TransferFDReceiveNonBlocking(&g_sFlexcanHandle, &g_sRxXfer);
#else
    g_sRxXfer.psFrame = &g_sFrame;
    (void)FLEXCAN_TransferReceiveNonBlocking(&g_sFlexcanHandle, &g_sRxXfer);
#endif

    /* Wait until Rx receive full. */
    while (!g_bIsRxComplete)
    {
    };
    g_bIsRxComplete = false;

    LOG_INFO("Rx MB ID: 0x%3lx, Rx MB data: 0x%x, Time stamp: %u\r\n", g_sFrame.bitsId >> CAN_ID_STD_SHIFT,
             g_sFrame.u8DataByte0, g_sFrame.bitsTimestamp);

    g_sFrame.bitsId       = FLEXCAN_ID_STD(g_u32TxIdentifier);
    g_sTxXfer.u8MsgBufIdx = (uint8_t)TX_MESSAGE_BUFFER_NUM;
#if (defined(USE_CANFD) && USE_CANFD)
    g_sFrame.bitBrs     = 1;
    g_sTxXfer.psFrameFD = &g_sFrame;
    (void)FLEXCAN_TransferFDSendNonBlocking(&g_sFlexcanHandle, &g_sTxXfer);
#else
    g_sTxXfer.psFrame = &g_sFrame;
    (void)FLEXCAN_TransferSendNonBlocking(&g_sFlexcanHandle, &g_sTxXfer);
#endif

    while (!g_bIsTxComplete)
    {
    };
    g_bIsTxComplete = false;
    LOG_INFO("Wait Node A to trigger the next transmission!\r\n\r\n");
}

static void DEMO_PrepareMsgBuf(void)
{
    flexcan_rx_mb_config_t sRxMbConfig;

    /* Setup Rx Message Buffer. */
    sRxMbConfig.eFormat = kFLEXCAN_FrameFormatStandard;
    sRxMbConfig.eType   = kFLEXCAN_FrameTypeData;
    sRxMbConfig.u32Id   = FLEXCAN_ID_STD(g_u32RxIdentifier);

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
}

/*!
 * @brief Main function
 */
int main(void)
{
    /* Initialize board hardware. */
    BOARD_InitHardware();

    LOG_INFO("********* FLEXCAN Interrupt EXAMPLE *********\r\n");
    LOG_INFO("    Message format: Standard (11 bit id)\r\n");
    LOG_INFO("    Message buffer %d used for Rx.\r\n", RX_MESSAGE_BUFFER_NUM);
    LOG_INFO("    Message buffer %d used for Tx.\r\n", TX_MESSAGE_BUFFER_NUM);
    LOG_INFO("    Interrupt Mode: Enabled\r\n");
    LOG_INFO("    Operation Mode: TX and RX --> Normal\r\n");
    LOG_INFO("*********************************************\r\n\r\n");

    DEMO_DecideNodeType();

    DEMO_InitCan();

    DEMO_PrepareMsgBuf();

    switch (g_eNodeType)
    {
        case kDEMO_NodeA:
            LOG_INFO("Press any key to trigger one-shot transmission\r\n\r\n");
            g_sFrame.u8DataByte0 = 0;
            break;

        case kDEMO_NodeB:
            LOG_INFO("Start to Wait data from Node A\r\n\r\n");

        default:
            break;
    }

    while (true)
    {
        switch (g_eNodeType)
        {
            case kDEMO_NodeA:
                DEMO_RunNodeA();
                break;
            case kDEMO_NodeB:
                DEMO_RunNodeB();

            default:
                break;
        }
    }
}
