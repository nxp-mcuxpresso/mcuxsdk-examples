/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "fsl_mscan.h"
#include "board.h"
#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define NODE_ID1 0x801
#define MSCAN_IDMR0 \
    (CAN_RXFG_IDR3_EXT_RTR_MASK | (uint32_t)(CAN_RXFG_IDR1_EXT_SRR_MASK | CAN_RXFG_IDR1_EXT_IDE_MASK) << 16U)
#define MSCAN_IDMR1 \
    (CAN_RXFG_IDR3_EXT_RTR_MASK | (uint32_t)(CAN_RXFG_IDR1_EXT_SRR_MASK | CAN_RXFG_IDR1_EXT_IDE_MASK) << 16U)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile bool rxComplete = false;
mscan_frame_t txFrame, rxFrame;

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
int main(void)
{
    mscan_config_t mscanConfig;
    /* Initialize board hardware. */
    BOARD_InitHardware();

    PRINTF("\r\n==MSCAN loopback functional example -- Start.==\r\n\r\n");

    /* Get MSCAN module default Configuration. */
    /*
     * mscanConfig.baudRate = 500000U;
     * mscanConfig.enableTimingCalc = true;
     * mscanConfig.enableTimer = false;
     * mscanConfig.enableWakeup = false;
     * mscanConfig.clkSrc = kMSCAN_ClkSrcBus;
     * mscanConfig.enableLoopBack = false;
     * mscanConfig.enableListen = false;
     * mscanConfig.busoffrecMode = kMSCAN_BusoffrecAuto;
     * mscanConfig.filterConfig.filterMode = kMSCAN_Filter32Bit;
     */
    MSCAN_GetDefaultConfig(&mscanConfig, EXAMPLE_MSCAN_CLK_FREQ);

    /* Acceptance filter configuration. */
    mscanConfig.filterConfig.u32IDAR0 = MSCAN_RX_MB_EXT_MASK(NODE_ID1);
    mscanConfig.filterConfig.u32IDAR1 = MSCAN_RX_MB_EXT_MASK(NODE_ID1);
    mscanConfig.filterConfig.u32IDMR0 = MSCAN_IDMR0;
    mscanConfig.filterConfig.u32IDMR1 = MSCAN_IDMR1;

    /* Enable loopback mode. */
    mscanConfig.enableLoopBack = true;

    /* Initialize MSCAN module, note that when set mscanConfig.enableTimingCalc to false, MSCAN_Init can accepting
     * user's precise timing values as mscanConfig.timingConfig */
    MSCAN_Init(EXAMPLE_MSCAN, &mscanConfig);

    /* Prepare Tx Frame for sending. */
    txFrame.ID_Type.ID = NODE_ID1;
    txFrame.format     = kMSCAN_FrameFormatExtend;
    txFrame.type       = kMSCAN_FrameTypeData;
    txFrame.DLR        = 8;
    txFrame.dataWord0  = 0x12345678;
    txFrame.dataWord1  = 0x87654321;

    PRINTF("Send message!\r\n");
    PRINTF("tx word0 = 0x%lx\r\n", txFrame.dataWord0);
    PRINTF("tx word1 = 0x%lx\r\n", txFrame.dataWord1);

    /* Send data through Tx Buffer using polling function. */
    MSCAN_TransferSendBlocking(EXAMPLE_MSCAN, &txFrame);

    while (!rxComplete)
    {
        /* If new data arrived. */
        if (MSCAN_GetRxBufferFullFlag(EXAMPLE_MSCAN))
        {
            MSCAN_ReadRxMb(EXAMPLE_MSCAN, &rxFrame);
            MSCAN_ClearRxBufferFullFlag(EXAMPLE_MSCAN);
            rxComplete = true;
        }
    }

    PRINTF("\r\nReceived message!\r\n");
    PRINTF("rx word0 = 0x%lx\r\n", rxFrame.dataWord0);
    PRINTF("rx word1 = 0x%lx\r\n", rxFrame.dataWord1);

    PRINTF("\r\n==MSCAN loopback functional example -- Finish.==\r\n");

    while (1)
    {
        __WFI();
    }
}
