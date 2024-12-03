/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"
#include "app.h"
#include "fsl_hostif.h"
#include "fsl_debug_console.h"
#include "Pcrm_Lp_Reg.h"
#include "PN76_Reg_Interface.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define HIFEX_TLV_TYPE_INDEX                       (0U)
#define TLV_DISPATCHER_BUFF_SIZE                   (64)
#define HIFEX_TLV_WRITE_REGISTER_CMD_LEN           (9)            /* T(1) + L(2) + V(6)[RegAddr(2) + Value(4)]*/
#define HIFEX_TLV_READ_REGISTER_CMD_LEN            (5)            /* T(1) + L(2) + V(2)[RegAddr(2)]*/
#define HIFEX_TLV_CMD_LEN_MIN                      (3)            /* Minimum length of TLV Command */
#define HOSTIF_CONTROL_REG                         (0x40000004UL) /* rw Control */
#define HOSTIF_CONTROL_REG_EXT_IRQ_CTRL_ENABLE_POS (7UL)
#define PCRM_SETBITN(RegAddr, BitPos)              PN76_Sys_WriteRegisterOrMask(RegAddr, (1 << BitPos)) /* Macro to set a bit */
static inline void AD_NFC_IRQ_Init()
{
    PCRM_SETBITN(PCRM_PAD_IRQ, PCRM_PAD_IRQ_IRQ_ENABLE_POS);
    PH_HALREG_SETBITN(HOSTIF_CONTROL_REG, HOSTIF_CONTROL_REG_EXT_IRQ_CTRL_ENABLE_POS);
}

typedef enum
{
    HIFEX_TLV_LOOPBACK       = 0x00,
    HIFEX_TLV_WRITE_REGISTER = 0x01,
    HIFEX_TLV_READ_REGISTER  = 0x02,
    HIFEX_TLV_EVENT          = 0x80
} tlvCommandCodes_t;

typedef enum
{
    HIFEX_RSP_ALL_OK                      = 0x00,
    HIFEX_RSP_TIMEOUT                     = 0X01,
    HIFEX_RSP_STATUS_INVALID_CMD          = 0x05,
    HIFEX_RSP_RX_TIMEOUT                  = 0x11,
    HIFEX_RSP_STATUS_INSTR_ERROR          = 0x18,
    HIFEX_RSP_STATUS_NO_EXTERNAL_RF_FIELD = 0x10,
    HIFEX_RSP_STATUS_USER_CANCELLED       = 0x12,
    HIFEX_RSP_STATUS_PREVENT_STANDBY      = 0x13,
    HIFEX_RSP_STATUS_SUCCSES_CHAINING     = 0xAF
} tlvResponseCodes_t;

typedef enum
{
    P_SEND = 0x0,
    P_RECEIVE
} printf_flag_t;
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void tlv_dispatcher(void);
static status_t send_boot_evt(void);
static uint32_t ReceiveTlvCommand(void);
static status_t I2C_Receive_Blocking(hostif_handle_t *handle, uint8_t count, uint32_t *numOfReveive);
static status_t I2C_Send_Blocking(hostif_handle_t *handle);
static void Tlv_Process(uint32_t dwCommandLen, uint32_t *pdwResponseLen);
static uint32_t HifEx_WriteRegister(uint32_t dwCommandLen);
static uint32_t HifEx_ReadRegister(uint32_t dwCommandLen);
static uint32_t HifEx_LoopBack(uint32_t dwCommandLen);
static uint32_t HifEx_UnknownCmd();
static status_t SendTlvResponse(uint32_t dwBufferSize);
void HIF_I2C_GetDefaultConfig(hostif_config_t *hifI2cConfig);
static void Do_Printf(uint8_t *buffer, uint32_t bufferLen, printf_flag_t flag);
/*******************************************************************************
 * Variables
 ******************************************************************************/
static volatile bool hifI2cFinished                            = false;
static hostif_handle_t g_hifI2cHandle                          = {0};
static uint32_t gaRxBufferRX0_DataReceived                     = 0U;
static hostif_transfer_t transfer[DEMO_HOSTIF_RX_BUFFER_COUNT] = {0};
static uint8_t gabDispBuffer[TLV_DISPATCHER_BUFF_SIZE] __attribute__((aligned(4)));
/*******************************************************************************
 * Code
 ******************************************************************************/
