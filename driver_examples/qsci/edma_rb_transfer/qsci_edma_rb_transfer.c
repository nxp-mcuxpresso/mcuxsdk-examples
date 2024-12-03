/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"
#include "app.h"
#include "fsl_qsci_edma.h"
#if defined(FSL_FEATURE_SOC_DMAMUX_COUNT) && FSL_FEATURE_SOC_DMAMUX_COUNT
#include "fsl_dmamux.h"
#endif
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Ring buffer size definition, please make sure to set this value large enough.
 * Otherwise, once overflow occurred, data in ring buffer will be overwritten.
 */
#define EXAMPLE_RING_BUFFER_SIZE (32U)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/* Initialzie the USART module. */
static void EXAMPLE_InitEDMA(void);

/* Initalize the DMA configuration for USART  TX and RX used.  */
static void EXAMPLE_InitQSCI(void);

/* Start ring buffer. */
static void EXAMPLE_StartRingBufferEDMA(void);

/* Reading data from the ring buffer. */
static void EXAMPLE_ReadRingBuffer(uint8_t *ringBuffer, uint8_t *receiveBuffer, uint32_t length);

/* QSCI RX EDMA call back. */
void EXAMPLE_EdmaRxCallback(edma_handle_t *handle, void *param, bool transferDone, uint32_t tcds);

/* QSCI user callback */
void EXAMPLE_QsciEdmaCallback(qsci_edma_transfer_handle_t *handle);

/*******************************************************************************
 * Variables
 ******************************************************************************/
AT_NONCACHEABLE_SECTION_ALIGN(qsci_edma_transfer_handle_t g_qsciEdmaHandle, sizeof(edma_channel_tcd_t));
edma_handle_t g_qsciRxEdmaHandle;
AT_NONCACHEABLE_SECTION_INIT(uint8_t g_tipString[]) =
    "QSCI EDMA ring buffer example.\r\nBoard will send back received characters.\r\nDo not input more than 32 bytes of "
    "characters at one time!\r\n";
AT_NONCACHEABLE_SECTION_INIT(uint8_t g_rxBuffer[EXAMPLE_RING_BUFFER_SIZE]) = {0};
volatile bool txOnGoing                                                    = false;
volatile bool isIdleLineDetected                                           = false;
volatile uint32_t ringBufferFlag                                           = 0U;
volatile uint32_t receivedBytes                                            = 0U;
volatile uint32_t ringBufferIndex                                          = 0U;

/* allocate ring buffer section. */
AT_NONCACHEABLE_SECTION_INIT(uint8_t g_ringBuffer[EXAMPLE_RING_BUFFER_SIZE]) = {0};
/* Allocate TCD memory poll with ring buffer used. */
AT_NONCACHEABLE_SECTION_ALIGN(static edma_channel_tcd_t tcdMemoryPoolPtr[1], sizeof(edma_channel_tcd_t));

/*******************************************************************************
 * Code
 ******************************************************************************/

/* Initialzie the USART module. */
static void EXAMPLE_InitQSCI(void)
{
    qsci_config_t config;

    /*
      config.eTransferMode     = kQSCI_Normal;
      config.bStopInWaitEnable = false;
      config.eDataBitMode      = kQSCI_Data8Bit;
      config.eWakeupMode       = kQSCI_WakeupOnIdleLine;
      config.ePolarityMode     = kQSCI_PolarityNormal;
      config.eParityMode       = kQSCI_ParityDisabled;
      config.bEnableStopHold   = false;
      config.bEnableTx         = false;
      config.bEnableRx         = false;
      config.bEnableFifo  = true;
      config.bEnableTxDMA = false;
      config.bEnableRxDMA = false;
      config.eTxFifoWatermark = kQSCI_TxWater3Word;
      config.eRxFifoWatermark = kQSCI_RxWater1Word;
      config.u32BaudRateBps  = 115200;
      config.u32SrcClockHz   = u32SrcClockHz;
      config.u8Interrupts = 0U;
     */
    QSCI_GetDefaultConfig(&config, BOARD_DEBUG_UART_BAUDRATE, EXAMPLE_QSCI_CLK_FREQ);
    QSCI_Init(EXAMPLE_QSCI, &config);
}

/* Initalize the DMA configuration for USART  TX and RX used. */
static void EXAMPLE_InitEDMA(void)
{
    edma_config_t config;

#if defined(FSL_FEATURE_SOC_DMAMUX_COUNT) && FSL_FEATURE_SOC_DMAMUX_COUNT
    /* Set channel for QSCI */
    DMAMUX_ConnectChannelToTriggerSource(EXAMPLE_QSCI_DMAMUX_BASEADDR, QSCI_TX_DMAMUX_CHANNEL, QSCI_TX_DMA_REQUEST);
    DMAMUX_ConnectChannelToTriggerSource(EXAMPLE_QSCI_DMAMUX_BASEADDR, QSCI_RX_DMAMUX_CHANNEL, QSCI_RX_DMA_REQUEST);
#endif

    /* Init the EDMA module */
    EDMA_GetDefaultConfig(&config);
    EDMA_Init(EXAMPLE_QSCI_DMA_BASEADDR, &config);

    /* Create QSCI DMA handle for sending data. */
    QSCI_TransferCreateHandleEDMA(EXAMPLE_QSCI, &g_qsciEdmaHandle, EXAMPLE_QsciEdmaCallback, NULL,
                                  EXAMPLE_QSCI_DMA_BASEADDR, QSCI_TX_DMA_CHANNEL, QSCI_RX_DMA_CHANNEL);
}

