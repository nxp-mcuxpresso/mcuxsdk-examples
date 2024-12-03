/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"
#include "app.h"
#include "fsl_lpsci_dma.h"
#include "fsl_dmamux.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define EXAMPLE_RING_BUFFER_SIZE      (32U)
#define EXAMPLE_DMA_MODULE_TYPE       kDMA_Modulo32Bytes
#define EXAMPLE_DMA_TRANSFER_MAX_SIZE (0xFFFFFU)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/* Initialzie the LPSCI module. */
static void EXAMPLE_InitLPSCI(void);

/* Initialize the DMA and create handle for used. */
static void EXAMPLE_InitDMA(void);

/* Start ring buffe with DMA used. */
static void EXAMPLE_StartRingBufferDMA(void);

/* Reading the data from ring buffer. */
static void EXAMPLE_ReadRingBufferDMA(uint8_t *ringBuffer, uint8_t *receiveBuffer, uint32_t length);

/* Get how many characters stored in ring buffer.
 * Note: This function must be called before re-write the DSR_BCR register.
 */
static uint32_t EXAMPLE_GetRingBufferLength(void);

/* LPSCI user callback */
void LPSCI_UserCallback(UART0_Type *base, lpsci_dma_handle_t *handle, status_t status, void *userData);

/*******************************************************************************
 * Variables
 ******************************************************************************/
lpsci_dma_handle_t g_lpsciDmaHandle;
dma_handle_t g_lpsciTxDmaHandle;
dma_handle_t g_lpsciRxDmaHandle;
uint8_t g_tipString[] = "LPSCI DMA ring buffer example.\r\nBoard will send back the received characters.\r\n";
uint8_t g_txBuffer[EXAMPLE_RING_BUFFER_SIZE] = {0};
volatile bool txOnGoing                      = false;
volatile bool isIdleDetected                 = false;
volatile uint32_t receivedBytes              = 0U;

/* Note that, ring buffer must be 16 bytes aligned. */
AT_NONCACHEABLE_SECTION_ALIGN(uint8_t g_ringBuffer[EXAMPLE_RING_BUFFER_SIZE], 16);
static uint32_t ringBufferIndex = 0U;

/*******************************************************************************
 * Code
 ******************************************************************************/
/* Initialzie the LPSCI module. */
static void EXAMPLE_InitLPSCI(void)
{
    lpsci_config_t config;

    /* Initialize the LPSCI. */
    LPSCI_GetDefaultConfig(&config);
    config.idleLineType = kLPSCI_IdleLineStopBit;
    config.baudRate_Bps = BOARD_DEBUG_UART_BAUDRATE;
    config.enableTx     = true;
    config.enableRx     = true;

    LPSCI_Init(EXAMPLE_LPSCI, &config, EXAMPLE_LPSCI_CLK_FREQ);
}

/* Initialize the DMA and create handle for used. */
static void EXAMPLE_InitDMA(void)
{
    /* Init DMAMUX */
    DMAMUX_Init(EXAMPLE_LPSCI_DMAMUX_BASEADDR);

    /* Set channel for LPSCI  */
    DMAMUX_SetSource(EXAMPLE_LPSCI_DMAMUX_BASEADDR, LPSCI_TX_DMA_CHANNEL, LPSCI_TX_DMA_REQUEST);
    DMAMUX_EnableChannel(EXAMPLE_LPSCI_DMAMUX_BASEADDR, LPSCI_TX_DMA_CHANNEL);
    DMAMUX_SetSource(EXAMPLE_LPSCI_DMAMUX_BASEADDR, LPSCI_RX_DMA_CHANNEL, LPSCI_RX_DMA_REQUEST);
    DMAMUX_EnableChannel(EXAMPLE_LPSCI_DMAMUX_BASEADDR, LPSCI_RX_DMA_CHANNEL);

    /* Init the DMA module */
    DMA_Init(EXAMPLE_LPSCI_DMA_BASEADDR);

    /* Create handle for DMA transfer used. */
    DMA_CreateHandle(&g_lpsciTxDmaHandle, EXAMPLE_LPSCI_DMA_BASEADDR, LPSCI_TX_DMA_CHANNEL);
    DMA_CreateHandle(&g_lpsciRxDmaHandle, EXAMPLE_LPSCI_DMA_BASEADDR, LPSCI_RX_DMA_CHANNEL);

    /* Create LPSCI DMA handle for sending data. */
    LPSCI_TransferCreateHandleDMA(EXAMPLE_LPSCI, &g_lpsciDmaHandle, LPSCI_UserCallback, NULL, &g_lpsciTxDmaHandle,
                                  NULL);
}

