/*
 * Copyright 2022 NXP
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

#include "fsl_gpio.h"
#include "PN76_Crypto.h"
#include "PN76_aesalt.h"
#include "PN76_CryptoHelper.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define HIFEX_TLV_TYPE_INDEX             (0U)
#define TLV_DISPATCHER_BUFF_SIZE         (64)
#define HIFEX_TLV_WRITE_REGISTER_CMD_LEN (9) /* T(1) + L(2) + V(6)[RegAddr(2) + Value(4)]*/
#define HIFEX_TLV_READ_REGISTER_CMD_LEN  (5) /* T(1) + L(2) + V(2)[RegAddr(2)]*/
#define HIFEX_TLV_CMD_LEN_MIN            (3) /* Minimum length of TLV Command */
#define PCRM_SETBITN(RegAddr, BitPos)    PN76_Sys_WriteRegisterOrMask(RegAddr, (1 << BitPos)) /* Macro to set a bit */

static inline void AD_NFC_IRQ_Init()
{
    PCRM_SETBITN(PCRM_PAD_IRQ, PCRM_PAD_IRQ_IRQ_ENABLE_POS);
}

typedef enum
{
    HIFEX_TLV_LOOPBACK       = 0x00,
    HIFEX_TLV_WRITE_REGISTER = 0x01,
    HIFEX_TLV_READ_REGISTER  = 0x02,
	HIFEX_ENC_DEC_DEMO       = 0x03,
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
static status_t SPI_Receive_Blocking(hostif_handle_t *handle, uint8_t count, uint32_t *numOfReveive);
static status_t SPI_Send_Blocking(hostif_handle_t *handle);
static void Tlv_Process(uint32_t dwCommandLen, uint32_t *pdwResponseLen);
static uint32_t HifEx_WriteRegister(uint32_t dwCommandLen);
static uint32_t HifEx_ReadRegister(uint32_t dwCommandLen);
static uint32_t HifEx_LoopBack(uint32_t dwCommandLen);
static uint32_t HifEx_EncDecDemo(uint32_t dwCommandLen);
static uint32_t HifEx_UnknownCmd();
static status_t SendTlvResponse(uint32_t dwBufferSize);
static void HIF_SPI_GetDefaultConfig(hostif_config_t *hifSpiConfig);
static void Do_Printf(uint8_t *buffer, uint32_t bufferLen, printf_flag_t flag);
/*!
 * @brief  This function performs an AES-CBC encryption or decryption operation
 *         on full blocks.
 *
 *         It performs the operation defined in the \p mode
 *         parameter (encrypt/decrypt), on the input data buffer defined in
 *         the \p input parameter.
 *
 *         It can be called as many times as needed, until all the input
 *         data is processed. mbedtls_aes_init(), and either
 *         mbedtls_aes_setkey_enc() or mbedtls_aes_setkey_dec() must be called
 *         before the first call to this API with the same context.
 *
 * \note   This function operates on full blocks, that is, the input size
 *         must be a multiple of the AES block size of \c 16 Bytes.
 *
 * \note   Upon exit, the content of the IV is updated so that you can
 *         call the same function again on the next
 *         block(s) of data and get the same result as if it was
 *         encrypted in one call. This allows a "streaming" usage.
 *         If you need to retain the contents of the IV, you should
 *         either save it manually or use the cipher module instead.
 *
 *
 * \param ctx      The AES context to use for encryption or decryption.
 *                 It must be initialized and bound to a key.
 * \param mode     The AES operation: #MBEDTLS_AES_ENCRYPT or
 *                 #MBEDTLS_AES_DECRYPT.
 * \param length   The length of the input data in Bytes. This must be a
 *                 multiple of the block size (\c 16 Bytes).
 * \param iv       Initialization vector (updated after use).
 *                 It must be a readable and writeable buffer of \c 16 Bytes.
 * \param input    The buffer holding the input data.
 *                 It must be readable and of size \p length Bytes.
 * \param output   The buffer holding the output data.
 *                 It must be writeable and of size \p length Bytes.
 *
 * \return         \c 0 on success.
 * \return         #MBEDTLS_ERR_AES_INVALID_INPUT_LENGTH
 *                 on failure.
 */
static int mbedtls_aes_crypt_cbc( mbedtls_aes_context *ctx,
                    int mode,
                    size_t length,
                    unsigned char iv[16],
                    const unsigned char *input,
                    unsigned char *output );
static void APP_DumpArray(char *arrayName, const uint8_t *array, uint32_t arrayLen);
/*******************************************************************************
 * Variables
 ******************************************************************************/
static volatile bool hifSpiFinished                            = false;
static hostif_handle_t g_hifSpiHandle                          = {0};
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
            hifSpiFinished = true;
            break;
        }
        default:
            PRINTF("Unknown Interrupt!\r\n");
    }
}

