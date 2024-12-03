/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2022 NXP
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
/* Fix MISRA_C-2012 Rule 17.7. */
#define LOG_INFO (void)PRINTF
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
/*! @brief FlexCAN Internal State. */
enum _flexcan_state
{
    kFLEXCAN_StateIdle     = 0x0, /*!< MB/RxFIFO idle.*/
    kFLEXCAN_StateRxData   = 0x1, /*!< MB receiving.*/
    kFLEXCAN_StateRxRemote = 0x2, /*!< MB receiving remote reply.*/
    kFLEXCAN_StateTxData   = 0x3, /*!< MB transmitting.*/
    kFLEXCAN_StateTxRemote = 0x4, /*!< MB transmitting remote request.*/
    kFLEXCAN_StateRxFifo   = 0x5, /*!< RxFIFO receiving.*/
};

flexcan_handle_t flexcanHandle;
volatile bool txComplete = false;
volatile bool rxComplete = false;
volatile bool wakenUp    = false;
flexcan_mb_transfer_t txXfer, rxXfer;
#if (defined(USE_CANFD) && USE_CANFD)
flexcan_fd_frame_t frame;
#else
flexcan_frame_t frame;
#endif
uint32_t txIdentifier;
uint32_t rxIdentifier;

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Check if Message Buffer interrupt is enabled and happened.
 *
 *
 * @param base FlexCAN peripheral base address.
 * @param mbIdx The FlexCAN Message Buffer index.
 */
static bool User_IsMbHasInt(CAN_Type *base, uint8_t mbIdx)
{
    /* Assertion. */
    assert(mbIdx < (uint8_t)FSL_FEATURE_FLEXCAN_HAS_MESSAGE_BUFFER_MAX_NUMBERn(base));
#if (defined(FSL_FEATURE_FLEXCAN_HAS_MORE_THAN_64_MB) && FSL_FEATURE_FLEXCAN_HAS_MORE_THAN_64_MB)
    if (mbIdx >= 64U)
    {
#if defined(CAN_IMASK3_BUF95TO64M_MASK)
        if (mbIdx < 96U)
        {
            return (base->IMASK3 & base->IFLAG3 & ((uint32_t)(1 << (mbIdx - 64U))));
        }
#endif
#if defined(CAN_IMASK4_BUF127TO96_MASK)
        else
        {
            return (base->IMASK4 & base->IFLAG4 & ((uint32_t)(1 << (mbIdx - 96U))));
        }
#else
        return false;
#endif
    }
    else if (mbIdx >= 32U)
    {
        return (base->IMASK2 & base->IFLAG2 & ((uint32_t)(1 << (mbIdx - 32U))));
    }
    else
    {
        return (base->IMASK1 & base->IFLAG1 & ((uint32_t)(1 << mbIdx)));
    }
#elif (defined(FSL_FEATURE_FLEXCAN_HAS_EXTENDED_FLAG_REGISTER)) && (FSL_FEATURE_FLEXCAN_HAS_EXTENDED_FLAG_REGISTER > 0)
    if (mbIdx >= 32U)
    {
        return (base->IMASK2 & base->IFLAG2 & ((uint32_t)(1 << (mbIdx - 32))));
    }
    else
    {
        return (base->IMASK1 & base->IFLAG1 & ((uint32_t)(1 << mbIdx)));
    }
#else
    return (base->IMASK1 & base->IFLAG1 & ((uint32_t)(1 << mbIdx)));
#endif
}
/*!
 * brief User FlexCAN IRQ handle function.
 *
 * This function handles the FlexCAN Error, the Message Buffer, and the Rx FIFO IRQ request.
 *
 * param base FlexCAN peripheral base address.
 * param handle FlexCAN handle pointer.
 */
