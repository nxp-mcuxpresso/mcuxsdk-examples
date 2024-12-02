/*
 * Copyright  2017 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"
#include "app.h"
#include "fsl_usart.h"
#include "fsl_debug_console.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define BUFFER_SIZE         16
#define DEMO_USART_BAUDRATE 9600
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void EXAMPLE_USARTMasterInit(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
usart_handle_t g_usartSlaveHandle;
usart_transfer_t slaveXfer;

uint8_t g_txBuffer[BUFFER_SIZE] = {0};
uint8_t g_rxBuffer[BUFFER_SIZE] = {0};

volatile bool isSlaveCompleted = false;
/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Main function
 */
int main(void)
{
    uint32_t i = 0;
    BOARD_InitHardware();

    /* Print information to the terminal. */
    PRINTF("USART synchronous transfer example.\r\n");
    PRINTF("Master USART will send data to slave on another board.\r\n");

    /* Initialize the master USART. */
    EXAMPLE_USARTMasterInit();
    for (; i < BUFFER_SIZE; i++)
        g_txBuffer[i] = i % 256;
    /* USART master send data in polling mode. */
    USART_WriteBlocking(USART_MASTER_BASEADDR, g_txBuffer, BUFFER_SIZE);

    /* Deinit the USART instances, */
    USART_Deinit(USART_MASTER_BASEADDR);

    while (1)
    {
    }
}

static void EXAMPLE_USARTMasterInit(void)
{
    usart_config_t masterConfig;

    /* Default config by using USART_GetDefaultConfig():
     * config->baudRate_Bps = 9600U;
     * config->parityMode = kUSART_ParityDisabled;
     * config->stopBitCount = kUSART_OneStopBit;
     * config->bitCountPerChar = kUSART_8BitsPerChar;
     * config->loopback = false;
     * config->enableRx = false;
     * config->enableTx = false;
     * config->syncMode = kUSART_SyncModeDisabled;
     */
    USART_GetDefaultConfig(&masterConfig);
    masterConfig.baudRate_Bps = DEMO_USART_BAUDRATE;
    masterConfig.enableTx     = true;
    masterConfig.syncMode     = kUSART_SyncModeMaster;

    /* Initialize the master USART. */
    USART_Init(USART_MASTER_BASEADDR, &masterConfig, USART_MASTER_CLK_FREQ);
}
