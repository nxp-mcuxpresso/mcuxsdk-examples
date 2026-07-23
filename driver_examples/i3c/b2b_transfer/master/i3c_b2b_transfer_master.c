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
#include "fsl_i3c.h"
#include "fsl_edma.h"
#include "fsl_i3c_msg_dma.h"
#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define CCC_RSTDAA  0x06U
#define CCC_SETDASA 0x87U

#define I3C_BROADCAST_ADDR         0x7EU
#define I3C_MASTER_SLAVE_ADDR_7BIT 0x1EU
#define I3C_SLAVE_DYNAMIC_ADDR     0x30U
#define I3C_DATA_LENGTH            30U

/* HDR-DDR 7-bit command code for the message command word. The read/write direction bit
   is set by the driver from the transfer direction, only the command code is given. */
#define I3C_HDR_CMD 0x00U

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* Message data buffers, 2-byte aligned and non-cacheable as required by the DMA
   message data ports. */
AT_NONCACHEABLE_SECTION_ALIGN(static uint8_t g_master_txBuff[I3C_DATA_LENGTH], 2U);
AT_NONCACHEABLE_SECTION_ALIGN(static uint8_t g_master_rxBuff[I3C_DATA_LENGTH], 2U);
static uint8_t g_master_ibiBuff[10];

static volatile bool g_msgDmaComplFlag       = false;
static volatile status_t g_msgDmaComplStatus = kStatus_Success;
static volatile bool g_ibiWonFlag            = false;
static volatile uint8_t g_ibiDataSize        = 0U;
static i3c_master_msg_dma_handle_t g_i3c_msg_dma_handle;
static edma_handle_t g_tx_edma_handle;
static edma_handle_t g_rx_edma_handle;

/*******************************************************************************
 * Code
 ******************************************************************************/
static void i3c_master_msg_dma_callback(I3C_Type *base,
                                        i3c_master_msg_dma_handle_t *handle,
                                        status_t status,
                                        void *userData)
{
    (void)base;
    (void)handle;
    (void)userData;

    g_msgDmaComplStatus = status;
    g_msgDmaComplFlag   = true;
}

static void i3c_master_msg_dma_ibi_callback(I3C_Type *base,
                                            i3c_master_msg_dma_handle_t *handle,
                                            i3c_ibi_type_t ibiType,
                                            i3c_ibi_state_t ibiState)
{
    uint8_t payloadSize = 0U;

    (void)base;
    (void)ibiType;

    if (ibiState == kI3C_IbiDataBuffNeed)
    {
        handle->ibiBuff = g_master_ibiBuff;
    }
    else if (ibiState == kI3C_IbiReady)
    {
        if ((handle->ibiPayloadSize > 0U) && (NULL != handle->ibiBuff))
        {
            payloadSize = handle->ibiBuff[0];
        }

        g_ibiDataSize = (payloadSize <= I3C_DATA_LENGTH) ? payloadSize : 0U;
        g_ibiWonFlag  = true;
    }
    else
    {
        /* No action for other IBI states. */
    }
}

static const i3c_master_msg_dma_callback_t masterMsgDmaCallback = {
    .transferComplete = i3c_master_msg_dma_callback,
    .ibiCallback      = i3c_master_msg_dma_ibi_callback,
};

/*!
 * @brief Runs one message-mode DMA transfer and waits for its completion.
 */
static status_t app_msg_dma_transfer(i3c_master_msg_t *msg, i3c_bus_type_t busType)
{
    i3c_master_msg_transfer_t xfer = {0};
    status_t result;

    assert(NULL != msg);

    xfer.msgList     = msg;
    xfer.msgCount    = 1U;
    xfer.busType     = busType;
    xfer.ibiResponse = kI3C_IbiRespNack;

    g_msgDmaComplFlag = false;
    result            = I3C_MasterMsgTransferDMA(EXAMPLE_MASTER, &g_i3c_msg_dma_handle, &xfer);
    if (kStatus_Success != result)
    {
        return result;
    }
    while (!g_msgDmaComplFlag)
    {
    }
    return g_msgDmaComplStatus;
}