void hostif_callback(hostif_handle_t *handle, void *userData, uint32_t transferredLength, uint32_t status)
{
    switch (status)
    {
        case kHOSTIF_EndOfReception:
        {
            gaRxBufferRX0_DataReceived += transferredLength;
            PRINTF("End Of Reception!\r\n");
            hifI2cFinished = true;
            break;
        }
        case kHOSTIF_TransferError:
        {
            PRINTF("Transfer Error!\r\n");
            break;
        }
        case kHOSTIF_ReachedWaterLevel:
        {
            PRINTF("Reached WaterLevel!\r\n");
            break;
        }
        case kHOSTIF_EndOfTransmit:
        {
            PRINTF("End Of Transmit!\r\n");
            hifI2cFinished = true;
            break;
        }
        default:
            PRINTF("Unknown Interrupt!\r\n");
    }
}

static void tlv_dispatcher()
{
    uint32_t dwCommandLen = 0;
    uint32_t dwResponseLen;
    status_t bStatus = kStatus_Fail;

    PRINTF("\nTLV Dispatcher Running...");

    /* Send Boot TLV Event Packet to indicate IC booted to Normal operation mode */
    bStatus = send_boot_evt();
    if (bStatus != kStatus_Success)
    {
        PRINTF("\nBoot Event Not read by Host. Host not running?");
    }

    while (1)
    {
        dwCommandLen = ReceiveTlvCommand();
        if (dwCommandLen != 0)
        {
            Do_Printf(transfer[0].startAddress, dwCommandLen, P_RECEIVE);
            Tlv_Process(dwCommandLen, &dwResponseLen);
            SendTlvResponse(dwResponseLen);
        }
    }
}

static status_t send_boot_evt()
{
    status_t bStatus;
    uint32_t dwByteIndex;
    uint32_t dwRegVal = 0;
    uint8_t abCmd[16];

    do
    {
        /* Read SYS_BOOT1_STATUS Logical register */
        bStatus = PN76_Sys_ReadRegister(PCRM_SYS_BOOT1_STS, &dwRegVal);
        if (bStatus != kStatus_Success)
            break;

        /* Create TLV Boot Event Packet */
        dwByteIndex          = HIFEX_TLV_TYPE_INDEX;
        abCmd[dwByteIndex++] = HIFEX_TLV_EVENT; /* Type Byte */
        abCmd[dwByteIndex++] = 0x00;            /* Len MSB Byte */
        abCmd[dwByteIndex++] = 0x08; /* Len LSB Byte - Payload contains 4 byte Event Status and 4 byte Event Data */
        abCmd[dwByteIndex++] = 0x01; /* Payload - Event Status LSB. Set Boot Event */
        abCmd[dwByteIndex++] = 0x00; /* Payload - Event Status byte */
        abCmd[dwByteIndex++] = 0x00; /* Payload - Event Status byte */
        abCmd[dwByteIndex++] = 0x00; /* Payload - Event Status MSB */
        abCmd[dwByteIndex++] = (uint8_t)dwRegVal;
        abCmd[dwByteIndex++] = (uint8_t)(dwRegVal >> 8);
        abCmd[dwByteIndex++] = (uint8_t)(dwRegVal >> 16);
        abCmd[dwByteIndex++] = (uint8_t)(dwRegVal >> 24);

    } while (0);

    if (bStatus != kStatus_Success)
    {
        dwRegVal = 0;
        /* Create TLV Boot Event Packet */
        dwByteIndex          = HIFEX_TLV_TYPE_INDEX;
        abCmd[dwByteIndex++] = HIFEX_TLV_EVENT; /* Type Byte */
        abCmd[dwByteIndex++] = 0x00;            /* Len MSB Byte */
        abCmd[dwByteIndex++] = 0x0C; /* Len LSB Byte - Payload contains 4 byte Event Status and 4 byte Event Data and 4
                                        byte General Error Status */
        abCmd[dwByteIndex++] = 0x03; /* Payload - Event Status LSB. Set Boot and Error Event */
        abCmd[dwByteIndex++] = 0x00; /* Payload - Event Status byte */
        abCmd[dwByteIndex++] = 0x00; /* Payload - Event Status byte */
        abCmd[dwByteIndex++] = 0x00; /* Payload - Event Status MSB */
        abCmd[dwByteIndex++] = (uint8_t)dwRegVal; /* Payload - Event Data LSB. */
        abCmd[dwByteIndex++] = (uint8_t)(dwRegVal >> 8);
        abCmd[dwByteIndex++] = (uint8_t)(dwRegVal >> 16);
        abCmd[dwByteIndex++] = (uint8_t)(dwRegVal >> 24);
        abCmd[dwByteIndex++] = 0x20; /* Payload - Error Status LSB. Set General Error */
        abCmd[dwByteIndex++] = 0x00; /* Payload - Error Status byte */
        abCmd[dwByteIndex++] = 0x00; /* Payload - Error Status byte */
        abCmd[dwByteIndex++] = 0x00; /* Payload - Error Status MSB */
    }

    /* Send the Boot Event TLV packet over HIF */
    transfer[0].startAddress = (uint8_t *)abCmd;
    transfer[0].bufferLength = dwByteIndex;
    bStatus                  = HOSTIF_SendNonBlocking(&g_hifI2cHandle, transfer);
    return bStatus;
}

