/*
 * Copyright 2025 NXP
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
/* Fix MISRA_C-2012 Rule 17.7. */
#define LOG_INFO (void)PRINTF
#define DLC (8)
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
flexcan_handle_t flexcanHandle;
volatile bool txComplete = false;
volatile bool rxComplete = false;
flexcan_mb_transfer_t txXfer, rxXfer;
flexcan_frame_t txframe, rxframe;
uint8_t node_type;

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief FlexCAN Call Back function
 */
static FLEXCAN_CALLBACK(flexcan_callback)
{
    switch (status)
    {
        case kStatus_FLEXCAN_RxIdle:
            if (RX_MESSAGE_BUFFER_NUM == result)
            {
                /* Receive next Data Frame or Remote Request Frame. */
                rxXfer.mbIdx = (uint8_t)RX_MESSAGE_BUFFER_NUM;
                rxXfer.frame = &rxframe;
                (void)FLEXCAN_TransferReceiveNonBlocking(EXAMPLE_CAN, &flexcanHandle, &rxXfer);

                rxComplete = true;
            }
            break;
        case kStatus_FLEXCAN_TxIdle:
            if (TX_MESSAGE_BUFFER_NUM == result)
            {
                txComplete = true;
            }
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
    flexcan_config_t flexcanConfig;
    flexcan_rx_mb_config_t rxMbConfig;
    
    /* Initialize board hardware. */
    BOARD_InitHardware();

    PRINTF("MCUX SDK version: %s\r\n", MCUXSDK_VERSION_FULL_STR);

    LOG_INFO("******* FLEXCAN RTR Non-Blocking EXAMPLE *******\r\n");
    LOG_INFO("      Message format: Standard (11 bit id)\r\n");
    LOG_INFO("      Message buffer %d used for Rx.\r\n", RX_MESSAGE_BUFFER_NUM);
    LOG_INFO("      Message buffer %d used for Tx.\r\n", TX_MESSAGE_BUFFER_NUM);
    LOG_INFO("      Interrupt Mode: Enabled\r\n");
    LOG_INFO("************************************************\r\n\r\n");

    do
    {
        LOG_INFO("Please select local node as A or B:\r\n");
        LOG_INFO("Note: Node B should start first.\r\n");
        LOG_INFO("Node:");
        node_type = GETCHAR();
        LOG_INFO("%c", node_type);
        LOG_INFO("\r\n");
    } while ((node_type != 'A') && (node_type != 'B') && (node_type != 'a') && (node_type != 'b'));

    /* Get FlexCAN module default Configuration. */
    /*
     * flexcanConfig.clkSrc                 = kFLEXCAN_ClkSrc0;
     * flexcanConfig.bitRate                = 1000000U;
     * flexcanConfig.bitRateFD              = 2000000U;
     * flexcanConfig.maxMbNum               = 16;
     * flexcanConfig.enableLoopBack         = false;
     * flexcanConfig.enableSelfWakeup       = false;
     * flexcanConfig.enableIndividMask      = false;
     * flexcanConfig.disableSelfReception   = false;
     * flexcanConfig.enableListenOnlyMode   = false;
     * flexcanConfig.enableDoze             = false;
     */
    FLEXCAN_GetDefaultConfig(&flexcanConfig);

    flexcanConfig.bitRate = 500000U;
    /* 
     * flexcanConfig.enableRemoteRequestFrameStored = true:
     * Frame's ID is compared to the IDs of the receive mailboxes with the CODE field configured as
     * 0b0100, 0b0010, 0b0110. Message buffer will store the remote frame in the same fashion of a
     * data frame. No automatic remote response frame will be generated. User need to setup another
     * message buffer to respond remote request.
     */
    flexcanConfig.enableRemoteRequestFrameStored = true;

    /* Disable Self-Reception to prevent RTR frame transmitted by Tx MB received by Rx MB.  */
    flexcanConfig.disableSelfReception = true;

#if defined(EXAMPLE_CAN_CLK_SOURCE)
    flexcanConfig.clkSrc = EXAMPLE_CAN_CLK_SOURCE;
#endif

#if (defined(USE_IMPROVED_TIMING_CONFIG) && USE_IMPROVED_TIMING_CONFIG)
    flexcan_timing_config_t timing_config;
    memset(&timing_config, 0, sizeof(flexcan_timing_config_t));
    if (FLEXCAN_CalculateImprovedTimingValues(EXAMPLE_CAN, flexcanConfig.bitRate, EXAMPLE_CAN_CLK_FREQ, &timing_config))
    {
        /* Update the improved timing configuration*/
        memcpy(&(flexcanConfig.timingConfig), &timing_config, sizeof(flexcan_timing_config_t));
    }
    else
    {
        LOG_INFO("No found Improved Timing Configuration. Just used default configuration\r\n\r\n");
    }
#endif

    FLEXCAN_Init(EXAMPLE_CAN, &flexcanConfig, EXAMPLE_CAN_CLK_FREQ);

    /* Create FlexCAN handle structure and set call back function. */
    FLEXCAN_TransferCreateHandle(EXAMPLE_CAN, &flexcanHandle, flexcan_callback, NULL);

    if ((node_type == 'A') || (node_type == 'a'))
    {
        /* Setup Tx Message Buffer to send Remote Request Frame. */
        FLEXCAN_SetTxMbConfig(EXAMPLE_CAN, TX_MESSAGE_BUFFER_NUM, true);

        /* Setup Rx Message Buffer to receive Data Frame. */
        rxMbConfig.format = kFLEXCAN_FrameFormatStandard;
        rxMbConfig.type   = kFLEXCAN_FrameTypeData;
        rxMbConfig.id     = FLEXCAN_ID_STD(0x321);
        FLEXCAN_SetRxMbConfig(EXAMPLE_CAN, RX_MESSAGE_BUFFER_NUM, &rxMbConfig, true);

        /* Receive Data Frame. */
        rxXfer.mbIdx = (uint8_t)RX_MESSAGE_BUFFER_NUM;
        rxXfer.frame = &rxframe;
        (void)FLEXCAN_TransferReceiveNonBlocking(EXAMPLE_CAN, &flexcanHandle, &rxXfer);

        LOG_INFO("Press any key to trigger one-shot remote request\r\n\r\n");
    }
    else
    {
        /* Setup Tx Message Buffer to send Data Frame. */
        FLEXCAN_SetTxMbConfig(EXAMPLE_CAN, TX_MESSAGE_BUFFER_NUM, true);

        /* Setup Rx Message Buffer to receive Remote Request frame. */
        rxMbConfig.format = kFLEXCAN_FrameFormatStandard;
        rxMbConfig.type   = kFLEXCAN_FrameTypeRemote;
        rxMbConfig.id     = FLEXCAN_ID_STD(0x321);
        FLEXCAN_SetRxMbConfig(EXAMPLE_CAN, RX_MESSAGE_BUFFER_NUM, &rxMbConfig, true);

        /* Receive Remote Request Frame. */
        rxXfer.mbIdx = (uint8_t)RX_MESSAGE_BUFFER_NUM;
        rxXfer.frame = &rxframe;
        (void)FLEXCAN_TransferReceiveNonBlocking(EXAMPLE_CAN, &flexcanHandle, &rxXfer);

        txframe.dataWord0 = 0;
        txframe.dataWord1 = 0x55;

        LOG_INFO("Start to wait request from Node A\r\n\r\n");
    }

    while (true)
    {
        if ((node_type == 'A') || (node_type == 'a'))
        {
            GETCHAR();

            /* Transmit Remote Request Frame. */
            txframe.id     = FLEXCAN_ID_STD(0x321);
            txframe.format = (uint8_t)kFLEXCAN_FrameFormatStandard;
            txframe.type   = (uint8_t)kFLEXCAN_FrameTypeRemote;
            txframe.length = (uint8_t)DLC;
            txXfer.mbIdx   = (uint8_t)TX_MESSAGE_BUFFER_NUM;
            txXfer.frame   = &txframe;
            (void)FLEXCAN_TransferRemoteRequestNonBlocking(EXAMPLE_CAN, &flexcanHandle, &txXfer);

            while (!txComplete)
            {
            };
            txComplete = false;

            LOG_INFO("Remote request message sent\r\n");

            /* Wait Data Frame. */
            while (!rxComplete)
            {
            };
            rxComplete = false;

            LOG_INFO("Response message word0: 0x%x, word1: 0x%x\r\n", rxframe.dataWord0, rxframe.dataWord1);
            LOG_INFO("Response message Timestamp: %d\r\n\r\n", rxframe.timestamp);
        }
        else
        {
            /* Receive Remote Request Frame. */
            while (!rxComplete)
            {
            };
            rxComplete = false;

            LOG_INFO("Received remote request from Node A\r\n");

            /* Transmit Data Frame. */
            txframe.id     = FLEXCAN_ID_STD(0x321);
            txframe.format = (uint8_t)kFLEXCAN_FrameFormatStandard;
            txframe.type   = (uint8_t)kFLEXCAN_FrameTypeData;
            txframe.length = (uint8_t)DLC;
            txXfer.mbIdx   = (uint8_t)TX_MESSAGE_BUFFER_NUM;
            txXfer.frame   = &txframe;
            (void)FLEXCAN_TransferSendNonBlocking(EXAMPLE_CAN, &flexcanHandle, &txXfer);

            while (!txComplete)
            {
            };
            txComplete = false;

            txframe.dataWord0++;

            LOG_INFO("Response message sent\r\n");
            LOG_INFO("Update next response message.\r\n\r\n");
        }
    }
}
