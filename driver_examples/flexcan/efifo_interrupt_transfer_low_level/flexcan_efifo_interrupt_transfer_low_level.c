/*
 * Copyright 2026 NXP
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
/*
 *    DWORD_IN_MB    DLC    BYTES_IN_MB             Maximum MBs
 *    2              8      kFLEXCAN_8BperMB    32(1 RAM block)  64(2 RAM block)  96(3 RAM block)
 *    4              10     kFLEXCAN_16BperMB   21(1 RAM block)  42(2 RAM block)  63(3 RAM block)
 *    8              13     kFLEXCAN_32BperMB   12(1 RAM block)  24(2 RAM block)  36(3 RAM block)
 *    16             15     kFLEXCAN_64BperMB   7(1 RAM block)   14(2 RAM block)  21(3 RAM block)
 *
 * Dword in each message buffer, Length of data in bytes, Payload size must align,
 * and the Message Buffers are limited corresponding to each payload configuration:
 */
#define DLC             (15)
#define BYTES_IN_MB     kFLEXCAN_64BperMB
/*
 * Ring buffer capacity for received frames. Must be a power of 2 and large
 * enough to hold frames that accumulate between two main-loop iterations.
 */
#define RX_FRAME_BUF_SIZE (16U)

/*******************************************************************************
 * Variables
 ******************************************************************************/
/*
 * Ring buffer shared between the ISR (producer) and the main loop (consumer).
 * Both indices are volatile so the compiler does not cache their values across
 * ISR/main context switches on Cortex-M (no cache, no explicit barrier needed).
 *
 * Invariant: (g_rxWriteIdx - g_rxReadIdx) <= RX_FRAME_BUF_SIZE at all times.
 * 
 * DESIGN NOTE: Index overflow behavior
 * -------------------------------------
 * The indices are uint32_t and intentionally allowed to overflow. This is SAFE
 * because C guarantees unsigned integer overflow wraps modulo 2^32. The key
 * properties that make this work:
 *
 *   1. Unsigned subtraction (g_rxWriteIdx - g_rxReadIdx) produces the correct
 *      number of unread frames even after overflow, due to modular arithmetic.
 *   2. Array indexing via (index % RX_FRAME_BUF_SIZE) always yields valid
 *      offsets regardless of how large the indices grow.
 *   3. No manual reset to zero is needed or desired — doing so would require
 *      complex synchronization and could violate the invariant.
 */
flexcan_fd_frame_t g_rxBuf[RX_FRAME_BUF_SIZE];
flexcan_fd_frame_t g_dummy; /* For popping frames when ring buffer is full. */
volatile uint32_t  g_rxWriteIdx = 0U; /* Written by ISR only.       */
volatile uint32_t  g_rxReadIdx  = 0U; /* Written by main loop only. */
volatile bool      g_rxOverflow = false; /* Set by ISR on ERFOVF.   */

flexcan_fd_frame_t txFrame = {0};
/* Configure FIFO filters to accept standard frames with ID 0x123 ~ 0x126. */
uint32_t rxEnFifoFilter[] = {FLEXCAN_ENHANCED_RX_FIFO_STD_MASK_AND_FILTER(0x123, 0, 0x7FF, 0),
                             FLEXCAN_ENHANCED_RX_FIFO_STD_MASK_AND_FILTER(0x124, 0, 0x7FF, 0),
                             FLEXCAN_ENHANCED_RX_FIFO_STD_MASK_AND_FILTER(0x125, 0, 0x7FF, 0),
                             FLEXCAN_ENHANCED_RX_FIFO_STD_MASK_AND_FILTER(0x126, 0, 0x7FF, 0)};

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief FlexCAN ISR — low-level Enhanced Rx FIFO handler.
 *
 * This ISR bypasses the FlexCAN transfer layer and calls the hardware APIs
 * directly. It therefore does NOT call FLEXCAN_TransferHandleIRQ and does NOT
 * require a flexcan_handle_t.
 *
 * Reception design notes
 * ----------------------
 * Only the Data Available (ERFDA) interrupt is enabled.  ERFDA is a hardware
 * status bit that reflects whether the FIFO output port holds a frame
 * (ERFEL > 0).  It clears automatically when FLEXCAN_ReadEnhancedRxFifo pops
 * the last frame from the FIFO.
 *
 * Unlike UART, CAN has no hardware bus-idle / frame-gap interrupt.  ERFDA is
 * therefore the simplest reliable mechanism for receiving an unknown number of
 * frames: it fires for every frame regardless of count, so the application
 * never needs to pre-specify how many frames to expect.
 *
 * The Enhanced Rx FIFO Watermark interrupt is NOT enabled here.  Enabling it
 * simultaneously with ERFDA in an ISR-driven design offers no benefit: because
 * ERFDA causes the ISR to read each frame as it arrives, the hardware FIFO
 * count never accumulates to the watermark level under normal conditions.
 */