/* Start ring buffe with DMA used. */
static void EXAMPLE_StartRingBufferDMA(void)
{
    dma_transfer_config_t xferConfig;

    ringBufferIndex = 0U;

    /* Prepare transfer. */
    DMA_PrepareTransfer(&xferConfig, (void *)LPSCI_GetDataRegisterAddress(EXAMPLE_LPSCI), sizeof(uint8_t), g_ringBuffer,
                        sizeof(uint8_t), EXAMPLE_DMA_TRANSFER_MAX_SIZE, kDMA_PeripheralToMemory);

    /* Submit transfer. */
    DMA_SubmitTransfer(&g_lpsciRxDmaHandle, &xferConfig, kDMA_EnableInterrupt);

    /* Note that, the dma_modulo_t must be equals to ring buffer size. */
    DMA_SetModulo(EXAMPLE_LPSCI_DMA_BASEADDR, LPSCI_RX_DMA_CHANNEL, kDMA_ModuloDisable, EXAMPLE_DMA_MODULE_TYPE);

    DMA_StartTransfer(&g_lpsciRxDmaHandle);

    /* Enable LPSCI RX DMA. */
    LPSCI_EnableRxDMA(EXAMPLE_LPSCI, true);

    /* Enable RX IDLE interrupt. */
    LPSCI_ClearStatusFlags(EXAMPLE_LPSCI, kLPSCI_IdleLineFlag);
    LPSCI_EnableInterrupts(EXAMPLE_LPSCI, kLPSCI_IdleLineInterruptEnable);
    EnableIRQ(EXAMPLE_LPSCI_IRQn);

    /* Set interrupt priority. */
    NVIC_SetPriority(DMA1_IRQn, 1);
    NVIC_SetPriority(EXAMPLE_LPSCI_IRQn, 2);
}

/* Reading the data from ring buffer. */
static void EXAMPLE_ReadRingBufferDMA(uint8_t *ringBuffer, uint8_t *receiveBuffer, uint32_t length)
{
    assert(ringBuffer);
    assert(receiveBuffer);
    assert(length);

    uint32_t index = length;

    /* If length if larger than ring buffer size, it means overflow occurred, need to reset the ringBufferIndex. */
    if (length > EXAMPLE_RING_BUFFER_SIZE)
    {
        ringBufferIndex = ((ringBufferIndex + length) % EXAMPLE_RING_BUFFER_SIZE);
        index           = EXAMPLE_RING_BUFFER_SIZE;
        receivedBytes   = EXAMPLE_RING_BUFFER_SIZE;
    }

    while (index)
    {
        *(receiveBuffer++) = ringBuffer[ringBufferIndex++];
        if (ringBufferIndex == EXAMPLE_RING_BUFFER_SIZE)
        {
            ringBufferIndex = 0U;
        }
        index--;
    }
}

/* Get how many characters stored in ring buffer.
 * Note: This function must be called before re-write the DSR_BCR register.
 */
static uint32_t EXAMPLE_GetRingBufferLength(void)
{
    return (EXAMPLE_DMA_TRANSFER_MAX_SIZE - DMA_GetRemainingBytes(EXAMPLE_LPSCI_DMA_BASEADDR, LPSCI_RX_DMA_CHANNEL));
}

/* LPSCI IRQ handler to handle the routine of IDLE line occurred. */
void EXAMPLE_LPSCI_IRQHandler(void)
{
    /* If new data arrived. */
    if ((kLPSCI_IdleLineFlag)&LPSCI_GetStatusFlags(EXAMPLE_LPSCI))
    {
        isIdleDetected = true;

        receivedBytes = EXAMPLE_GetRingBufferLength();

        /* Reload the BCR count for continueous transfer. */
        EXAMPLE_LPSCI_DMA_BASEADDR->DMA[LPSCI_RX_DMA_CHANNEL].DSR_BCR = DMA_DSR_BCR_BCR(EXAMPLE_DMA_TRANSFER_MAX_SIZE);

        LPSCI_ClearStatusFlags(EXAMPLE_LPSCI, kLPSCI_IdleLineFlag);
    }
    SDK_ISR_EXIT_BARRIER;
}

/* LPSCI user callback for LPSCI DMA Ssend. */
void LPSCI_UserCallback(UART0_Type *base, lpsci_dma_handle_t *handle, status_t status, void *userData)
{
    userData = userData;

    if (kStatus_LPSCI_TxIdle == status)
    {
        txOnGoing = false;
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    lpsci_transfer_t sendXfer;

    BOARD_InitHardware();

    /* Initialize the LPSCI module. */
    EXAMPLE_InitLPSCI();

    /* Initialize the DMA and create handles for LPSCI DMA transfer. */
    EXAMPLE_InitDMA();

    /* Send g_tipString out. */
    sendXfer.data     = g_tipString;
    sendXfer.dataSize = sizeof(g_tipString) - 1;
    txOnGoing         = true;
    LPSCI_TransferSendDMA(EXAMPLE_LPSCI, &g_lpsciDmaHandle, &sendXfer);

    /* Wait send finished */
    while (txOnGoing)
    {
    }

    /* Start ring buffer. */
    EXAMPLE_StartRingBufferDMA();

    while (1)
    {
        isIdleDetected = false;
        /* Wait for RX IDLE detected. */
        while (false == isIdleDetected)
        {
        }

        /* Move the data from ring buffer to given buffer section. */
        EXAMPLE_ReadRingBufferDMA(g_ringBuffer, g_txBuffer, receivedBytes);

        /* Wait send finished */
        while (txOnGoing)
        {
        }

        txOnGoing         = true;
        sendXfer.data     = g_txBuffer;
        sendXfer.dataSize = receivedBytes;
        LPSCI_TransferSendDMA(EXAMPLE_LPSCI, &g_lpsciDmaHandle, &sendXfer);
    }
}