static uint32_t ReceiveTlvCommand(void)
{
    uint32_t dwTotalBytesRxd = 0;
    uint32_t dwExpBytes;
    uint8_t *pRxBuffer    = gabDispBuffer; /* Global Dispatcher RX Buffer */
    uint32_t dwBufferSize = TLV_DISPATCHER_BUFF_SIZE;
    status_t status;

    transfer[0].startAddress = (uint8_t *)pRxBuffer;
    transfer[0].bufferLength = dwBufferSize;

    /* Recieve TLV Command packet over HIF */
    status = I2C_Receive_Blocking(&g_hifI2cHandle, 1, &dwTotalBytesRxd);
    if (status != kStatus_Success)
    {
        dwTotalBytesRxd = 0;
    }

    if (dwTotalBytesRxd >= HIFEX_TLV_CMD_LEN_MIN)
    {
        /* Determine TLV Payload length */
        dwExpBytes = (((uint32_t)pRxBuffer[1]) << 8) | pRxBuffer[2]; /* TLV Payload length bytes are in Big Endian */
        dwExpBytes += HIFEX_TLV_CMD_LEN_MIN; /* Add header len to get total number of bytes expected */

        dwExpBytes = (dwExpBytes > dwBufferSize) ? dwBufferSize : dwExpBytes; /* Cannot read more than size of Buffer */

        if (dwExpBytes > dwTotalBytesRxd)
        {
            /* Broken packet received ! Ignore Packet. */
            dwTotalBytesRxd = 0;
        }
    }

    return dwTotalBytesRxd;
}

static status_t I2C_Receive_Blocking(hostif_handle_t *handle, uint8_t count, uint32_t *numOfReveive)
{
    status_t status;
    hifI2cFinished = false;
    do
    {
        /* Init Count for Received bytes */
        *numOfReveive = 0;
        /* Init global count for received bytes which will be updated in RX Buffer ID 0 EOR Callback */
        gaRxBufferRX0_DataReceived = 0;

        status = HOSTIF_ReceiveNonBlocking(handle, transfer, count);
        if (status != kStatus_Success)
        {
            PRINTF("Receive_Blocking ERROR\r\n");
            break;
        }
        while (hifI2cFinished != true)
        {
        }
        /* Save number of Received bytes in RX0 Buffer */
        *numOfReveive = gaRxBufferRX0_DataReceived;
    } while (0);

    /* Disable the RX Buffer RX0 */
    HOSTIF_AbortReceive(handle);
    return status;
}

status_t I2C_Send_Blocking(hostif_handle_t *handle)
{
    status_t status;
    hifI2cFinished = false;
    do
    {
        status = HOSTIF_SendNonBlocking(handle, transfer);
        if (status != kStatus_Success)
        {
            PRINTF("Send_Blocking ERROR\r\n");
            break;
        }
        while (hifI2cFinished != true)
        {
        }
    } while (0);
    return status;
}