void EXAMPLE_FLEXCAN_IRQHandler(void)
{
    uint64_t flags = FLEXCAN_GetStatusFlags(EXAMPLE_CAN);

    /* Handle Enhanced Rx FIFO overflow. This flag must be cleared explicitly. */
    if (0U != (flags & (uint64_t)kFLEXCAN_ERxFifoOverflowIntFlag))
    {
        g_rxOverflow = true;
        FLEXCAN_ClearStatusFlags(EXAMPLE_CAN, (uint64_t)kFLEXCAN_ERxFifoOverflowIntFlag);
    }

    /* Handle Enhanced Rx FIFO data available. */
    if (0U != (flags & (uint64_t)kFLEXCAN_ERxFifoDataAvlIntFlag))
    {
        /* Unsigned subtraction correctly computes unread count even after overflow. */
        if ((g_rxWriteIdx - g_rxReadIdx) < RX_FRAME_BUF_SIZE)
        {
            /* Ring buffer has space: store the received frame. */
            (void)FLEXCAN_ReadEnhancedRxFifo(EXAMPLE_CAN, &g_rxBuf[g_rxWriteIdx % RX_FRAME_BUF_SIZE]);
            /* Increment is safe: uint32_t overflow wraps to 0 per C standard. */
            g_rxWriteIdx++;
        }
        else
        {
            /*
             * Ring buffer is full. Pop the frame from the hardware FIFO anyway
             * to prevent the hardware FIFO from overflowing and losing frames
             * that will fit once the main loop catches up.
             */
            (void)FLEXCAN_ReadEnhancedRxFifo(EXAMPLE_CAN, &g_dummy);
        }
    }

    SDK_ISR_EXIT_BARRIER;
}

/*!
 * @brief Main function
 */
