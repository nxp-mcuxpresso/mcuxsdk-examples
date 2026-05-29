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
#if (defined(USE_CANFD) && USE_CANFD)
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
#define DLC         (15)
#define BYTES_IN_MB kFLEXCAN_64BperMB
#else
#define DLC (8)
#endif
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile bool txComplete = false;
volatile bool rxComplete = false;
#if (defined(USE_CANFD) && USE_CANFD)
flexcan_fd_frame_t rxFrame;
flexcan_fd_frame_t txFrame;
#else
flexcan_frame_t rxFrame;
flexcan_frame_t txFrame;
#endif

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief FlexCAN_RX ISR — low-level Message Buffer handler.
 *
 * This ISR handles reception on FlexCAN_RX. It bypasses the FlexCAN transfer layer
 * and calls the hardware APIs directly.
 */
void EXAMPLE_FLEXCAN_RX_IRQHandler(void)
{
    uint64_t esrStatus = 0U;

    /* Get Current FlexCAN Module Error and Status. */
    esrStatus = FLEXCAN_GetStatusFlags(EXAMPLE_CAN_RX);

    if (0U != (esrStatus & (uint32_t)kFLEXCAN_BusOffIntFlag))
    {
        FLEXCAN_ClearStatusFlags(EXAMPLE_CAN_RX, (uint32_t)kFLEXCAN_BusOffIntFlag);
    }
    if (0U != (esrStatus & (uint32_t)kFLEXCAN_ErrorIntFlag))
    {
        FLEXCAN_ClearStatusFlags(EXAMPLE_CAN_RX, (uint32_t)kFLEXCAN_ErrorIntFlag);
    }
    if (0U != (esrStatus & (uint32_t)kFLEXCAN_RxWarningIntFlag))
    {
        FLEXCAN_ClearStatusFlags(EXAMPLE_CAN_RX, (uint32_t)kFLEXCAN_RxWarningIntFlag);
    }

    /* Handle Rx Message Buffer interrupt. */
    if (0U != (FLEXCAN_GetMbStatusFlags(EXAMPLE_CAN_RX, 1UL << RX_MESSAGE_BUFFER_NUM)))
    {
#if (defined(USE_CANFD) && USE_CANFD)
        (void)FLEXCAN_ReadFDRxMb(EXAMPLE_CAN_RX, RX_MESSAGE_BUFFER_NUM, &rxFrame);
#else
        (void)FLEXCAN_ReadRxMb(EXAMPLE_CAN_RX, RX_MESSAGE_BUFFER_NUM, &rxFrame);
#endif
        FLEXCAN_ClearMbStatusFlags(EXAMPLE_CAN_RX, 1UL << RX_MESSAGE_BUFFER_NUM);
        rxComplete = true;
    }

    SDK_ISR_EXIT_BARRIER;
}

/*!
 * @brief FlexCAN_TX ISR — low-level Message Buffer handler.
 *
 * This ISR handles transmission on FlexCAN_TX. It bypasses the FlexCAN transfer layer
 * and calls the hardware APIs directly.
 */
void EXAMPLE_FLEXCAN_TX_IRQHandler(void)
{
    uint64_t esrStatus = 0U;

    /* Get Current FlexCAN Module Error and Status. */
    esrStatus = FLEXCAN_GetStatusFlags(EXAMPLE_CAN_TX);

    if (0U != (esrStatus & (uint32_t)kFLEXCAN_BusOffIntFlag))
    {
        FLEXCAN_ClearStatusFlags(EXAMPLE_CAN_TX, (uint32_t)kFLEXCAN_BusOffIntFlag);
    }
    if (0U != (esrStatus & (uint32_t)kFLEXCAN_ErrorIntFlag))
    {
        FLEXCAN_ClearStatusFlags(EXAMPLE_CAN_TX, (uint32_t)kFLEXCAN_ErrorIntFlag);
    }
    if (0U != (esrStatus & (uint32_t)kFLEXCAN_TxWarningIntFlag))
    {
        FLEXCAN_ClearStatusFlags(EXAMPLE_CAN_TX, (uint32_t)kFLEXCAN_TxWarningIntFlag);
    }

    /* Handle Tx Message Buffer interrupt. */
    if (0U != (FLEXCAN_GetMbStatusFlags(EXAMPLE_CAN_TX, 1UL << TX_MESSAGE_BUFFER_NUM)))
    {
        FLEXCAN_ClearMbStatusFlags(EXAMPLE_CAN_TX, 1UL << TX_MESSAGE_BUFFER_NUM);
        txComplete = true;
    }

    SDK_ISR_EXIT_BARRIER;
}

