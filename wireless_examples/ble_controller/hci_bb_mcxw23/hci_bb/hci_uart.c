/**
 * Copyright 2018-2025 NXP.
 * NXP Confidential and Proprietary.
 * This software is owned or controlled by NXP and may only be used
 * strictly in accordance with the applicable license terms.  By expressly
 * accepting such terms or by downloading, installing, activating and/or
 * otherwise using the software, you are agreeing that you have read, and
 * that you agree to comply with and are bound by, such license terms.  If
 * you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 */

#include <stdio.h>
#include "fsl_usart.h"
#include "fsl_flexcomm.h"
#include "fsl_dma.h"
#include "hci_uart.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define HCI_UART                USART0
#define HCI_UART_IRQ            FLEXCOMM0_IRQn
#define HCI_UART_RESET          kFC0_RST_SHIFT_RSTn
#define HCI_UART_DMA_CHANNEL_RX (4u)
#define HCI_UART_DMA_CHANNEL_TX (5u)
#define LL_TO_HOST_BUF_SIZE (256u)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/** The size of the UART receive buffer in bytes. Must be a multiple of 2.
  * 16 would be equal to FIFO size for which some tests fail at 230400 baud. */
#define DATA_BUFFER_SIZE 256

/** Pointer to buffer to store the received bytes. */
static uint8_t *s_uReadBusBuffer;
/** The number of bytes to read as requested by the LL */
static uint16_t s_uReadSize = 0;
/** The number of bytes already read */
static uint16_t s_uReadPos  = 0;
/** Holds HCI UART call back functions */
static hci_uart_cb_t s_hciUart_cb;
/** DMA rx handle */
static dma_handle_t s_dmaHandleRx;
/** DMA tx handle */
static dma_handle_t s_dmaHandleTx;
/** Buffer to store received DMAdata */
static uint8_t s_dmaReceiveBuffer[DATA_BUFFER_SIZE];
/** Pointer to DMA read location in s_dmaReceiveBuffer*/
static uint32_t s_dmaReceivePos                                              = 0;
/** Array that stores next DMA descriptor */
static __attribute__((aligned(16))) dma_descriptor_t s_nextDescr[2] = {0};
/** Function pointer to store registered prefilter function */
ll_uart_write_prefilter_t s_uartTxPreFilter;

/*******************************************************************************
 * Code
 ******************************************************************************/

static void HciUartRxTxHandler(void *par1, void *par2)
{
    uint32_t remainingBytes = DMA_GetRemainingBytes(DMA0, HCI_UART_DMA_CHANNEL_RX);

    /* the amount of bytes received is (remaining bytes at last reading - actual remaining bytes) modulo
     * DATA_BUFFER_SIZE */
    int32_t amount = (DATA_BUFFER_SIZE - (int32_t)remainingBytes) - (int32_t)s_dmaReceivePos;
    if (amount < 0)
        amount += DATA_BUFFER_SIZE;

    /* If new data arrived. */
    while (amount > 0)
    {
        s_uReadBusBuffer[s_uReadPos++] = s_dmaReceiveBuffer[s_dmaReceivePos++];
        s_dmaReceivePos &= (DATA_BUFFER_SIZE - 1);
        amount--;

        if (s_uReadPos == s_uReadSize)
        {
            s_uReadBusBuffer = NULL;
            if (s_hciUart_cb.rxCb != NULL)
            {
                s_hciUart_cb.rxCb();
            }
        }
    }
}

/* User callback function for DMA transfer. */
static void DMA_Callback(dma_handle_t *handle, void *param, bool transferDone, uint32_t tcds)
{
    if (transferDone && s_hciUart_cb.txCb != NULL)
    {
        s_hciUart_cb.txCb();
    }
}