static void User_TransferHandleIRQ(CAN_Type *base, flexcan_handle_t *handle)
{
    /* Assertion. */
    assert(handle);

    status_t status = kStatus_FLEXCAN_UnHandled;
    uint32_t result;

    /* Store Current FlexCAN Module Error and Status. */
    result = base->ESR1;

    do
    {
        /* Solve FlexCAN Error and Status Interrupt. */
        if (0U != (result & (kFLEXCAN_TxWarningIntFlag | kFLEXCAN_RxWarningIntFlag | kFLEXCAN_BusOffIntFlag |
                             kFLEXCAN_ErrorIntFlag)))
        {
            status = kStatus_FLEXCAN_ErrorStatus;

            /* Clear FlexCAN Error and Status Interrupt. */
            FLEXCAN_ClearStatusFlags(base, kFLEXCAN_TxWarningIntFlag | kFLEXCAN_RxWarningIntFlag |
                                               kFLEXCAN_BusOffIntFlag | kFLEXCAN_ErrorIntFlag);
        }
        else if (0U != (result & kFLEXCAN_WakeUpIntFlag))
        {
            status = kStatus_FLEXCAN_WakeUp;
            FLEXCAN_ClearStatusFlags(base, kFLEXCAN_WakeUpIntFlag);
        }
        /* Solve FlexCAN Rx FIFO & Message Buffer Interrupt. */
        else
        {
            /* For this implementation, we solve the Message with lowest MB index first. */
            for (result = 0; result < (uint32_t)FSL_FEATURE_FLEXCAN_HAS_MESSAGE_BUFFER_MAX_NUMBERn(base); result++)
            {
                /* Get the lowest unhandled Message Buffer */
                if (User_IsMbHasInt(base, result))
                {
                    break;
                }
            }

            /* Does not find Message to deal with. */
            if (result == (uint32_t)FSL_FEATURE_FLEXCAN_HAS_MESSAGE_BUFFER_MAX_NUMBERn(base))
            {
                break;
            }

            /* Solve Rx FIFO interrupt. */
            if ((kFLEXCAN_StateIdle != handle->rxFifoState) && ((1 << result) <= kFLEXCAN_RxFifoOverflowFlag))
            {
                switch (1 << result)
                {
                    case kFLEXCAN_RxFifoOverflowFlag:
                        status = kStatus_FLEXCAN_RxFifoOverflow;
                        break;

                    case kFLEXCAN_RxFifoWarningFlag:
                        status = kStatus_FLEXCAN_RxFifoWarning;
                        break;

                    case kFLEXCAN_RxFifoFrameAvlFlag:
                        status = FLEXCAN_ReadRxFifo(base, handle->rxFifoFrameBuf);
                        if (kStatus_Success == status)
                        {
                            status = kStatus_FLEXCAN_RxFifoIdle;
                        }
                        FLEXCAN_TransferAbortReceiveFifo(base, handle);
                        break;

                    default:
                        status = kStatus_FLEXCAN_UnHandled;
                        break;
                }
            }
            else
            {
                /* Get current State of Message Buffer. */
                switch (handle->mbState[result])
                {
                    /* Solve Rx Data Frame or Rx Remote Frame. */
                    case kFLEXCAN_StateRxData:
                    case kFLEXCAN_StateRxRemote:
#if (defined(FSL_FEATURE_FLEXCAN_HAS_FLEXIBLE_DATA_RATE) && FSL_FEATURE_FLEXCAN_HAS_FLEXIBLE_DATA_RATE)
                        if (0U != (base->MCR & CAN_MCR_FDEN_MASK))
                        {
                            status = FLEXCAN_ReadFDRxMb(base, result, handle->mbFDFrameBuf[result]);
                        }
                        else
                        {
                            status = FLEXCAN_ReadRxMb(base, result, handle->mbFrameBuf[result]);
                        }
#else
                        status = FLEXCAN_ReadRxMb(base, result, handle->mbFrameBuf[result]);
#endif
                        if (kStatus_Success == status)
                        {
                            status = kStatus_FLEXCAN_RxIdle;
                        }
#if (defined(FSL_FEATURE_FLEXCAN_HAS_FLEXIBLE_DATA_RATE) && FSL_FEATURE_FLEXCAN_HAS_FLEXIBLE_DATA_RATE)
                        if (0U != (base->MCR & CAN_MCR_FDEN_MASK))
                        {
                            FLEXCAN_TransferFDAbortReceive(base, handle, result);
                        }
                        else
                        {
                            FLEXCAN_TransferAbortReceive(base, handle, result);
                        }
#else
                        FLEXCAN_TransferAbortReceive(base, handle, result);
#endif
                        break;

                    /* Solve Tx Data Frame. */
                    case kFLEXCAN_StateTxData:
                        status = kStatus_FLEXCAN_TxIdle;
#if (defined(FSL_FEATURE_FLEXCAN_HAS_FLEXIBLE_DATA_RATE) && FSL_FEATURE_FLEXCAN_HAS_FLEXIBLE_DATA_RATE)
                        if (0U != (base->MCR & CAN_MCR_FDEN_MASK))
                        {
                            FLEXCAN_TransferFDAbortSend(base, handle, result);
                        }
                        else
                        {
                            FLEXCAN_TransferAbortSend(base, handle, result);
                        }
#else
                        FLEXCAN_TransferAbortSend(base, handle, result);
#endif
                        break;

                    /* Solve Tx Remote Frame. */
                    case kFLEXCAN_StateTxRemote:
                        handle->mbState[result] = kFLEXCAN_StateRxRemote;
                        status                  = kStatus_FLEXCAN_TxSwitchToRx;
                        break;

                    default:
                        status = kStatus_FLEXCAN_UnHandled;
                        break;
                }
            }

            /* Clear resolved Message Buffer IRQ. */
#if (defined(FSL_FEATURE_FLEXCAN_HAS_MORE_THAN_64_MB) && FSL_FEATURE_FLEXCAN_HAS_MORE_THAN_64_MB)
            if (result >= 64U)
            {
                FLEXCAN_ClearHigh64MbStatusFlags(base, (uint64_t)1U << (result - 64U));
            }
            else
            {
                FLEXCAN_ClearMbStatusFlags(base, (uint64_t)1U << result);
            }
#elif (defined(FSL_FEATURE_FLEXCAN_HAS_EXTENDED_FLAG_REGISTER)) && (FSL_FEATURE_FLEXCAN_HAS_EXTENDED_FLAG_REGISTER > 0)
            FLEXCAN_ClearMbStatusFlags(base, (uint64_t)1U << result);
#else
            FLEXCAN_ClearMbStatusFlags(base, (uint32_t)1U << result);
#endif
        }

        /* Calling Callback Function if has one. */
        if (handle->callback != NULL)
        {
            handle->callback(base, handle, status, result, handle->userData);
        }

        /* Reset return status */
        status = kStatus_FLEXCAN_UnHandled;

        /* Store Current FlexCAN Module Error and Status. */
        result = base->ESR1;
    }
#if (defined(FSL_FEATURE_FLEXCAN_HAS_MORE_THAN_64_MB) && FSL_FEATURE_FLEXCAN_HAS_MORE_THAN_64_MB)
    while ((0U != FLEXCAN_GetMbStatusFlags(base, 0xFFFFFFFFFFFFFFFFU)) ||
           (0U != FLEXCAN_GetHigh64MbStatusFlags(base, 0xFFFFFFFFFFFFFFFFU)) ||
           (0U != (result & (kFLEXCAN_TxWarningIntFlag | kFLEXCAN_RxWarningIntFlag | kFLEXCAN_BusOffIntFlag |
                             kFLEXCAN_ErrorIntFlag | kFLEXCAN_WakeUpIntFlag))));
#elif (defined(FSL_FEATURE_FLEXCAN_HAS_EXTENDED_FLAG_REGISTER)) && (FSL_FEATURE_FLEXCAN_HAS_EXTENDED_FLAG_REGISTER > 0)
    while ((0U != FLEXCAN_GetMbStatusFlags(base, 0xFFFFFFFFFFFFFFFFU)) ||
           (0U != (result & (kFLEXCAN_TxWarningIntFlag | kFLEXCAN_RxWarningIntFlag | kFLEXCAN_BusOffIntFlag |
                             kFLEXCAN_ErrorIntFlag | kFLEXCAN_WakeUpIntFlag))));
#else
    while ((0U != FLEXCAN_GetMbStatusFlags(base, 0xFFFFFFFFU)) ||
           (0U != (result & (kFLEXCAN_TxWarningIntFlag | kFLEXCAN_RxWarningIntFlag | kFLEXCAN_BusOffIntFlag |
                             kFLEXCAN_ErrorIntFlag | kFLEXCAN_WakeUpIntFlag))));
#endif
}

