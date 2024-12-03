/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "fsl_flexcan.h"
#include "fsl_flexcan_edma.h"
#include "board.h"
#include "app.h"
#if defined(FSL_FEATURE_SOC_DMAMUX_COUNT) && FSL_FEATURE_SOC_DMAMUX_COUNT
#include "fsl_dmamux.h"
#endif
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Fix MISRA_C-2012 Rule 17.7. */
#define LOG_INFO (void)PRINTF
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile bool txComplete = false;
volatile bool rxComplete = false;
flexcan_handle_t flexcanHandle;
flexcan_edma_handle_t flexcanEdmaHandle;
edma_handle_t flexcanRxFifoEdmaHandle;
flexcan_mb_transfer_t txXfer;
flexcan_fifo_transfer_t rxFifoXfer;
flexcan_frame_t g_sTxFrame;
AT_NONCACHEABLE_SECTION(flexcan_frame_t g_sRxFrame);

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief FlexCAN Call Back function
 */
static void flexcan_callback(CAN_Type *base, flexcan_handle_t *handle, status_t status, uint32_t result, void *userData)
{
    /* Process FlexCAN Tx event. */
    if ((kStatus_FLEXCAN_TxIdle == status) && (TX_MESSAGE_BUFFER_NUM == result))
    {
        txComplete = true;
    }
}

/*!
 * @brief FlexCAN DMA Call Back function
 */