/* Function called by LL that needs to be implemented by application */
void ll_uart_init(hci_uart_cb_t uartCb)
{
    dma_transfer_config_t transferConfig;
    usart_config_t usartConfig;
    s_hciUart_cb = uartCb;

    RESET_PeripheralReset(HCI_UART_RESET);
    USART_GetDefaultConfig(&usartConfig);
    usartConfig.enableTx     = true;
    usartConfig.enableRx     = true;
    usartConfig.baudRate_Bps = HCI_UART_BAUDRATE;
#if (defined(HCI_BLACKBOX_ENABLE_UART_HW_FLOW_CONTROL) && (HCI_BLACKBOX_ENABLE_UART_HW_FLOW_CONTROL > 0U))
    usartConfig.enableHardwareFlowControl = true;
#endif

    USART_Init(HCI_UART, &usartConfig, 12000000u);
    USART_EnableRxDMA(HCI_UART, true);
    USART_EnableTxDMA(HCI_UART, true);

    USART_ClearStatusFlags(HCI_UART, kUSART_TxError | kUSART_RxError);
    USART_EnableInterrupts(HCI_UART, kUSART_RxLevelInterruptEnable);

    FLEXCOMM_SetIRQHandler(HCI_UART, (void *)HciUartRxTxHandler, NULL);
    /* Enable interrupt in NVIC. */
    (void)EnableIRQ(HCI_UART_IRQ);

    /* Configure DMA RX */
    DMA_Init(DMA0);
    DMA_EnableChannel(DMA0, HCI_UART_DMA_CHANNEL_RX);
    DMA_CreateHandle(&s_dmaHandleRx, DMA0, HCI_UART_DMA_CHANNEL_RX);
    DMA_PrepareTransfer(&transferConfig, (void *)&HCI_UART->FIFORD, s_dmaReceiveBuffer, 1, DATA_BUFFER_SIZE,
                        kDMA_PeripheralToMemory, &s_nextDescr[1]);
    DMA_SubmitTransfer(&s_dmaHandleRx, &transferConfig);

    transferConfig.xfercfg.intA = false;
    transferConfig.xfercfg.intB = false;
    DMA_CreateDescriptor(&s_nextDescr[1], &transferConfig.xfercfg, (void *)&HCI_UART->FIFORD, &s_dmaReceiveBuffer[0],
                         &s_nextDescr[0]);
    DMA_CreateDescriptor(&s_nextDescr[0], &transferConfig.xfercfg, (void *)&HCI_UART->FIFORD, &s_dmaReceiveBuffer[0],
                         &s_nextDescr[1]);

    DMA_StartTransfer(&s_dmaHandleRx);

    /* Configure DMA TX */
    DMA_EnableChannel(DMA0, HCI_UART_DMA_CHANNEL_TX);
    DMA_CreateHandle(&s_dmaHandleTx, DMA0, HCI_UART_DMA_CHANNEL_TX);
    DMA_SetCallback(&s_dmaHandleTx, DMA_Callback, NULL);
}

void ll_uart_deinit(void)
{
    /* Disable interrupt in NVIC. */
    (void)DisableIRQ(HCI_UART_IRQ);
    USART_Deinit(HCI_UART);
    /* Disable NVIC interrupt */
    (void)DisableIRQ(DMA0_IRQn);
    DMA_Deinit(DMA0);
}

/* Function called by LL that needs to be implemented by application */
void ll_uart_read(uint8_t *buffer, uint16_t size)
{
    s_uReadSize      = size;
    s_uReadBusBuffer = buffer;
    s_uReadPos       = 0;
}

void ll_uart_register_write_prefilter(ll_uart_write_prefilter_t write_prefilter)
{
    s_uartTxPreFilter = write_prefilter;
}

void ll_uart_unregister_write_prefilter(void)
{
    s_uartTxPreFilter = NULL;
}

/* Function called by LL that needs to be implemented by application */
void ll_uart_write(uint8_t *buffer, uint16_t size)
{
    static uint8_t ll_to_host_buf[LL_TO_HOST_BUF_SIZE];

    if (s_uartTxPreFilter != NULL) {
        /* 
           Per HCI package this function is called twice. 
           Once with the first byte and the second time with the rest of the buffer
           The complete packet is needed by the prefilter to determine
           whether it blocks the sending via the UART so we store the first byte
           notify the layer above that the transfer is done and wait for the rest
        */
        if (size == 1) {
            ll_to_host_buf[0] = buffer[0];
            s_hciUart_cb.txCb();
            return;
        }
        
        /* 
            When the rest of the buffer comes we feed the whole buffer to the prefilter.
        */
        memcpy(&ll_to_host_buf[1], buffer, size);
        size = size + 1;
        buffer = ll_to_host_buf;
        hci_uart_prefilter_t filter = s_uartTxPreFilter(buffer, size);
        /*
            If the prefilter blocks the package we notify that the transfer is done and return.
        */
        if (filter == kPREFILTER_BLOCK_SENDING) {
            s_hciUart_cb.txCb();
            return;
        }
        /*
            Otherwise we send the whole buffer via UART
        */
    }
    
    dma_channel_config_t transferConfig;

    DMA_PrepareChannelTransfer(&transferConfig, buffer, (void *)&HCI_UART->FIFOWR,
                               DMA_CHANNEL_XFER(false, true, true, false, 1, kDMA_AddressInterleave1xWidth,
                                                kDMA_AddressInterleave0xWidth, size),
                               kDMA_MemoryToPeripheral, NULL, NULL);
    DMA_SubmitChannelTransfer(&s_dmaHandleTx, &transferConfig);

    DMA_StartTransfer(&s_dmaHandleTx);
}