/*!
 * brief User FlexCAN IRQ handle function.
 *
 * This function handles the FlexCAN Error, the Message Buffer IRQ request.
 *
 * param base FlexCAN peripheral base address.
 * param handle FlexCAN handle pointer.
 */
static void User_CreateHandle(CAN_Type *base,
                              flexcan_handle_t *handle,
                              flexcan_transfer_callback_t callback,
                              void *userData)
{
    assert(handle);

    /* Clean FlexCAN transfer handle. */
    memset(handle, 0, sizeof(*handle));

    /* Register Callback function. */
    handle->callback = callback;
    handle->userData = userData;

    /* We Enable Error & Status interrupt here, because this interrupt just
     * report current status of FlexCAN module through Callback function.
     * It is insignificance without a available callback function.
     */
    if (handle->callback != NULL)
    {
        FLEXCAN_EnableInterrupts(base, kFLEXCAN_BusOffInterruptEnable | kFLEXCAN_ErrorInterruptEnable |
                                           kFLEXCAN_RxWarningInterruptEnable | kFLEXCAN_TxWarningInterruptEnable |
                                           kFLEXCAN_WakeUpInterruptEnable);
    }
    else
    {
        FLEXCAN_DisableInterrupts(base, kFLEXCAN_BusOffInterruptEnable | kFLEXCAN_ErrorInterruptEnable |
                                            kFLEXCAN_RxWarningInterruptEnable | kFLEXCAN_TxWarningInterruptEnable |
                                            kFLEXCAN_WakeUpInterruptEnable);
    }

    /* Enable interrupts in NVIC. */
    (void)EnableIRQ((IRQn_Type)(EXAMPLE_FLEXCAN_IRQn));
    (void)EnableIRQ((IRQn_Type)(EXAMPLE_FLEXCAN_MBIRQn));
}

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
                rxComplete = true;
            }
            break;

        case kStatus_FLEXCAN_TxIdle:
            if (TX_MESSAGE_BUFFER_NUM == result)
            {
                txComplete = true;
            }
            break;

        case kStatus_FLEXCAN_WakeUp:
            wakenUp = true;
            break;

        default:
            break;
    }
}