static void flexcan_dma_callback(CAN_Type *base, flexcan_edma_handle_t *psHandle, status_t status, void *userData)
{
    /* Process FlexCAN Rx event. */
    if (kStatus_FLEXCAN_RxFifoIdle == status)
    {
        rxComplete = true;
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    edma_config_t edmaConfig;
    flexcan_config_t sFlexcanConfig;
    flexcan_rx_fifo_config_t rxFifoConfig;
    uint32_t rxFifoFilter[] = {
        FLEXCAN_RX_FIFO_STD_FILTER_TYPE_A(0x321, 0, 0), FLEXCAN_RX_FIFO_STD_FILTER_TYPE_A(0x321, 1, 0),
        FLEXCAN_RX_FIFO_STD_FILTER_TYPE_A(0x123, 0, 0), FLEXCAN_RX_FIFO_STD_FILTER_TYPE_A(0x123, 1, 0)};
    uint8_t i;

    /* Initialize board hardware. */
    BOARD_InitHardware();

    LOG_INFO("\r\n==FlexCAN loopback edma example -- Start.==\r\n\r\n");

    /* Init FlexCAN module. */
    /*
     * sFlexcanConfig.clkSrc                 = kFLEXCAN_ClkSrc0;
     * sFlexcanConfig.u32BaudRateBps               = 1000000U;
     * sFlexcanConfig.u32FDBaudRateBps             = 2000000U;
     * sFlexcanConfig.maxMbNum               = 16;
     * sFlexcanConfig.bEnableLoopBack         = false;
     * sFlexcanConfig.bEnableSelfWakeup       = false;
     * sFlexcanConfig.bEnableIndividMask      = false;
     * sFlexcanConfig.disableSelfReception   = false;
     * sFlexcanConfig.bEnableListenOnlyMode   = false;
     * sFlexcanConfig.bEnableDoze             = false;
     */
    FLEXCAN_GetDefaultConfig(&sFlexcanConfig, EXAMPLE_CAN_CLK_FREQ);

#if defined(EXAMPLE_CAN_CLK_SOURCE)
    sFlexcanConfig.clkSrc = EXAMPLE_CAN_CLK_SOURCE;
#endif

    sFlexcanConfig.bEnableLoopBack = true;

#if (defined(USE_IMPROVED_TIMING_CONFIG) && USE_IMPROVED_TIMING_CONFIG)
    flexcan_timing_config_t timing_config;
    memset(&timing_config, 0, sizeof(flexcan_timing_config_t));
    if (FLEXCAN_CalculateImprovedTimingValues(sFlexcanConfig.u32BaudRateBps, EXAMPLE_CAN_CLK_FREQ, &timing_config))
    {
        /* Update the improved timing configuration*/
        memcpy(&(sFlexcanConfig.sTimingConfig), &timing_config, sizeof(flexcan_timing_config_t));
    }
    else
    {
        LOG_INFO("No found Improved Timing Configuration. Just used default configuration\r\n\r\n");
    }
#endif

    FLEXCAN_Init(EXAMPLE_CAN, &sFlexcanConfig);

#if defined(FSL_FEATURE_SOC_DMAMUX_COUNT) && FSL_FEATURE_SOC_DMAMUX_COUNT
    /* Configure DMA. */
    DMAMUX_Init(EXAMPLE_CAN_DMAMUX);
    DMAMUX_SetSource(EXAMPLE_CAN_DMAMUX, (dmamux_dma_channel_t)EXAMPLE_CAN_DMA_CHANNEL, EXAMPLE_CAN_DMA_REQUEST);
    DMAMUX_EnableChannel(EXAMPLE_CAN_DMAMUX, (dmamux_dma_channel_t)EXAMPLE_CAN_DMA_CHANNEL);
#endif

    /*
     * edmaConfig.enableRoundRobinArbitration = false;
     * edmaConfig.enableHaltOnError = true;
     * edmaConfig.enableContinuousLinkMode = false;
     * edmaConfig.enableDebugMode = false;
     */
    EDMA_GetDefaultConfig(&edmaConfig);
    EDMA_Init(EXAMPLE_CAN_DMA, &edmaConfig);

    /* Create EDMA handle. */
    EDMA_CreateHandle(&flexcanRxFifoEdmaHandle, EXAMPLE_CAN_DMA, EXAMPLE_CAN_DMA_CHANNEL);

    /* Setup Tx Message Buffer. */
    FLEXCAN_SetTxMbConfig(EXAMPLE_CAN, TX_MESSAGE_BUFFER_NUM, true);

    /* Setup Rx FIFO. */
    rxFifoConfig.pu32IdFilterTable = rxFifoFilter;
    rxFifoConfig.eIdFilterType     = kFLEXCAN_RxFifoFilterTypeA;
    rxFifoConfig.u8IdFilterNum     = sizeof(rxFifoFilter) / sizeof(rxFifoFilter[0]);
    rxFifoConfig.ePriority         = kFLEXCAN_RxFifoPriorityHigh;
    FLEXCAN_SetRxFifoConfig(EXAMPLE_CAN, &rxFifoConfig, true);

    /* Create FlexCAN handle structure and set call back function. */
    FLEXCAN_TransferCreateHandle(EXAMPLE_CAN, &flexcanHandle, flexcan_callback, NULL);

    /* Create FlexCAN EDMA handle structure and set call back function. */
    FLEXCAN_TransferCreateHandleEDMA(EXAMPLE_CAN, &flexcanEdmaHandle, flexcan_dma_callback, NULL,
                                     &flexcanRxFifoEdmaHandle);

    /* Send first message through Tx Message Buffer. */
    g_sTxFrame.bitFormat    = (uint8_t)kFLEXCAN_FrameFormatStandard;
    g_sTxFrame.bitType      = (uint8_t)kFLEXCAN_FrameTypeData;
    g_sTxFrame.bitsId       = FLEXCAN_ID_STD(0x123);
    g_sTxFrame.bitsLength   = 8U;
    g_sTxFrame.u32DataWord0 = CAN_WORD0_DATA_BYTE_0(0x11) | CAN_WORD0_DATA_BYTE_1(0x11) | CAN_WORD0_DATA_BYTE_2(0x11) |
                              CAN_WORD0_DATA_BYTE_3(0x11);
    g_sTxFrame.u32DataWord1 = CAN_WORD1_DATA_BYTE_4(0x11) | CAN_WORD1_DATA_BYTE_5(0x11) | CAN_WORD1_DATA_BYTE_6(0x11) |
                              CAN_WORD1_DATA_BYTE_7(0x11);

    txXfer.psFrame     = &g_sTxFrame;
    txXfer.u8MsgBufIdx = (uint8_t)TX_MESSAGE_BUFFER_NUM;
    (void)FLEXCAN_TransferSendNonBlocking(EXAMPLE_CAN, &flexcanHandle, &txXfer);
    while (!txComplete)
    {
    }
    txComplete = false;
    LOG_INFO("Send Msg1 to Rx FIFO: word0 = 0x%lx, word1 = 0x%lx.\r\n", g_sTxFrame.u32DataWord0,
             g_sTxFrame.u32DataWord1);

    /* Send second message through Tx Message Buffer. */
    g_sTxFrame.u32DataWord0 = CAN_WORD0_DATA_BYTE_0(0x22) | CAN_WORD0_DATA_BYTE_1(0x22) | CAN_WORD0_DATA_BYTE_2(0x22) |
                              CAN_WORD0_DATA_BYTE_3(0x22);
    g_sTxFrame.u32DataWord1 = CAN_WORD1_DATA_BYTE_4(0x22) | CAN_WORD1_DATA_BYTE_5(0x22) | CAN_WORD1_DATA_BYTE_6(0x22) |
                              CAN_WORD1_DATA_BYTE_7(0x22);
    (void)FLEXCAN_TransferSendNonBlocking(EXAMPLE_CAN, &flexcanHandle, &txXfer);
    while (!txComplete)
    {
    }
    txComplete = false;
    LOG_INFO("Send Msg2 to Rx FIFO: word0 = 0x%lx, word1 = 0x%lx.\r\n", g_sTxFrame.u32DataWord0,
             g_sTxFrame.u32DataWord1);

    /* Send third message through Tx Message Buffer. */
    txXfer.psFrame          = &g_sTxFrame;
    txXfer.u8MsgBufIdx      = (uint8_t)TX_MESSAGE_BUFFER_NUM;
    g_sTxFrame.u32DataWord0 = CAN_WORD0_DATA_BYTE_0(0x33) | CAN_WORD0_DATA_BYTE_1(0x33) | CAN_WORD0_DATA_BYTE_2(0x33) |
                              CAN_WORD0_DATA_BYTE_3(0x33);
    g_sTxFrame.u32DataWord1 = CAN_WORD1_DATA_BYTE_4(0x33) | CAN_WORD1_DATA_BYTE_5(0x33) | CAN_WORD1_DATA_BYTE_6(0x33) |
                              CAN_WORD1_DATA_BYTE_7(0x33);
    (void)FLEXCAN_TransferSendNonBlocking(EXAMPLE_CAN, &flexcanHandle, &txXfer);
    while (!txComplete)
    {
    }
    txComplete = false;
    LOG_INFO("Send Msg3 to Rx FIFO: word0 = 0x%lx, word1 = 0x%lx.\r\n", g_sTxFrame.u32DataWord0,
             g_sTxFrame.u32DataWord1);

    /* Send fourth message through Tx Message Buffer. */
    txXfer.psFrame          = &g_sTxFrame;
    txXfer.u8MsgBufIdx      = (uint8_t)TX_MESSAGE_BUFFER_NUM;
    g_sTxFrame.u32DataWord0 = CAN_WORD0_DATA_BYTE_0(0x44) | CAN_WORD0_DATA_BYTE_1(0x44) | CAN_WORD0_DATA_BYTE_2(0x44) |
                              CAN_WORD0_DATA_BYTE_3(0x44);
    g_sTxFrame.u32DataWord1 = CAN_WORD1_DATA_BYTE_4(0x44) | CAN_WORD1_DATA_BYTE_5(0x44) | CAN_WORD1_DATA_BYTE_6(0x44) |
                              CAN_WORD1_DATA_BYTE_7(0x44);
    (void)FLEXCAN_TransferSendNonBlocking(EXAMPLE_CAN, &flexcanHandle, &txXfer);
    while (!txComplete)
    {
    }
    txComplete = false;
    LOG_INFO("Send Msg4 to Rx FIFO: word0 = 0x%lx, word1 = 0x%lx.\r\n\r\n", g_sTxFrame.u32DataWord0,
             g_sTxFrame.u32DataWord1);

    /* Receive data through Rx FIFO. */
    rxFifoXfer.psFrame = &g_sRxFrame;
    for (i = 0; i < 4; i++)
    {
        (void)FLEXCAN_TransferReceiveFifoEDMA(EXAMPLE_CAN, &flexcanEdmaHandle, &rxFifoXfer);
        while (!rxComplete)
        {
        }
        rxComplete = false;

        LOG_INFO("Receive Msg%d from FIFO: word0 = 0x%lx, word1 = 0x%lx, ID Filter Hit%d.\r\n", i + 1,
                 g_sRxFrame.u32DataWord0, g_sRxFrame.u32DataWord1, g_sRxFrame.bitsIdHit);
    }

    LOG_INFO("\r\n==FlexCAN loopback EDMA example -- Finish.==\r\n");

    while (true)
    {
        __WFI();
    }
}
