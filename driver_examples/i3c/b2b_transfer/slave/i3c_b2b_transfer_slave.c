/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*  Standard C Included Files */
#include <string.h>
/*  SDK Included Files */
#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_i3c_edma.h"
#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define I3C_MASTER_SLAVE_ADDR_7BIT 0x1EU
#define I3C_VENDOR_ID              0x123U
#define I3C_DATA_LENGTH            30U

/* HDR-DDR delivers the controller's command byte inline as the first received byte, so an
   HDR write lands as [command, data...] while an SDR write lands as [data...]. The receive
   buffer is sized for the larger HDR framing (one command byte + the data). */
#define I3C_HDR_PACKET_LENGTH (I3C_DATA_LENGTH + 1U)

/*******************************************************************************
 * Variables
 ******************************************************************************/
AT_NONCACHEABLE_SECTION(static uint8_t g_slave_buff[I3C_HDR_PACKET_LENGTH]);
static i3c_slave_edma_handle_t g_i3c_s_handle;
static edma_handle_t g_tx_dma_handle;
static edma_handle_t g_rx_dma_handle;
static edma_config_t g_edma_config;
volatile bool g_slaveCompletionFlag  = false;
volatile bool g_slaveRequestSentFlag = false;

/*******************************************************************************
 * Code
 ******************************************************************************/
static void i3c_slave_callback(I3C_Type *base, i3c_slave_edma_transfer_t *xfer, void *userData)
{
    switch ((uint32_t)xfer->event)
    {
        case kI3C_SlaveCompletionEvent:
            if (xfer->completionStatus == kStatus_Success)
            {
                g_slaveCompletionFlag = true;
            }
            break;

        case kI3C_SlaveRequestSentEvent:
            g_slaveRequestSentFlag = true;
            break;

#if defined(I3C_ASYNC_WAKE_UP_INTR_CLEAR)
        case kI3C_SlaveAddressMatchEvent:
            I3C_ASYNC_WAKE_UP_INTR_CLEAR
            break;
#endif

        default:
            break;
    }
}

static status_t app_slave_arm(uint8_t *rxData, size_t rxSize, uint8_t *txData, size_t txSize, uint32_t eventMask)
{
    i3c_slave_edma_transfer_t slaveXfer;

    (void)memset(&slaveXfer, 0, sizeof(slaveXfer));
    slaveXfer.rxData      = rxData;
    slaveXfer.rxDataSize  = rxSize;
    slaveXfer.txData      = txData;
    slaveXfer.txDataSize  = txSize;
    g_slaveCompletionFlag = false;
    return I3C_SlaveTransferEDMA(EXAMPLE_SLAVE, &g_i3c_s_handle, &slaveXfer, eventMask);
}

/*!
 * @brief Runs one receive-then-echo exchange.
 *
 * @a headerBytes is the number of leading framing bytes the controller's mode prepends
 * before the data: 0 for SDR, 1 for HDR-DDR (the inline command byte). The data length is
 * fixed at #I3C_DATA_LENGTH. The slave receives the packet, presents the data for the
 * controller's read-back, and notifies the controller with an IBI carrying the length.
 */
static status_t app_slave_exchange(uint32_t headerBytes, uint32_t eventMask)
{
    status_t result;
    uint8_t dataLen = I3C_DATA_LENGTH;

    (void)memset(g_slave_buff, 0, sizeof(g_slave_buff));
    result = app_slave_arm(g_slave_buff, I3C_DATA_LENGTH + headerBytes, NULL, 0U, eventMask);
    if (result != kStatus_Success)
    {
        return result;
    }
    while (!g_slaveCompletionFlag)
    {
    }

    PRINTF("\r\nSlave received %d bytes :", (int)dataLen);
    for (uint32_t i = 0U; i < dataLen; i++)
    {
        if (i % 8U == 0U)
        {
            PRINTF("\r\n");
        }
        PRINTF("0x%2x  ", g_slave_buff[headerBytes + i]);
    }
    PRINTF("\r\n");

    /* Arm the transmit echo (with the request-sent event) and notify the controller that
       data is ready for read-back; the IBI payload is the length. The transmit is armed
       once: the controller releases the bus after the IBI (its message-mode IBI service
       emits a STOP), so the read-back starts cleanly at the first data byte. */
    result =
        app_slave_arm(NULL, 0U, &g_slave_buff[headerBytes], dataLen, eventMask | (uint32_t)kI3C_SlaveRequestSentEvent);
    if (result != kStatus_Success)
    {
        return result;
    }
    g_slaveRequestSentFlag = false;
    I3C_SlaveRequestIBIWithData(EXAMPLE_SLAVE, &dataLen, 1U);
    while (!g_slaveRequestSentFlag)
    {
    }

    while (!g_slaveCompletionFlag)
    {
    }
    return kStatus_Success;
}