static status_t app_write_read(uint8_t slaveAddr, i3c_bus_type_t busType, uint8_t command, size_t length)
{
    i3c_master_msg_t msg           = {0};
    uint8_t ibiDataSize;
    status_t result;

    g_ibiWonFlag = false;

    msg.slaveAddress = slaveAddr;
    msg.direction    = kI3C_Write;
    msg.data         = g_master_txBuff;
    msg.dataSize     = length;
    if (busType == kI3C_TypeI3CDdr)
    {
        msg.command = command;
    }

    result = app_msg_dma_transfer(&msg, busType);
    if (kStatus_Success != result)
    {
        (void)PRINTF("\r\nMessage write failed with error %u.\r\n", result);
        return result;
    }

    /* Target prepares the data and requests controller with IBI. */
    while (!g_ibiWonFlag)
    {
    }
    g_ibiWonFlag = false;

    ibiDataSize = g_ibiDataSize;
    (void)PRINTF("\r\nController received target IBI, data size %d.\r\n", (int)ibiDataSize);
    if ((ibiDataSize == 0U) || ((size_t)ibiDataSize > length))
    {
        return kStatus_Fail;
    }

    (void)memset(g_master_rxBuff, 0, length);

    msg.direction    = kI3C_Read;
    msg.data         = g_master_rxBuff;
    msg.dataSize     = (size_t)ibiDataSize;

    result = app_msg_dma_transfer(&msg, busType);
    if (kStatus_Success != result)
    {
        (void)PRINTF("\r\nMessage read failed with error %u.\r\n", result);
        return result;
    }

    (void)PRINTF("\r\nReceive sent data from slave :");
    for (uint32_t i = 0U; i < (uint32_t)msg.dataSize; i++)
    {
        if (i % 8U == 0U)
        {
            (void)PRINTF("\r\n");
        }
        (void)PRINTF("0x%2x  ", g_master_rxBuff[i]);
    }
    (void)PRINTF("\r\n");

    for (uint32_t i = 0U; i < (uint32_t)msg.dataSize; i++)
    {
        if (g_master_rxBuff[i] != g_master_txBuff[i])
        {
            (void)PRINTF("\r\nData mismatch in transfer!\r\n");
            return kStatus_Fail;
        }
    }

    return kStatus_Success;
}

/*!
 * @brief Main function
 */
int main(void)
{
    status_t result = kStatus_Success;
    i3c_register_ibi_addr_t ibiRecord;
    i3c_master_config_t masterConfig;
    i3c_master_transfer_t masterXfer;
    edma_config_t edmaConfig;
    uint8_t slaveAddr;

    BOARD_InitHardware();

    (void)PRINTF("MCUX SDK version: %s\r\n", MCUXSDK_VERSION_FULL_STR);

    (void)PRINTF("\r\nI3C bus transfer example -- Master.\r\n");

    I3C_MasterGetDefaultConfig(&masterConfig);
    masterConfig.baudRate_Hz.i2cBaud          = EXAMPLE_I2C_BAUDRATE;
    masterConfig.baudRate_Hz.i3cPushPullBaud  = EXAMPLE_I3C_PP_BAUDRATE;
    masterConfig.baudRate_Hz.i3cOpenDrainBaud = EXAMPLE_I3C_OD_BAUDRATE;
    masterConfig.enableOpenDrainStop          = false;
    masterConfig.disableTimeout               = true;
    I3C_MasterInit(EXAMPLE_MASTER, &masterConfig, I3C_MASTER_CLOCK_FREQUENCY);

    (void)PRINTF("\r\nI3C master do dynamic address assignment to the I3C slaves on bus.");

    (void)memset(&masterXfer, 0, sizeof(masterXfer));
    masterXfer.slaveAddress   = I3C_BROADCAST_ADDR;
    masterXfer.subaddress     = CCC_RSTDAA;
    masterXfer.subaddressSize = 1U;
    masterXfer.direction      = kI3C_Write;
    masterXfer.busType        = kI3C_TypeI3CSdr;
    masterXfer.flags          = (uint32_t)kI3C_TransferDefaultFlag;
    masterXfer.ibiResponse    = kI3C_IbiRespAckMandatory;
    result                    = I3C_MasterTransferBlocking(EXAMPLE_MASTER, &masterXfer);
    if (kStatus_Success != result)
    {
        (void)PRINTF("\r\nRSTDAA CCC transfer failed with error %u.\r\n", result);
        return -1;
    }

#if defined(EXAMPLE_USE_SETDASA_ASSIGN_ADDR) && (EXAMPLE_USE_SETDASA_ASSIGN_ADDR)
    (void)memset(&masterXfer, 0, sizeof(masterXfer));
    masterXfer.slaveAddress   = I3C_BROADCAST_ADDR;
    masterXfer.subaddress     = CCC_SETDASA;
    masterXfer.subaddressSize = 1U;
    masterXfer.direction      = kI3C_Write;
    masterXfer.busType        = kI3C_TypeI3CSdr;
    masterXfer.flags          = (uint32_t)kI3C_TransferNoStopFlag;
    masterXfer.ibiResponse    = kI3C_IbiRespAckMandatory;
    result                    = I3C_MasterTransferBlocking(EXAMPLE_MASTER, &masterXfer);
    if (kStatus_Success != result)
    {
        (void)PRINTF("\r\nSETDASA CCC transfer failed with error %u.\r\n", result);
        return -1;
    }

    /* Assign the slave a dynamic address distinct from its 0x1E static address (identify
       the device by its static address, then hand it the new dynamic address). */
    slaveAddr = I3C_SLAVE_DYNAMIC_ADDR;
    (void)memset(&masterXfer, 0, sizeof(masterXfer));
    masterXfer.slaveAddress   = I3C_MASTER_SLAVE_ADDR_7BIT;
    masterXfer.subaddress     = (uint32_t)slaveAddr << 1U;
    masterXfer.subaddressSize = 1U;
    masterXfer.direction      = kI3C_Write;
    masterXfer.busType        = kI3C_TypeI3CSdr;
    masterXfer.flags          = (uint32_t)kI3C_TransferDefaultFlag;
    masterXfer.ibiResponse    = kI3C_IbiRespAckMandatory;
    result                    = I3C_MasterTransferBlocking(EXAMPLE_MASTER, &masterXfer);
    if (kStatus_Success != result)
    {
        (void)PRINTF("\r\nSETDASA address assignment failed with error %u.\r\n", result);
        return -1;
    }
#else
    uint8_t addressList[8] = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38};
    result                 = I3C_MasterProcessDAA(EXAMPLE_MASTER, addressList, sizeof(addressList));
    if (result != kStatus_Success)
    {
        (void)PRINTF("\r\nENTDAA with error %u.\r\n", result);
        return -1;
    }

    i3c_device_info_t *devList;
    uint8_t devIndex;
    uint8_t devCount;
    devList = I3C_MasterGetDeviceListAfterDAA(EXAMPLE_MASTER, &devCount);
    for (devIndex = 0; devIndex < devCount; devIndex++)
    {
        if (devList[devIndex].vendorID == 0x123U)
        {
            slaveAddr = devList[devIndex].dynamicAddr;
            break;
        }
    }
    if (devIndex == devCount)
    {
        (void)PRINTF("\r\nI3C master dynamic address assignment fails!\r\n");
        return -1;
    }