int main(void)
{
    flexcan_config_t flexcanConfig;
    flexcan_enhanced_rx_fifo_config_t rxEhFifoConfig;
    uint8_t node_type;
    uint32_t i;

    /* Initialize board hardware. */
    BOARD_InitHardware();

    PRINTF("MCUX SDK version: %s\r\n", MCUXSDK_VERSION_FULL_STR);

    LOG_INFO("FlexCAN Enhanced Rx FIFO low-level interrupt example.\r\n");
    LOG_INFO("Board to board mode.\r\n");
    LOG_INFO("Node B: Enhanced Rx FIFO used for Rx (frame count unknown in advance).\r\n");
    LOG_INFO("Node A: Message buffer %d used for Tx.\r\n", TX_MESSAGE_BUFFER_NUM);

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

#if defined(EXAMPLE_CAN_CLK_SOURCE)
    flexcanConfig.clkSrc = EXAMPLE_CAN_CLK_SOURCE;
#endif

#if (defined(USE_IMPROVED_TIMING_CONFIG) && USE_IMPROVED_TIMING_CONFIG)
    flexcan_timing_config_t timing_config;
    memset(&timing_config, 0, sizeof(flexcan_timing_config_t));
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
#endif

    FLEXCAN_FDInit(EXAMPLE_CAN, &flexcanConfig, EXAMPLE_CAN_CLK_FREQ, BYTES_IN_MB, true);

    if ((node_type == 'A') || (node_type == 'a'))
    {
        /* Node A: configure Tx Message Buffer. */
        FLEXCAN_SetFDTxMbConfig(EXAMPLE_CAN, TX_MESSAGE_BUFFER_NUM, true);
        txFrame.dataWord[0] = 0;
        txFrame.dataWord[1] = 0x55U;
        txFrame.format      = (uint8_t)kFLEXCAN_FrameFormatStandard;
        txFrame.type        = (uint8_t)kFLEXCAN_FrameTypeData;
        txFrame.length      = (uint8_t)DLC;
        txFrame.brs         = (uint8_t)1U;
        txFrame.edl         = (uint8_t)1U;
    }
    else
    {
        /*
         * Node B: configure Enhanced Rx FIFO and enable interrupts.
         *
         * No flexcan_handle_t is created and no transfer function is called.
         * FLEXCAN_EnableInterrupts arms the hardware interrupt lines directly.
         * The ISR (EXAMPLE_FLEXCAN_IRQHandler) handles them with low-level APIs.
         */
        rxEhFifoConfig.idFilterTable     = rxEnFifoFilter;
        rxEhFifoConfig.idFilterPairNum   = sizeof(rxEnFifoFilter) / sizeof(rxEnFifoFilter[0]) / 2U;
        rxEhFifoConfig.extendIdFilterNum = 0;
        rxEhFifoConfig.fifoWatermark     = FSL_FEATURE_FLEXCAN_HAS_ENHANCED_RX_FIFO_SIZE - 1U;
        rxEhFifoConfig.dmaPerReadLength  = kFLEXCAN_19WordPerRead;
        rxEhFifoConfig.priority          = kFLEXCAN_RxFifoPrioHigh;
        FLEXCAN_SetEnhancedRxFifoConfig(EXAMPLE_CAN, &rxEhFifoConfig, true);

        FLEXCAN_EnableInterrupts(EXAMPLE_CAN, (uint64_t)kFLEXCAN_ERxFifoDataAvlInterruptEnable |
                                              (uint64_t)kFLEXCAN_ERxFifoOverflowInterruptEnable);
        (void)EnableIRQ(EXAMPLE_FLEXCAN_IRQn);

        LOG_INFO("Waiting for frames from Node A...\r\n\r\n");
    }

    while (true)
    {
        if ((node_type == 'A') || (node_type == 'a'))
        {
            uint8_t index  = 0;
            uint32_t times = 0;
            LOG_INFO("Please input the number of CAN FD messages to be send and end with enter.\r\n");
            while (index != 0x0D)
            {
                index = GETCHAR();

                if (times > 256U)
                {
                    times = 256U;
                    LOG_INFO("\r\nMaximum number of CAN FD messages to be send is 256.\r\n");
                }
                if ((index >= '0') && (index <= '9'))
                {
                    (void)PUTCHAR(index);
                    times = times * 10 + (index - 0x30U);
                }
            }
            LOG_INFO("\r\n");

            for (i = 0U; i < times; i++)
            {
                /* Standard ID: 0x123, 0x124, 0x125, 0x126. */
                txFrame.id = FLEXCAN_ID_STD(0x123U + i % 4U);
                txFrame.dataWord[0] = i;
                /*
                 * Write directly to the Tx Message Buffer and poll the IFLAG
                 * bit for completion.  This avoids the transfer layer entirely:
                 * no handle, no callback, no interrupt required on the Tx side.
                 */
                (void)FLEXCAN_WriteFDTxMb(EXAMPLE_CAN, TX_MESSAGE_BUFFER_NUM, &txFrame);
                while (0U == FLEXCAN_GetMbStatusFlags(EXAMPLE_CAN, 1UL << TX_MESSAGE_BUFFER_NUM))
                {
                }
                FLEXCAN_ClearMbStatusFlags(EXAMPLE_CAN, 1UL << TX_MESSAGE_BUFFER_NUM);

                LOG_INFO("Send Msg%d to Enhanced Rx FIFO: word0 = 0x%x, word1 = 0x%x, id = 0x%x.\r\n",
                         i, txFrame.dataWord[0], txFrame.dataWord[1], 0x123U + i % 4U);

                /* Wait for 10ms after every frame transmission. */
                SDK_DelayAtLeastUs(10000U, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
            }
            LOG_INFO("Transmission done.\r\n\r\n");
        }
        else
        {
            /* Report hardware FIFO overflow detected by the ISR. */
            if (g_rxOverflow)
            {
                g_rxOverflow = false;
                LOG_INFO("Warning: Enhanced Rx FIFO overflow! Some frames may have been lost.\r\n");
            }

            /*
             * Drain the ring buffer.  The ISR increments g_rxWriteIdx for every
             * frame received; the main loop increments g_rxReadIdx after
             * processing each frame.  No explicit synchronisation primitive is
             * needed on Cortex-M (no data cache) for this single-producer /
             * single-consumer pattern with volatile indices.
             *
             * Comparison (g_rxReadIdx != g_rxWriteIdx) works correctly even after
             * overflow because both indices wrap at the same modulus (2^32).
             */
            while (g_rxReadIdx != g_rxWriteIdx)
            {
                const flexcan_fd_frame_t *pFrame = &g_rxBuf[g_rxReadIdx % RX_FRAME_BUF_SIZE];
                LOG_INFO("Receive frame from Enhanced Rx FIFO: word0 = 0x%x, word1 = 0x%x, "
                         "ID Filter Hit: %d, Time stamp: %d.\r\n",
                         pFrame->dataWord[0], pFrame->dataWord[1],
                         pFrame->idhit, pFrame->timestamp);
                /* Increment is safe: uint32_t overflow wraps to 0 per C standard. */
                g_rxReadIdx++;
            }
        }
    }
}