/*!
 * @brief Main function
 */
int main(void)
{
    uint32_t eventMask = (uint32_t)kI3C_SlaveCompletionEvent;
#if defined(I3C_ASYNC_WAKE_UP_INTR_CLEAR)
    eventMask |= (uint32_t)kI3C_SlaveAddressMatchEvent;
#endif
    i3c_slave_config_t slaveConfig;

    BOARD_InitHardware();

    PRINTF("MCUX SDK version: %s\r\n", MCUXSDK_VERSION_FULL_STR);

    PRINTF("\r\nI3C bus transfer example -- Slave.\r\n");

    /* The default config enables HDR-DDR (hdrMode = kI3C_HDRModeDDR), so the slave accepts
       both SDR and HDR-DDR message-mode traffic from the controller. */
    I3C_SlaveGetDefaultConfig(&slaveConfig);
    slaveConfig.staticAddr = I3C_MASTER_SLAVE_ADDR_7BIT;
    slaveConfig.vendorID   = I3C_VENDOR_ID;
    slaveConfig.offline    = false;
    I3C_SlaveInit(EXAMPLE_SLAVE, &slaveConfig, I3C_SLAVE_CLOCK_FREQUENCY);

    /* DMA-service the slave data path so it keeps up with the message / HDR data rate. */
    EDMA_GetDefaultConfig(&g_edma_config);
#if defined(BOARD_GetEDMAConfig)
    BOARD_GetEDMAConfig(g_edma_config);
#endif
    EDMA_Init(EXAMPLE_DMA, &g_edma_config);
    EDMA_CreateHandle(&g_tx_dma_handle, EXAMPLE_DMA, EXAMPLE_I3C_TX_DMA_CHANNEL);
    EDMA_CreateHandle(&g_rx_dma_handle, EXAMPLE_DMA, EXAMPLE_I3C_RX_DMA_CHANNEL);
    EDMA_SetChannelMux(EXAMPLE_DMA, EXAMPLE_I3C_TX_DMA_CHANNEL, EXAMPLE_I3C_TX_DMA_CHANNEL_MUX);
    EDMA_SetChannelMux(EXAMPLE_DMA, EXAMPLE_I3C_RX_DMA_CHANNEL, EXAMPLE_I3C_RX_DMA_CHANNEL_MUX);
    I3C_SlaveTransferCreateHandleEDMA(EXAMPLE_SLAVE, &g_i3c_s_handle, i3c_slave_callback, NULL, &g_rx_dma_handle,
                                      &g_tx_dma_handle);

    /* SDR exchange: no framing byte, the data lands at buff[0]. */
    PRINTF("\r\nCheck I3C master SDR transfer.\r\n");
    if (app_slave_exchange(0U, eventMask) != kStatus_Success)
    {
        PRINTF("\r\nSlave SDR exchange failed.\r\n");
        return -1;
    }
    PRINTF("\r\nI3C master SDR transfer finished.\r\n");

    /* HDR-DDR exchange: 1 framing byte (the inline command byte), data at buff[1]. */
    PRINTF("\r\nCheck I3C master HDR-DDR transfer.\r\n");
    if (app_slave_exchange(1U, eventMask) != kStatus_Success)
    {
        PRINTF("\r\nSlave HDR-DDR exchange failed.\r\n");
        return -1;
    }
    PRINTF("\r\nI3C master HDR-DDR transfer finished.\r\n");

    while (1)
    {
    }
}