static void Tlv_Process(uint32_t dwCommandLen, uint32_t *pdwResponseLen)
{
    uint8_t bCmdType;
    uint32_t dwTlvResponseLen;

    /* Init TLV response Length */
    *pdwResponseLen = 0;

    bCmdType = gabDispBuffer[HIFEX_TLV_TYPE_INDEX];

    switch (bCmdType)
    {
        case HIFEX_TLV_WRITE_REGISTER:
            dwTlvResponseLen = HifEx_WriteRegister(dwCommandLen);
            break;

        case HIFEX_TLV_READ_REGISTER:
            dwTlvResponseLen = HifEx_ReadRegister(dwCommandLen);
            break;

        case HIFEX_TLV_LOOPBACK:
            dwTlvResponseLen = HifEx_LoopBack(dwCommandLen);
            break;

        default:
            dwTlvResponseLen = HifEx_UnknownCmd();
    }

    *pdwResponseLen = dwTlvResponseLen;
}

static uint32_t HifEx_WriteRegister(uint32_t dwCommandLen)
{
    uint32_t dwByteIndex;
    uint32_t dwRegVal;
    PN76_Status_t bStatus;
    uint16_t wRegAddr;
    uint8_t bStatusCode;

    do
    {
        bStatusCode = HIFEX_RSP_STATUS_INSTR_ERROR;

        if (dwCommandLen != HIFEX_TLV_WRITE_REGISTER_CMD_LEN)
            break;

        /* Retrieve Logical register address and Register bank from command packet payload */
        wRegAddr = gabDispBuffer[3];
        wRegAddr |= (((uint16_t)gabDispBuffer[4]) << 8);

        /* Retrieve 32bit data to write, from command packet payload */
        dwRegVal = gabDispBuffer[5];
        dwRegVal |= (((uint32_t)gabDispBuffer[6]) << 8);
        dwRegVal |= (((uint32_t)gabDispBuffer[7]) << 16);
        dwRegVal |= (((uint32_t)gabDispBuffer[8]) << 24);

        bStatus = PN76_Sys_WriteRegister(wRegAddr, dwRegVal);
        if (bStatus != PN76_STATUS_SUCCESS)
            break;

        /* Operation success */
        bStatusCode = HIFEX_RSP_ALL_OK;

    } while (0);

    /* Create TLV Response Packet in same Dispatcher Buffer */
    dwByteIndex                  = HIFEX_TLV_TYPE_INDEX;
    gabDispBuffer[dwByteIndex++] = HIFEX_TLV_WRITE_REGISTER; /* Type Byte */
    gabDispBuffer[dwByteIndex++] = 0x00;                     /* Len MSB Byte */
    gabDispBuffer[dwByteIndex++] = 0x01;                     /* Len LSB Byte - Payload contains 1 byte Status code */
    gabDispBuffer[dwByteIndex++] = bStatusCode;              /* Payload - 1 byte Status Code */

    return dwByteIndex;
}

static uint32_t HifEx_ReadRegister(uint32_t dwCommandLen)
{
    uint32_t dwByteIndex;
    uint32_t dwRegVal;
    PN76_Status_t bStatus;
    uint16_t wRegAddr;
    uint8_t bStatusCode;

    do
    {
        bStatusCode = HIFEX_RSP_STATUS_INSTR_ERROR;

        if (dwCommandLen != HIFEX_TLV_READ_REGISTER_CMD_LEN)
            break;

        /* Retrieve Logical register address and Register bank from command packet payload */
        wRegAddr = gabDispBuffer[3];
        wRegAddr |= (((uint16_t)gabDispBuffer[4]) << 8);
        dwRegVal = 0;
        bStatus  = PN76_Sys_ReadRegister(wRegAddr, &dwRegVal);
        if (bStatus != PN76_STATUS_SUCCESS)
            break;

        /* Operation success */
        bStatusCode = HIFEX_RSP_ALL_OK;

    } while (0);

    /* Create TLV Response Packet in same Dispatcher Buffer */
    dwByteIndex                  = HIFEX_TLV_TYPE_INDEX;
    gabDispBuffer[dwByteIndex++] = HIFEX_TLV_READ_REGISTER; /* Type Byte */
    gabDispBuffer[dwByteIndex++] = 0x00;                    /* Len MSB Byte */
    if (bStatusCode == HIFEX_RSP_ALL_OK)
    {
        gabDispBuffer[dwByteIndex++] =
            0x05; /* Len LSB Byte - Payload contains 5 bytes - Status code + 32 bit reg value */
        gabDispBuffer[dwByteIndex++] = bStatusCode; /* Payload - 1 byte Status Code */
        gabDispBuffer[dwByteIndex++] = (uint8_t)dwRegVal;
        gabDispBuffer[dwByteIndex++] = (uint8_t)(dwRegVal >> 8);
        gabDispBuffer[dwByteIndex++] = (uint8_t)(dwRegVal >> 16);
        gabDispBuffer[dwByteIndex++] = (uint8_t)(dwRegVal >> 24);
    }
    else
    {
        gabDispBuffer[dwByteIndex++] = 0x01;        /* Len LSB Byte - Payload contains 1 byte Status code */
        gabDispBuffer[dwByteIndex++] = bStatusCode; /* Payload - 1 byte Status Code */
    }

    return dwByteIndex;
}

