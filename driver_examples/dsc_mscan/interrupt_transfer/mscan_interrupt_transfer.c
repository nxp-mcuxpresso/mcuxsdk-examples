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

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile bool txComplete = false;
volatile bool rxComplete = false;
mscan_handle_t mscanHandle;
mscan_mb_transfer_t txXfer, rxXfer;
mscan_frame_t txRxFrame;
uint32_t txIdentifier;
uint32_t rxIdentifier;

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief MSCAN Call Back function
 */
static void mscan_callback(CAN_Type *base, mscan_handle_t *handle, status_t status, void *userData)
{
    switch (status)
    {
        /* Process MSCAN Rx event. */
        case kStatus_MSCAN_RxIdle:
            rxComplete = true;
            break;

        /* Process MSCAN Tx event. */
        case kStatus_MSCAN_TxIdle:
            txComplete = true;
            break;

        default:
            break;
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    mscan_config_t mscanConfig;
    uint8_t node_type;

    /* Initialize board hardware. */
    BOARD_InitHardware();

    PRINTF("********* MSCAN Interrupt EXAMPLE *********\r\n");
    PRINTF("    Message format: Standard (11 bit id)\r\n");
    PRINTF("    Interrupt Mode: Enabled\r\n");
    PRINTF("    Operation Mode: TX and RX --> Normal\r\n");
    PRINTF("*********************************************\r\n\r\n");

    do
    {
        PRINTF("Please select local node as A or B:\r\n");
        PRINTF("Note: Node B should start first.\r\n");
        PRINTF("Node:");
        node_type = GETCHAR();
        PRINTF("%c", node_type);
        PRINTF("\r\n");
    } while ((node_type != 'A') && (node_type != 'B') && (node_type != 'a') && (node_type != 'b'));

    /* Select mailbox ID. */
    if ((node_type == 'A') || (node_type == 'a'))
    {
        txIdentifier = 0x321;
        rxIdentifier = 0x123;
    }
    else
    {
        txIdentifier = 0x123;
        rxIdentifier = 0x321;
    }

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
    mscanConfig.filterConfig.u32IDAR0 = MSCAN_RX_MB_STD_MASK(rxIdentifier);
    mscanConfig.filterConfig.u32IDAR1 = MSCAN_RX_MB_STD_MASK(rxIdentifier);
    /* To receive standard identifiers in 32-bit filter mode, program the last three bits ([2:0]) in the mask
       registers CANIDMR1 and CANIDMR5 to don't care. */
    mscanConfig.filterConfig.u32IDMR0 = 0x00070000;
    mscanConfig.filterConfig.u32IDMR1 = 0x00070000;

    /* Initialize MSCAN module, note that when set mscanConfig.enableTimingCalc to false, MSCAN_Init can accepting
     * user's precise timing values as mscanConfig.timingConfig */
    MSCAN_Init(EXAMPLE_MSCAN, &mscanConfig);

    /* Create MSCAN handle structure and set call back function. */
    MSCAN_TransferCreateHandle(EXAMPLE_MSCAN, &mscanHandle, mscan_callback, NULL);

    if ((node_type == 'A') || (node_type == 'a'))
    {
        PRINTF("Press any key to trigger one-shot transmission\r\n\r\n");
        txRxFrame.dataByte0 = 0x0;
    }
    else
    {
        PRINTF("Start to Wait data from Node A\r\n\r\n");
    }

    while (1)
    {
        if ((node_type == 'A') || (node_type == 'a'))
        {
            GETCHAR();

            /* Prepare Tx txRxFrame for sending. */
            txRxFrame.ID_Type.ID = txIdentifier;
            txRxFrame.format     = kMSCAN_FrameFormatStandard;
            txRxFrame.type       = kMSCAN_FrameTypeData;
            txRxFrame.DLR        = 1;

            /* Send data through Tx Message Buffer. */
            txXfer.frame = &txRxFrame;
            txXfer.mask  = kMSCAN_TxEmptyInterruptEnable;
            MSCAN_TransferSendNonBlocking(EXAMPLE_MSCAN, &mscanHandle, &txXfer);

            while (!txComplete)
            {
            };
            txComplete = false;

            /* Start receive data through Rx Message Buffer. */
            rxXfer.frame = &txRxFrame;
            rxXfer.mask  = kMSCAN_RxFullInterruptEnable;
            MSCAN_TransferReceiveNonBlocking(EXAMPLE_MSCAN, &mscanHandle, &rxXfer);

            /* Wait until Rx MB full. */
            while (!rxComplete)
            {
            };
            rxComplete = false;
            PRINTF("Rx MB ID: 0x%lx, Tx MB data: 0x%x\r\n", txRxFrame.ID_Type.ID, txRxFrame.dataByte0);
            PRINTF("Press any key to trigger the next transmission!\r\n\r\n");
            txRxFrame.dataByte0++;
        }
        else
        {
            /* Start receive data through Rx Message Buffer. */
            rxXfer.frame = &txRxFrame;
            rxXfer.mask  = kMSCAN_RxFullInterruptEnable;
            MSCAN_TransferReceiveNonBlocking(EXAMPLE_MSCAN, &mscanHandle, &rxXfer);

            /* Wait until Rx receive full. */
            while (!rxComplete)
            {
            };
            rxComplete = false;

            PRINTF("Rx MB ID: 0x%lx, Rx MB data: 0x%x\r\n", txRxFrame.ID_Type.ID, txRxFrame.dataByte0);

            /* Send data through Tx Message Buffer. */
            txRxFrame.ID_Type.ID = txIdentifier;
            txXfer.frame         = &txRxFrame;
            txXfer.mask          = kMSCAN_TxEmptyInterruptEnable;
            MSCAN_TransferSendNonBlocking(EXAMPLE_MSCAN, &mscanHandle, &txXfer);

            while (!txComplete)
            {
            };
            txComplete = false;
            PRINTF("Wait Node A to trigger the next transmission!\r\n\r\n");
        }
    }
}