/*!
 * @brief Main function
 */
int main(void)
{
    flexcan_config_t flexcanConfig;
    flexcan_rx_mb_config_t mbConfig;

    /* Initialize board hardware. */
    BOARD_InitHardware(); 

    PRINTF("MCUX SDK version: %s\r\n", MCUXSDK_VERSION_FULL_STR);

    LOG_INFO("********* FLEXCAN Interrupt (Low-Level) EXAMPLE *********\r\n");
    LOG_INFO("    Message format: Standard (11 bit id)\r\n");
    LOG_INFO("    FlexCAN_RX: Receiver (Message buffer %d)\r\n", RX_MESSAGE_BUFFER_NUM);
    LOG_INFO("    FlexCAN_TX: Transmitter (Message buffer %d)\r\n", TX_MESSAGE_BUFFER_NUM);
    LOG_INFO("    Interrupt Mode: Enabled (Low-Level API)\r\n");
    LOG_INFO("    Operation Mode: One-way communication\r\n");
    LOG_INFO("**********************************************************\r\n\r\n");

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
#if (defined(USE_CANFD) && USE_CANFD)
    if (FLEXCAN_FDCalculateImprovedTimingValues(EXAMPLE_CAN_RX, flexcanConfig.bitRate, flexcanConfig.bitRateFD,
                                                EXAMPLE_CAN_RX_CLK_FREQ, &timing_config))
    {
        /* Update the improved timing configuration*/
        memcpy(&(flexcanConfig.timingConfig), &timing_config, sizeof(flexcan_timing_config_t));
    }
    else
    {
        LOG_INFO("No found Improved Timing Configuration. Just used default configuration\r\n\r\n");
    }
#else
    if (FLEXCAN_CalculateImprovedTimingValues(EXAMPLE_CAN_RX, flexcanConfig.bitRate, EXAMPLE_CAN_RX_CLK_FREQ, &timing_config))
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

    /* Initialize FlexCAN_RX (Receiver) */
    LOG_INFO("Initializing FlexCAN_RX as receiver...\r\n");
#if (defined(USE_CANFD) && USE_CANFD)
    FLEXCAN_FDInit(EXAMPLE_CAN_RX, &flexcanConfig, EXAMPLE_CAN_RX_CLK_FREQ, BYTES_IN_MB, true);
#else
    FLEXCAN_Init(EXAMPLE_CAN_RX, &flexcanConfig, EXAMPLE_CAN_RX_CLK_FREQ);
#endif

    /* Initialize FlexCAN_TX (Transmitter) */
    LOG_INFO("Initializing FlexCAN_TX as transmitter...\r\n\r\n");
#if (defined(USE_CANFD) && USE_CANFD)
    FLEXCAN_FDInit(EXAMPLE_CAN_TX, &flexcanConfig, EXAMPLE_CAN_TX_CLK_FREQ, BYTES_IN_MB, true);
#else
    FLEXCAN_Init(EXAMPLE_CAN_TX, &flexcanConfig, EXAMPLE_CAN_TX_CLK_FREQ);
#endif

    /*
     * Low-level interrupt setup: no handle, no transfer layer.
     * FLEXCAN_EnableInterrupts arms the hardware interrupt lines directly.
     */

    /* Set Rx Masking mechanism. Only accept data frame with ID 0x123. */
    FLEXCAN_SetRxMbGlobalMask(EXAMPLE_CAN_RX, FLEXCAN_RX_MB_STD_MASK(0x7FFU, 0, 0));

    /* Setup Rx Message Buffer on FlexCAN_RX. */
    mbConfig.format = kFLEXCAN_FrameFormatStandard;
    mbConfig.type   = kFLEXCAN_FrameTypeData;
    mbConfig.id     = FLEXCAN_ID_STD(0x123);
#if (defined(USE_CANFD) && USE_CANFD)
    FLEXCAN_SetFDRxMbConfig(EXAMPLE_CAN_RX, RX_MESSAGE_BUFFER_NUM, &mbConfig, true);
#else
    FLEXCAN_SetRxMbConfig(EXAMPLE_CAN_RX, RX_MESSAGE_BUFFER_NUM, &mbConfig, true);
#endif

    /* Setup Tx Message Buffer on FlexCAN_TX. */
#if (defined(USE_CANFD) && USE_CANFD)
    FLEXCAN_SetFDTxMbConfig(EXAMPLE_CAN_TX, TX_MESSAGE_BUFFER_NUM, true);
#else
    FLEXCAN_SetTxMbConfig(EXAMPLE_CAN_TX, TX_MESSAGE_BUFFER_NUM, true);
#endif

    /* Enable Message Buffer interrupts using low-level API. */
    FLEXCAN_EnableMbInterrupts(EXAMPLE_CAN_RX, (1UL << RX_MESSAGE_BUFFER_NUM));
    FLEXCAN_EnableMbInterrupts(EXAMPLE_CAN_TX, (1UL << TX_MESSAGE_BUFFER_NUM));
    FLEXCAN_EnableInterrupts(EXAMPLE_CAN_RX, (uint32_t)kFLEXCAN_BusOffInterruptEnable |
                                             (uint32_t)kFLEXCAN_ErrorInterruptEnable |
                                             (uint32_t)kFLEXCAN_RxWarningInterruptEnable);
    FLEXCAN_EnableInterrupts(EXAMPLE_CAN_TX, (uint32_t)kFLEXCAN_BusOffInterruptEnable |
                                             (uint32_t)kFLEXCAN_ErrorInterruptEnable |
                                             (uint32_t)kFLEXCAN_TxWarningInterruptEnable);
    (void)EnableIRQ(EXAMPLE_FLEXCAN_RX_IRQn);
    (void)EnableIRQ(EXAMPLE_FLEXCAN_TX_IRQn);

    LOG_INFO("Press any key to trigger transmission from FlexCAN_TX to FlexCAN_RX\r\n\r\n");

    /* Prepare TX frame */
    txFrame.id     = FLEXCAN_ID_STD(0x123);
    txFrame.format = (uint8_t)kFLEXCAN_FrameFormatStandard;
    txFrame.type   = (uint8_t)kFLEXCAN_FrameTypeData;
    txFrame.length = (uint8_t)DLC;
    txFrame.dataByte0 = 0;
    txFrame.dataByte1 = 0x55;
#if (defined(USE_CANFD) && USE_CANFD)
    txFrame.brs = 1U;
    txFrame.edl = 1U;
#endif

    while (true)
    {
        GETCHAR();

        /* Transmit frame from FlexCAN_TX */
#if (defined(USE_CANFD) && USE_CANFD)
        (void)FLEXCAN_WriteFDTxMb(EXAMPLE_CAN_TX, TX_MESSAGE_BUFFER_NUM, &txFrame);
#else
        (void)FLEXCAN_WriteTxMb(EXAMPLE_CAN_TX, TX_MESSAGE_BUFFER_NUM, &txFrame);
#endif

        /* Wait for transmission complete */
        while (!txComplete)
        {
        };
        txComplete = false;
        LOG_INFO("FlexCAN_TX: Transmitted frame with data 0x%02x\r\n", txFrame.dataByte0);

        /* Wait for FlexCAN_RX to receive the frame */
        while (!rxComplete)
        {
        };
        rxComplete = false;

        LOG_INFO("FlexCAN_RX: Received frame - ID: 0x%03x, Data: 0x%02x, Timestamp: %d\r\n",
                 rxFrame.id >> CAN_ID_STD_SHIFT, rxFrame.dataByte0, rxFrame.timestamp);
        LOG_INFO("Press any key to trigger the next transmission!\r\n\r\n");

        /* Increment data for next transmission */
        txFrame.dataByte0++;
    }
}