#endif

    (void)PRINTF("\r\nI3C master dynamic address assignment done.\r\n");

    EDMA_GetDefaultConfig(&edmaConfig);
#if defined(BOARD_GetEDMAConfig)
    BOARD_GetEDMAConfig(edmaConfig);
#endif
    EDMA_Init(EXAMPLE_DMA, &edmaConfig);
    (void)EDMA_CreateHandle(&g_tx_edma_handle, EXAMPLE_DMA, EXAMPLE_I3C_TX_DMA_CHANNEL);
    (void)EDMA_CreateHandle(&g_rx_edma_handle, EXAMPLE_DMA, EXAMPLE_I3C_RX_DMA_CHANNEL);
    EDMA_SetChannelMux(EXAMPLE_DMA, EXAMPLE_I3C_TX_DMA_CHANNEL, (uint32_t)EXAMPLE_I3C_TX_DMA_CHANNEL_MUX);
    EDMA_SetChannelMux(EXAMPLE_DMA, EXAMPLE_I3C_RX_DMA_CHANNEL, (uint32_t)EXAMPLE_I3C_RX_DMA_CHANNEL_MUX);
    I3C_MasterMsgTransferCreateHandleDMA(EXAMPLE_MASTER, &g_i3c_msg_dma_handle, &masterMsgDmaCallback, NULL,
                                         &g_rx_edma_handle, &g_tx_edma_handle);

    (void)memset(&ibiRecord, 0, sizeof(ibiRecord));
    ibiRecord.address[0]    = slaveAddr;
    ibiRecord.ibiHasPayload = true;
    I3C_MasterRegisterIBI(EXAMPLE_MASTER, &ibiRecord);

    for (uint32_t i = 0U; i < I3C_DATA_LENGTH; i++)
    {
        g_master_txBuff[i] = (uint8_t)i;
    }

    (void)PRINTF("\r\nMaster will send data :");
    for (uint32_t i = 0U; i < I3C_DATA_LENGTH; i++)
    {
        if (i % 8U == 0U)
        {
            (void)PRINTF("\r\n");
        }
        (void)PRINTF("0x%2x  ", g_master_txBuff[i]);
    }
    (void)PRINTF("\r\n");

    /* SDR message-mode write + IBI-synchronized read-back. */
    (void)PRINTF("\r\nStart I3C master transfer in SDR mode.\r\n");
    result = app_write_read(slaveAddr, kI3C_TypeI3CSdr, 0U, I3C_DATA_LENGTH);
    if (kStatus_Success != result)
    {
        return -1;
    }
    (void)PRINTF("\r\nI3C master transfer successful in SDR mode.\r\n");

    /* HDR-DDR message-mode write + IBI-synchronized read-back. */
    (void)PRINTF("\r\nStart I3C master transfer in HDR-DDR mode.\r\n");
    result = app_write_read(slaveAddr, kI3C_TypeI3CDdr, I3C_HDR_CMD, I3C_DATA_LENGTH);
    if (kStatus_Success != result)
    {
        return -1;
    }
    (void)PRINTF("\r\nI3C master transfer successful in HDR-DDR mode.\r\n");

    (void)PRINTF("\r\nI3C bus transfer example -- all transfers successful!\r\n");

    while (1)
    {
    }
}