void EXAMPLE_FLEXCAN_IRQHandler(void)
{
    User_TransferHandleIRQ(EXAMPLE_CAN, &flexcanHandle);
    SDK_ISR_EXIT_BARRIER;
}

/*!
 * @brief Main function
 */
int main(void)
{
    flexcan_config_t flexcanConfig;
    flexcan_rx_mb_config_t mbConfig;
    uint8_t node_type;

    /* Initialize board hardware. */
    BOARD_InitHardware();

    LOG_INFO("********* FLEXCAN Interrupt EXAMPLE *********\r\n");
    LOG_INFO("    Message format: Standard (11 bit id)\r\n");
    LOG_INFO("    Message buffer %d used for Rx.\r\n", RX_MESSAGE_BUFFER_NUM);
    LOG_INFO("    Message buffer %d used for Tx.\r\n", TX_MESSAGE_BUFFER_NUM);
    LOG_INFO("    Interrupt Mode: Enabled\r\n");
    LOG_INFO("    Operation Mode: TX and RX --> Normal\r\n");
    LOG_INFO("*********************************************\r\n\r\n");

    do
    {
        LOG_INFO("Please select local node as A or B:\r\n");
        LOG_INFO("Note: Node B should start first.\r\n");
        LOG_INFO("Node:");
        node_type = GETCHAR();
        LOG_INFO("%c", node_type);
        LOG_INFO("\r\n");
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

    /* Get FlexCAN module default Configuration. */
    /*
     * flexcanConfig.clkSrc                 = kFLEXCAN_ClkSrc0;
     * flexcanConfig.bitRate               = 1000000U;
     * flexcanConfig.bitRateFD             = 2000000U;
     * flexcanConfig.maxMbNum               = 16;
     * flexcanConfig.enableLoopBack         = false;
     * flexcanConfig.enableSelfWakeup       = false;
     * flexcanConfig.enableIndividMask      = false;
     * flexcanConfig.disableSelfReception   = false;
     * flexcanConfig.enableListenOnlyMode   = false;
     * flexcanConfig.enableDoze             = false;
     */
    FLEXCAN_GetDefaultConfig(&flexcanConfig);

#if defined(EXAMPLE_CAN_CLK_SOURCE)
    flexcanConfig.clkSrc = EXAMPLE_CAN_CLK_SOURCE;
#endif

/* If special quantum setting is needed, set the timing parameters. */
#if (defined(SET_CAN_QUANTUM) && SET_CAN_QUANTUM)
    flexcanConfig.timingConfig.phaseSeg1 = PSEG1;
    flexcanConfig.timingConfig.phaseSeg2 = PSEG2;
    flexcanConfig.timingConfig.propSeg   = PROPSEG;
#if (defined(FSL_FEATURE_FLEXCAN_HAS_FLEXIBLE_DATA_RATE) && FSL_FEATURE_FLEXCAN_HAS_FLEXIBLE_DATA_RATE)
    flexcanConfig.timingConfig.fphaseSeg1 = FPSEG1;
    flexcanConfig.timingConfig.fphaseSeg2 = FPSEG2;
    flexcanConfig.timingConfig.fpropSeg   = FPROPSEG;
#endif
#endif

#if (defined(USE_IMPROVED_TIMING_CONFIG) && USE_IMPROVED_TIMING_CONFIG)
    flexcan_timing_config_t timing_config;
    memset(&timing_config, 0, sizeof(flexcan_timing_config_t));
#if (defined(USE_CANFD) && USE_CANFD)
    if (FLEXCAN_FDCalculateImprovedTimingValues(EXAMPLE_CAN, flexcanConfig.bitRate, flexcanConfig.bitRateFD,
                                                EXAMPLE_CAN_CLK_FREQ, &timing_config))
    {
        /* Update the improved timing configuration*/
        memcpy(&(flexcanConfig.timingConfig), &timing_config, sizeof(flexcan_timing_config_t));
    }
    else
    {
        LOG_INFO("No found Improved Timing Configuration. Just used default configuration\r\n\r\n");
    }
#else
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
#endif

#if (defined(USE_CANFD) && USE_CANFD)
    FLEXCAN_FDInit(EXAMPLE_CAN, &flexcanConfig, EXAMPLE_CAN_CLK_FREQ, BYTES_IN_MB, true);
#else
    FLEXCAN_Init(EXAMPLE_CAN, &flexcanConfig, EXAMPLE_CAN_CLK_FREQ);
#endif

    /* Create FlexCAN handle structure and set call back function. */
    User_CreateHandle(EXAMPLE_CAN, &flexcanHandle, flexcan_callback, NULL);

    /* Set Rx Masking mechanism. */
    FLEXCAN_SetRxMbGlobalMask(EXAMPLE_CAN, FLEXCAN_RX_MB_STD_MASK(rxIdentifier, 0, 0));

    /* Setup Rx Message Buffer. */
    mbConfig.format = kFLEXCAN_FrameFormatStandard;
    mbConfig.type   = kFLEXCAN_FrameTypeData;
    mbConfig.id     = FLEXCAN_ID_STD(rxIdentifier);
#if (defined(USE_CANFD) && USE_CANFD)
    FLEXCAN_SetFDRxMbConfig(EXAMPLE_CAN, RX_MESSAGE_BUFFER_NUM, &mbConfig, true);
#else
    FLEXCAN_SetRxMbConfig(EXAMPLE_CAN, RX_MESSAGE_BUFFER_NUM, &mbConfig, true);
#endif

/* Setup Tx Message Buffer. */
#if (defined(USE_CANFD) && USE_CANFD)
    FLEXCAN_SetFDTxMbConfig(EXAMPLE_CAN, TX_MESSAGE_BUFFER_NUM, true);
#else
    FLEXCAN_SetTxMbConfig(EXAMPLE_CAN, TX_MESSAGE_BUFFER_NUM, true);
#endif

    if ((node_type == 'A') || (node_type == 'a'))
    {
        LOG_INFO("Press any key to trigger one-shot transmission\r\n\r\n");
        frame.dataByte0 = 0;
    }
    else
    {
        LOG_INFO("Start to Wait data from Node A\r\n\r\n");
    }

    while (true)
    {
        if ((node_type == 'A') || (node_type == 'a'))
        {
            GETCHAR();

            frame.id     = FLEXCAN_ID_STD(txIdentifier);
            frame.format = (uint8_t)kFLEXCAN_FrameFormatStandard;
            frame.type   = (uint8_t)kFLEXCAN_FrameTypeData;
            frame.length = (uint8_t)DLC;
#if (defined(USE_CANFD) && USE_CANFD)
            frame.brs = 1U;
            frame.edl = 1U;
#endif
            txXfer.mbIdx = (uint8_t)TX_MESSAGE_BUFFER_NUM;
#if (defined(USE_CANFD) && USE_CANFD)
            txXfer.framefd = &frame;
            (void)FLEXCAN_TransferFDSendNonBlocking(EXAMPLE_CAN, &flexcanHandle, &txXfer);
#else
            txXfer.frame = &frame;
            (void)FLEXCAN_TransferSendNonBlocking(EXAMPLE_CAN, &flexcanHandle, &txXfer);
#endif

            while (!txComplete)
            {
            };
            txComplete = false;

            LOG_INFO("A board Tx MB ID: 0x%3x, Tx MB data: 0x%x, Time stamp: %d\r\n", frame.id >> CAN_ID_STD_SHIFT,
                     frame.dataByte0, frame.timestamp);
            /* Start receive data through Rx Message Buffer. */
            rxXfer.mbIdx = (uint8_t)RX_MESSAGE_BUFFER_NUM;
#if (defined(USE_CANFD) && USE_CANFD)
            rxXfer.framefd = &frame;
            (void)FLEXCAN_TransferFDReceiveNonBlocking(EXAMPLE_CAN, &flexcanHandle, &rxXfer);
#else
            rxXfer.frame = &frame;
            (void)FLEXCAN_TransferReceiveNonBlocking(EXAMPLE_CAN, &flexcanHandle, &rxXfer);
#endif

            /* Wait until Rx MB full. */
            while (!rxComplete)
            {
            };
            rxComplete = false;

            LOG_INFO("A Board Rx MB ID: 0x%3x, Rx MB data: 0x%x, Time stamp: %d\r\n", frame.id >> CAN_ID_STD_SHIFT,
                     frame.dataByte0, frame.timestamp);
            LOG_INFO("Press any key to trigger the next transmission!\r\n\r\n");
            frame.dataByte0++;
            frame.dataByte1 = 0x55;
        }
        else
        {
            /* Before this , should first make node B enter STOP mode after FlexCAN
             * initialized with enableSelfWakeup=true and Rx MB configured, then A
             * sends frame N which wakes up node B. A will continue to send frame N
             * since no acknowledgement, then B received the second frame N(In the
             * application it seems that B received the frame that woke it up which
             * is not expected as stated in the reference manual, but actually the
             * output in the terminal B received is the same second frame N). */
            if (wakenUp)
            {
                LOG_INFO("B has been waken up!\r\n\r\n");
            }

            /* Start receive data through Rx Message Buffer. */
            rxXfer.mbIdx = (uint8_t)RX_MESSAGE_BUFFER_NUM;
#if (defined(USE_CANFD) && USE_CANFD)
            rxXfer.framefd = &frame;
            (void)FLEXCAN_TransferFDReceiveNonBlocking(EXAMPLE_CAN, &flexcanHandle, &rxXfer);
#else
            rxXfer.frame = &frame;
            (void)FLEXCAN_TransferReceiveNonBlocking(EXAMPLE_CAN, &flexcanHandle, &rxXfer);
#endif

            /* Wait until Rx receive full. */
            while (!rxComplete)
            {
            };
            rxComplete = false;

            LOG_INFO("B board Rx MB ID: 0x%3x, Rx MB data: 0x%x, Time stamp: %d\r\n", frame.id >> CAN_ID_STD_SHIFT,
                     frame.dataByte0, frame.timestamp);

            frame.id     = FLEXCAN_ID_STD(txIdentifier);
            txXfer.mbIdx = (uint8_t)TX_MESSAGE_BUFFER_NUM;
#if (defined(USE_CANFD) && USE_CANFD)
            txXfer.framefd = &frame;
            (void)FLEXCAN_TransferFDSendNonBlocking(EXAMPLE_CAN, &flexcanHandle, &txXfer);
#else
            txXfer.frame = &frame;
            (void)FLEXCAN_TransferSendNonBlocking(EXAMPLE_CAN, &flexcanHandle, &txXfer);
#endif

            while (!txComplete)
            {
            };
            txComplete = false;
            LOG_INFO("B board Tx MB ID: 0x%3x, Tx MB data: 0x%x, Time stamp: %d\r\n", frame.id >> CAN_ID_STD_SHIFT,
                     frame.dataByte0, frame.timestamp);
            LOG_INFO("Wait Node A to trigger the next transmission!\r\n\r\n");
        }
    }
}
