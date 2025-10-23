/*
 * Copyright 2022-2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "app.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_uart.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
uint8_t g_txBuffer0[TRANSFER_SIZE] = {0};
uint8_t g_txBuffer1[TRANSFER_SIZE] = {0};
uint8_t g_rxBuffer[TRANSFER_SIZE + 1U] = {0};

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
int main(void)
{
    status_t status;
    uart_config_t config;
    uint32_t i;

    BOARD_InitHardware();

    /*
     * config.baudRate_Bps = 115200U;
     * config.parityMode = kUART_ParityDisabled;
     * config.dataBitsCount = kUART_EightDataBits;
     * config.stopBitCount = kUART_OneStopBit;
     * config.txFifoWatermark = 0;
     * config.rxFifoWatermark = 1;
     * config.enableTx = false;
     * config.enableRx = false;
     */
    UART_GetDefaultConfig(&config);
    config.baudRate_Bps    = BOARD_DEBUG_UART_BAUDRATE;
    config.txFifoWatermark = 2;
    config.rxFifoWatermark = 8;
    config.enableTx        = true;
    config.enableRx        = true;

    status = UART_Init(DEMO_UART, &config, DEMO_UART_CLK_FREQ);
    if (kStatus_Success != status)
    {
        return kStatus_Fail;
    }

    /* Configure 9bit slave address. */
    UART_SetMatchAddress(DEMO_UART, EXAMPLE_ADDRESS);
    /* Select Automatic Address Detect mode. */
    UART_Set9bitAddressDetectMode(DEMO_UART, kUART_9BitAutoMode);
    /* Enable 9bit Slave Address Detected Interrupt. */
    UART_Set9bitAddressDetectInterrput(DEMO_UART, true);
    /* Set up transfer data */
    UART_Enable9bitMode(DEMO_UART, true);

    for (i = 0U; i < TRANSFER_SIZE; i++)
    {
        g_txBuffer0[i] = 0x10 + i;
    }

    for (i = 0U; i < TRANSFER_SIZE; i++)
    {
        g_txBuffer1[i] = 0x80 + i;
    }

    for (i = 0U; i < TRANSFER_SIZE + 1U; i++)
    {
        g_rxBuffer[i] = 0;
    }

    /* First send TRANSFER_SIZE byte of data without addressing itself first, these data should be discarded. */
    PRINTF("  UART will send first piece of data out without addressing itself:\n\r");
    PRINTF("    ");
    for (i = 0U; i < TRANSFER_SIZE; i++)
    {
        PRINTF("0x%2x  ", g_txBuffer0[i]);
    }
    PRINTF("\r\n\r\n");
    UART_WriteBlocking(DEMO_UART, g_txBuffer0, TRANSFER_SIZE);

    /* Second send TRANSFER_SIZE byte of data with addressing itself first, these data should be received. */
    PRINTF("  UART will send second piece of data out with addressing itself:\n\r");

    PRINTF("    Address: 0x%2x : ", EXAMPLE_ADDRESS);

    for (i = 0; i < TRANSFER_SIZE; i++)
    {
        PRINTF("0x%2x ", g_txBuffer1[i]);
    }
    PRINTF("\r\n\r\n");
    UART_SendAddress(DEMO_UART, EXAMPLE_ADDRESS);
    UART_WriteBlocking(DEMO_UART, g_txBuffer1, TRANSFER_SIZE);

    bool success = true;
    UART_ReadBlocking(DEMO_UART, g_rxBuffer, TRANSFER_SIZE + 1);
    PRINTF("  UART received data:\n\r");
    PRINTF("  Address: 0x%2x : ", g_rxBuffer[0]);
    for (i = 0; i < TRANSFER_SIZE; i++)
    {
        PRINTF("0x%2x ", g_rxBuffer[i + 1U]);
    }
    PRINTF("\r\n\r\n");

    /* Check if the data is right. */
    for (i = 0; i < TRANSFER_SIZE; i++)
    {
        if (g_rxBuffer[i + 1U] != g_txBuffer1[i])
        {
            success = false;
            PRINTF("  Received data does not match!\n\r");
            break;
        }
    }
    if (success)
    {
        PRINTF("  All data matches!\n\r");
    }

    while (1)
    {
    }
}