/* Start ring buffer. */
static void EXAMPLE_StartRingBufferEDMA(void)
{
    edma_channel_transfer_config_t xferConfig;

    EDMA_TransferCreateHandle(EXAMPLE_QSCI_DMA_BASEADDR, &g_qsciRxEdmaHandle, QSCI_RX_DMA_CHANNEL, tcdMemoryPoolPtr, 1U,
                              EXAMPLE_EdmaRxCallback, NULL);

    uint32_t u32SrcByteAddr = QSCI_GetDataRegisterAddress(EXAMPLE_QSCI);
    EDMA_GetChannelDefaultTransferConfig(&xferConfig, u32SrcByteAddr, (uint32_t)(uint8_t *)g_ringBuffer, 1U,
                                         EXAMPLE_RING_BUFFER_SIZE, kEDMA_ChannelTransferWidth8Bits,
                                         kEDMA_ChannelTransferPeripheralToMemory);
    xferConfig.u16EnabledInterruptMask               = kEDMA_ChannelMajorLoopCompleteInterruptEnable;
    xferConfig.bDisableRequestAfterMajorLoopComplete = false;

    /* Config loop transfer. */
    EDMA_TransferSubmitLoopTransfer(&g_qsciRxEdmaHandle, &xferConfig, 1U);
    EDMA_TransferStart(&g_qsciRxEdmaHandle);

    /* Enable QSCI RX EDMA. */
    QSCI_EnableRx(EXAMPLE_QSCI, true);
    QSCI_EnableRxDMA(EXAMPLE_QSCI, true);
}

/* Reading out the data from ring buffer. */
static void EXAMPLE_ReadRingBuffer(uint8_t *ringBuffer, uint8_t *receiveBuffer, uint32_t length)
{
    assert(ringBuffer != NULL);
    assert(receiveBuffer != NULL);
    assert(length != 0U);

    uint32_t index = length;

    /* If length if larger than ring buffer size, it means overflow occurred, need to reset the ringBufferIndex. */
    if (length > EXAMPLE_RING_BUFFER_SIZE)
    {
        ringBufferIndex = ((ringBufferIndex + length) % EXAMPLE_RING_BUFFER_SIZE);
        index           = EXAMPLE_RING_BUFFER_SIZE;
    }

    while (index)
    {
        *(receiveBuffer++) = ringBuffer[ringBufferIndex++];
        if (ringBufferIndex == EXAMPLE_RING_BUFFER_SIZE)
        {
            ringBufferIndex = 0U;
            ringBufferFlag--;
        }
        index--;
    }
}

/* QSCI RX EDMA call back. */
void EXAMPLE_EdmaRxCallback(edma_handle_t *handle, void *param, bool transferDone, uint32_t tcds)
{
    if (true == transferDone)
    {
        ringBufferFlag++;
    }
}

/* QSCI EDMA TX user callback */
void EXAMPLE_QsciEdmaCallback(qsci_edma_transfer_handle_t *handle)
{
    status_t busStatus = QSCI_GET_BUS_STATUS(handle);

    if (kStatus_QSCI_TxIdle == busStatus)
    {
        txOnGoing = false;
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    qsci_transfer_t sendXfer;

    BOARD_InitHardware();

    /* Initialize the QSCI module. */
    EXAMPLE_InitQSCI();

    /* Intialzie the EDMA configuration. */
    EXAMPLE_InitEDMA();

    /* Send g_tipString out. */
    sendXfer.pu8Data     = g_tipString;
    sendXfer.u32DataSize = sizeof(g_tipString) - 1;
    txOnGoing            = true;
    QSCI_TransferSendEDMA(&g_qsciEdmaHandle, &sendXfer);

    /* Wait send finished */
    while (txOnGoing)
    {
    }

    /* Start ring buffer with EDMA used. */
    EXAMPLE_StartRingBufferEDMA();

    while (1)
    {
        /* If new data arrived. */
        while (0U == ((uint16_t)kQSCI_RxIdleLineFlag & QSCI_GetStatusFlags(EXAMPLE_QSCI)))
        {
        }
        (void)QSCI_ClearStatusFlags(EXAMPLE_QSCI, kQSCI_RxIdleLineFlag);
        receivedBytes = EXAMPLE_RING_BUFFER_SIZE -
                        EDMA_GetChannelRemainingMajorLoopCount(EXAMPLE_QSCI_DMA_BASEADDR, QSCI_RX_DMA_CHANNEL);
        receivedBytes += (EXAMPLE_RING_BUFFER_SIZE * ringBufferFlag) - ringBufferIndex;

        if (receivedBytes > 32U)
        {
            __NOP();
        }

        EXAMPLE_ReadRingBuffer(g_ringBuffer, g_rxBuffer, receivedBytes);

        /* Start to echo. */
        sendXfer.pu8Data     = g_rxBuffer;
        sendXfer.u32DataSize = receivedBytes;

        QSCI_TransferSendEDMA(&g_qsciEdmaHandle, &sendXfer);
    }
}
