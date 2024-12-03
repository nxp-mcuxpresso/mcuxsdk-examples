/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "fsl_sdioslv.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* define data buffer size */
#define RING_DATA_BUFFER_SIZE  (8192U)
#define WRITE_DATA_BUFFER_SIZE (512)
#define SDIOSLV_DES_NUM        (2U)
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
SDIOSLV_ALLOCATE_ADMA_DESCRIPTOR(s_sdioslvAdma2DesBuf, 2U);
SDK_ALIGN(uint8_t g_pingpongBuffer[RING_DATA_BUFFER_SIZE * 2U], SDIOSLV_ADMA_ADDRESS_ALIGN);
SDK_ALIGN(uint8_t g_writeBuffer[WRITE_DATA_BUFFER_SIZE], SDIOSLV_ADMA_ADDRESS_ALIGN);
/*! @brief Data read from the card */
sdioslv_handle_t sdioslvHandle;
static bool s_sdioslvTransferFinshed = false;
static bool s_sdioslvFunc1Ready      = false;
/*******************************************************************************
 * Code
 ******************************************************************************/
void sdioslvCallback(
    SDIOSLV_Type *base, sdioslv_func_t func, status_t status, void *dataAddr, uint32_t dataSize, void *userData)
{
    if ((status == kStatus_SDIOSLV_FuncWriteComplete) || (status == kStatus_SDIOSLV_FuncReadComplete))
    {
        s_sdioslvTransferFinshed = true;
    }

    if (kStatus_SDIOSLV_FuncEnabled == status)
    {
        s_sdioslvFunc1Ready = true;
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    sdioslv_config_t sdioslvConfig;
    sdioslv_transfer_t sdioslvTransfer;
    uint32_t i = 0U;

    BOARD_InitHardware();

    PRINTF("SDIOSLV enumerate sdio card example.\r\n");

    /* sdio 2.0 slave initialization */
    SDIOSLV_GetSDIOV2Config(&sdioslvConfig);
    SDIOSLV_Init(SDIOSLV, &sdioslvConfig);
    SDIOSLV_TransferCreateHandle(SDIOSLV, &sdioslvHandle, sdioslvCallback, NULL);
    SDIOSLV_InstallADMADescriptorMemory(&sdioslvHandle, s_sdioslvAdma2DesBuf, SDIOSLV_DES_NUM);
    SDIOSLV_InstallPingpongBuffer(&sdioslvHandle, g_pingpongBuffer, RING_DATA_BUFFER_SIZE);

    while (!s_sdioslvFunc1Ready)
    {
    }

    /* memset write buffer */
    for (i = 0U; i < WRITE_DATA_BUFFER_SIZE; i++)
    {
        g_writeBuffer[i] = i;
    }

    sdioslvTransfer.dataAddr = g_writeBuffer;
    sdioslvTransfer.dataSize = WRITE_DATA_BUFFER_SIZE;
    SDIOSLV_TransferSendNonBlocking(SDIOSLV, &sdioslvHandle, &sdioslvTransfer);

    while (!s_sdioslvTransferFinshed)
    {
    }

    s_sdioslvTransferFinshed = false;

    /* wait read back finished */
    while (!s_sdioslvTransferFinshed)
    {
    }

    s_sdioslvTransferFinshed = false;

    if (memcmp(g_writeBuffer, g_pingpongBuffer, WRITE_DATA_BUFFER_SIZE))
    {
        PRINTF("Read data isn't consistent with the written data.");
        return -1;
    }

    PRINTF("Read data is consistent with the written data.\r\n");
    PRINTF("SDIOSLV enumerate sdio card example finished.\r\n");

    while (true)
    {
    }
}