void hostif_rx_done_callback(hostif_handle_t *handle,
                             void *userData,
                             uint32_t transferredLength,
                             uint32_t status,
                             hostif_rx_buffer_id_t rxBufferId)
{
    gaRxBufferRX0_DataReceived += transferredLength;
    PRINTF("End Of Reception!\r\n");
    hifSpiFinished = true;
}

static void tlv_dispatcher()
{
    uint32_t dwCommandLen = 0;
    uint32_t dwResponseLen;
    status_t status;

    PRINTF("\nTLV Dispatcher Running...");

    /* Send Boot TLV Event Packet to indicate IC booted to Normal operation mode */
    status = send_boot_evt();
    if (status != kStatus_Success)
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
    status_t status;
    uint32_t dwByteIndex;
    uint32_t dwRegVal = 0;
    uint8_t abCmd[16];

    do
    {
        status = kStatus_Success;
        /* Read SYS_BOOT1_STATUS Logical register */
#if defined(PN7640_SERIES)
        status = PN76_Sys_ReadRegister(PCRM_SYS_BOOT1_STS, &dwRegVal);
        if (status != kStatus_Success)
            break;
#else
        dwRegVal = PN76_Sys_ReadRegister(PCRM_SYS_BOOT1_STS);
#endif

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

    if (status != kStatus_Success)
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

    hifSpiFinished = false;
    status         = HOSTIF_SendNonBlocking(&g_hifSpiHandle, &transfer[0]);

    if (kStatus_Success == status)
    {
        while (!hifSpiFinished)
        {
        }
    }

    return status;
}

static uint32_t ReceiveTlvCommand(void)
{
    uint32_t dwTotalBytesRxd = 0;
    uint32_t dwExpBytes;
    uint8_t *pRxBuffer    = gabDispBuffer; /* Global Dispatcher RX Buffer */
    uint32_t dwBufferSize = TLV_DISPATCHER_BUFF_SIZE;
    status_t status;

    transfer[0].startAddress    = (uint8_t *)pRxBuffer;
    transfer[0].bufferLength    = dwBufferSize;
    transfer[0].rxCallback      = hostif_rx_done_callback;
    transfer[0].rxCallbackParam = NULL;

    /* Recieve TLV Command packet over HIF */
    status = SPI_Receive_Blocking(&g_hifSpiHandle, 1, &dwTotalBytesRxd);
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

static status_t SPI_Receive_Blocking(hostif_handle_t *handle, uint8_t count, uint32_t *numOfReveive)
{
    status_t status;
    hifSpiFinished = false;
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
        while (hifSpiFinished != true)
        {
        }
        /* Save number of Received bytes in RX0 Buffer */
        *numOfReveive = gaRxBufferRX0_DataReceived;
    } while (0);

    /* Disable the RX Buffer RX0 */
    HOSTIF_AbortReceive(handle);
    return status;
}

static status_t SPI_Send_Blocking(hostif_handle_t *handle)
{
    status_t status;
    hifSpiFinished = false;
    do
    {
        status = HOSTIF_SendNonBlocking(handle, transfer);
        if (status != kStatus_Success)
        {
            PRINTF("Send_Blocking ERROR\r\n");
            break;
        }
        while (hifSpiFinished != true)
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
        case HIFEX_ENC_DEC_DEMO:
        	dwTlvResponseLen = HifEx_EncDecDemo(dwCommandLen);
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
    PN76_Status_t status;
    uint16_t wRegAddr;
    uint8_t statusCode;

    do
    {
        statusCode = HIFEX_RSP_STATUS_INSTR_ERROR;

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

        status = PN76_Sys_WriteRegister(wRegAddr, dwRegVal);
        if (status != PN76_STATUS_SUCCESS)
            break;

        /* Operation success */
        statusCode = HIFEX_RSP_ALL_OK;

    } while (0);

    /* Create TLV Response Packet in same Dispatcher Buffer */
    dwByteIndex                  = HIFEX_TLV_TYPE_INDEX;
    gabDispBuffer[dwByteIndex++] = HIFEX_TLV_WRITE_REGISTER; /* Type Byte */
    gabDispBuffer[dwByteIndex++] = 0x00;                     /* Len MSB Byte */
    gabDispBuffer[dwByteIndex++] = 0x01;                     /* Len LSB Byte - Payload contains 1 byte Status code */
    gabDispBuffer[dwByteIndex++] = statusCode;               /* Payload - 1 byte Status Code */

    return dwByteIndex;
}

static uint32_t HifEx_ReadRegister(uint32_t dwCommandLen)
{
    uint32_t dwByteIndex;
    uint32_t dwRegVal;
#if defined(PN7640_SERIES)
    PN76_Status_t status;
#endif
    uint16_t wRegAddr;
    uint8_t statusCode;

    do
    {
        statusCode = HIFEX_RSP_STATUS_INSTR_ERROR;

        if (dwCommandLen != HIFEX_TLV_READ_REGISTER_CMD_LEN)
            break;

        /* Retrieve Logical register address and Register bank from command packet payload */
        wRegAddr = gabDispBuffer[3];
        wRegAddr |= (((uint16_t)gabDispBuffer[4]) << 8);
        dwRegVal = 0;
#if defined(PN7640_SERIES)
        status = PN76_Sys_ReadRegister(wRegAddr, &dwRegVal);
        if (status != PN76_STATUS_SUCCESS)
            break;
#else
        dwRegVal = PN76_Sys_ReadRegister(wRegAddr);
#endif

        /* Operation success */
        statusCode = HIFEX_RSP_ALL_OK;

    } while (0);

    /* Create TLV Response Packet in same Dispatcher Buffer */
    dwByteIndex                  = HIFEX_TLV_TYPE_INDEX;
    gabDispBuffer[dwByteIndex++] = HIFEX_TLV_READ_REGISTER; /* Type Byte */
    gabDispBuffer[dwByteIndex++] = 0x00;                    /* Len MSB Byte */
    if (statusCode == HIFEX_RSP_ALL_OK)
    {
        gabDispBuffer[dwByteIndex++] =
            0x05; /* Len LSB Byte - Payload contains 5 bytes - Status code + 32 bit reg value */
        gabDispBuffer[dwByteIndex++] = statusCode; /* Payload - 1 byte Status Code */
        gabDispBuffer[dwByteIndex++] = (uint8_t)dwRegVal;
        gabDispBuffer[dwByteIndex++] = (uint8_t)(dwRegVal >> 8);
        gabDispBuffer[dwByteIndex++] = (uint8_t)(dwRegVal >> 16);
        gabDispBuffer[dwByteIndex++] = (uint8_t)(dwRegVal >> 24);
    }
    else
    {
        gabDispBuffer[dwByteIndex++] = 0x01;       /* Len LSB Byte - Payload contains 1 byte Status code */
        gabDispBuffer[dwByteIndex++] = statusCode; /* Payload - 1 byte Status Code */
    }

    return dwByteIndex;
}

static uint32_t HifEx_LoopBack(uint32_t dwCommandLen)
{
    return dwCommandLen; /* Return Command length as response length.
                            Dispatcher buffer already contains command packet which will be sent back as response*/
}

static uint32_t HifEx_EncDecDemo(uint32_t dwCommandLen)
{
	uint32_t dwByteIndex;
	uint8_t bMode;
	uint8_t statusCode;
	mbedtls_aes_context ctx;
	uint8_t src[32] = {0};
	uint8_t	iv[16] = {0}, output[32];
	uint16_t keySize;
	uint8_t result = 1,Keyindex;
	const uint8_t key[32] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00};
	const uint16_t s_keySize[] = {128, 256};
	bMode = gabDispBuffer[3];
	Keyindex = gabDispBuffer[4];
	keySize=s_keySize[Keyindex];
	/*Copying the Data for encryption/decryption*/
    memcpy(src,&gabDispBuffer[5], sizeof(src));
	do
	{
		statusCode = HIFEX_RSP_STATUS_INSTR_ERROR;
		if (dwCommandLen < 5)
			break;
		/*Initialising the Crypto Module*/
		phmbedcrypto_Init_stz();

		memset(&ctx, 0, sizeof( mbedtls_aes_context ) );
		memset(output, 0x00, sizeof(output));
		memset(iv, 0, sizeof(iv));

		if (bMode == MBEDTLS_AES_ENCRYPT)
		{
			result    = mbedtls_aes_setkey_stz(&ctx, key, keySize);
		}
		else
		{
			result    = mbedtls_aes_setkey_stz(&ctx, key, keySize);
		}

		if (result != 0)
		{
			PRINTF("AES SetKey Failed\r\n");
			break;
		}

		result = mbedtls_aes_crypt_cbc(&ctx, bMode, sizeof(src), iv, src, output);


		if (result != 0)
		{
			PRINTF("Failed\r\n");
			APP_DumpArray("Actual result", output, sizeof(output));
			break;
		}
		else
		{
			PRINTF("Pass\r\n");
			/* Operation success */
			statusCode = HIFEX_RSP_ALL_OK;
		}

	} while (0);

	PN76_mbedtls_aes_free(&ctx);

	/* Create TLV Response Packet in same Dispatcher Buffer */
	dwByteIndex                  = HIFEX_TLV_TYPE_INDEX;
	gabDispBuffer[dwByteIndex++] = HIFEX_ENC_DEC_DEMO; /* Type Byte */
	gabDispBuffer[dwByteIndex++] = 0x00;                    /* Len MSB Byte */
	if (statusCode == HIFEX_RSP_ALL_OK)
	{
		gabDispBuffer[dwByteIndex++] = 0x22;                  /* Len LSB Byte - Payload contains 34 bytes - Status code+ RFU + 20 byte encrypted value */
		gabDispBuffer[dwByteIndex++] = statusCode;           /* Payload - 1 byte Status Code */
		gabDispBuffer[dwByteIndex++] = 0x00;                /* RFU*/
		memcpy((uint8_t*) &gabDispBuffer[dwByteIndex],output,sizeof(output));
		dwByteIndex = dwByteIndex + sizeof(output);        /*Passing the encrypted data with payload*/
	}
	else
	{
		gabDispBuffer[dwByteIndex++] = 0x01;       /* Len LSB Byte - Payload contains 1 byte Status code */
		gabDispBuffer[dwByteIndex++] = statusCode; /* Payload - 1 byte Status Code */
	}

	return dwByteIndex;
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
    transfer[0].startAddress = (uint8_t *)gabDispBuffer;
    transfer[0].bufferLength = dwBufferSize;
    Do_Printf(transfer[0].startAddress, transfer[0].bufferLength, P_SEND);
    wStatus = SPI_Send_Blocking(&g_hifSpiHandle);
    return wStatus;
}

void HIF_SPI_GetDefaultConfig(hostif_config_t *hifSpiConfig)
{
    assert(hifSpiConfig != NULL);
    HOSTIF_GetDefaultConfig(hifSpiConfig);
    hifSpiConfig->interface                               = kHOSTIF_SelectSpi;
    hifSpiConfig->interfaceConfig.spiConfig.clockPolarity = 1U;
    hifSpiConfig->interfaceConfig.spiConfig.clockPhase    = 1U;
    hifSpiConfig->enableExtIrqCtrl                        = true;
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

/*
 * AES-CBC buffer encryption/decryption
 */
static int mbedtls_aes_crypt_cbc(mbedtls_aes_context *ctx,
                          int mode,
                          size_t length,
                          unsigned char iv[16],
                          const unsigned char *input,
                          unsigned char *output)
{
   mbedtls_aes_crypt_cbc_params_t mbedtls_aes_crypt_cbc_params;
   mbedtls_aes_crypt_cbc_params.ctx = ctx;
   mbedtls_aes_crypt_cbc_params.mode = mode;
   mbedtls_aes_crypt_cbc_params.length = length;
   mbedtls_aes_crypt_cbc_params.iv = iv;
   mbedtls_aes_crypt_cbc_params.input = input;
   mbedtls_aes_crypt_cbc_params.output = output;


   return mbedtls_aes_crypt_cbc_stz(&mbedtls_aes_crypt_cbc_params);
}

static void APP_DumpArray(char *arrayName, const uint8_t *array, uint32_t arrayLen)
{
	uint32_t i;

	PRINTF("%s:\r\n", arrayName);

	for (i = 0; i < arrayLen; i++)
	{
		PRINTF("%02x ", *array++);

		if ((i % 16) == 15)
		{
			PRINTF("\r\n");
		}
	}

	PRINTF("\r\n");
}
/*!
 * @brief Main function.
 */
int main(void)
{
    /* Init data. */
    hostif_config_t hifSpiConfig = {0};
    /* Init hardware. */
    BOARD_InitHardware();
    PRINTF("HIF SPI Slave Started!\r\n");

    /* Initialize the SPI HAL */
    /*
     * hifSpiConfig.interface = kHOSTIF_SelectSpi;
     * hifSpiConfig.timeOut = 0U;
     * hifSpiConfig.waterMarkLevel = 0U;
     * hifSpiConfig.enableCrc = false;
     * hifSpiConfig.enableExtIrqCtrl = false;
     */
    HIF_SPI_GetDefaultConfig(&hifSpiConfig);
    HOSTIF_Init(HOSTIF, &hifSpiConfig);
    AD_NFC_IRQ_Init();

    HOSTIF_CreateHandle(HOSTIF, &g_hifSpiHandle, hostif_callback, NULL);

    tlv_dispatcher();
    while (1)
    {
    }
}