static uint32_t HifEx_LoopBack(uint32_t dwCommandLen)
{
    return dwCommandLen; /* Return Command length as response length.
                            Dispatcher buffer already contains command packet which will be sent back as response*/
}

static uint32_t HifEx_UnknownCmd()
{
    uint32_t dwByteIndex = HIFEX_TLV_TYPE_INDEX;
    /* Create TLV Response Packet */
    dwByteIndex++;                       /* Keep Type Byte as it is */
    gabDispBuffer[dwByteIndex++] = 0x00; /* Len MSB Byte */
    gabDispBuffer[dwByteIndex++] = 0x01; /* Len LSB Byte - Payload contains 1 byte - Status code */
    gabDispBuffer[dwByteIndex++] = HIFEX_RSP_STATUS_INVALID_CMD; /* Payload - 1 byte Status Code */

    return dwByteIndex;
}

static status_t SendTlvResponse(uint32_t dwBufferSize)
{
    status_t wStatus;
    // data_generate(gabDispBuffer, &dwBufferSize);
    transfer[0].startAddress = (uint8_t *)gabDispBuffer;
    transfer[0].bufferLength = dwBufferSize;
    Do_Printf(transfer[0].startAddress, transfer[0].bufferLength, P_SEND);
    wStatus = I2C_Send_Blocking(&g_hifI2cHandle);
    return wStatus;
}

void HIF_I2C_GetDefaultConfig(hostif_config_t *hifI2cConfig)
{
    HOSTIF_GetDefaultConfig(hifI2cConfig);

    hifI2cConfig->interfaceConfig.i2cConfig.enableDeviceID = 0U;
    hifI2cConfig->interfaceConfig.i2cConfig.enableHsMode   = 0U;
    hifI2cConfig->interfaceConfig.i2cConfig.enableReset    = 0U;
    hifI2cConfig->interfaceConfig.i2cConfig.slaveAddress   = 0U;
    hifI2cConfig->interfaceConfig.i2cConfig.revisionId |=
        ((0U << HOSTIF_I2C_CONTROL_I2C_REV_ID_SHIFT) | HOSTIF_I2C_CONTROL_I2C_REV_ID_MASK);
}

static void Do_Printf(uint8_t *buffer, uint32_t bufferLen, printf_flag_t flag)
{
    switch (flag)
    {
        case P_SEND:
        {
            PRINTF("Send: \r\n");
            break;
        }
        case P_RECEIVE:
        {
            PRINTF("Receied: \r\n");
            break;
        }
    }
    for (int i = 0; i < bufferLen; i++)
    {
        PRINTF("%02X\r\n", buffer[i]);
    }
}

/*!
 * @brief Main function.
 */
int main(void)
{
    /* Init data. */
    hostif_config_t hifI2cConfig = {0};
    /* Init hardware. */
    BOARD_InitHardware();
    PRINTF("HIF I2C Slave Started!\r\n");

    /* Initialize the I2C HAL */
    /*
     * hifI2cConfig.interface        = kHOSTIF_SelectI2c;
     * hifI2cConfig.timeOut          = 0U;
     * hifI2cConfig.waterMarkLevel   = 0U;
     * hifI2cConfig.enableCrc        = false;
     * hifI2cConfig.enableExtIrqCtrl = false;
     */
    HIF_I2C_GetDefaultConfig(&hifI2cConfig);
    HOSTIF_Init(HOSTIF, &hifI2cConfig);
    AD_NFC_IRQ_Init();

    HOSTIF_CreateHandle(HOSTIF, &g_hifI2cHandle, hostif_callback, NULL);

    tlv_dispatcher();
    while (1)
    {
    }
}
