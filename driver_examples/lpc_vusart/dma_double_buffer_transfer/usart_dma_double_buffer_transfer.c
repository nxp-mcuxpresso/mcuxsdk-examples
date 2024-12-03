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
#include "fsl_usart_dma.h"
#include "fsl_dma.h"
#include "fsl_debug_console.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
/*!< The DMA RX Handles. */
dma_handle_t g_DMA_Handle;
uint8_t g_data_buffer[16];

/*******************************************************************************
 * Code
 ******************************************************************************/

void DMA_Callback(dma_handle_t *handle, void *param, bool transferDone, uint32_t tcds)
{
    int i;
    if (tcds == kDMA_IntB)
    {
        PRINTF("\n\r CallBack B \n\r");
        for (i = 0; i <= 7; i++)
        {
            PRINTF("%c", g_data_buffer[i]);
        }
        PRINTF(" ");
        for (i = 8; i <= 15; i++)
        {
            PRINTF("%c", g_data_buffer[i]);
        }
    }
    if (tcds == kDMA_IntA)
    {
        PRINTF("\n\r CallBack A \n\r");
        for (i = 0; i <= 7; i++)
        {
            PRINTF("%c", g_data_buffer[i]);
        }
        PRINTF(" ");
        for (i = 8; i <= 15; i++)
        {
            PRINTF("%c", g_data_buffer[i]);
        }
    }
}

/*! @brief Static table of descriptors */
#if defined(__ICCARM__)
#pragma data_alignment              = 16
dma_descriptor_t g_pingpong_desc[2] = {0};
#elif defined(__CC_ARM) || defined(__ARMCC_VERSION)
__attribute__((aligned(16))) dma_descriptor_t g_pingpong_desc[2] = {0};
#elif defined(__GNUC__)
__attribute__((aligned(16))) dma_descriptor_t g_pingpong_desc[2] = {0};
#endif

/*!
 * @brief Main function
 */
int main(void)
{
    dma_transfer_config_t transferConfig;
    usart_config_t config;

    BOARD_InitHardware();

    /* DeInit USART, because debug console may has initialized the DEMO_USART. */
    USART_Deinit(DEMO_USART);

    /* Default configuration:
     * config.baudRate_Bps = 115200U;
     * config.parityMode = kUSART_ParityDisabled;
     * config.stopBitCount = kUSART_OneStopBit;
     * config.fifoConfig.txFifoThreshold = 0;
     * config.fifoConfig.rxFifoThreshold = 0;
     * config.fifoConfig.TxFifoSize = 16U;
     * config.fifoConfig.RxFifoSize = 16U;
     */
    USART_GetDefaultConfig(&config);
    config.enableRx     = true;
    config.enableTx     = true;
    config.baudRate_Bps = BOARD_DEBUG_UART_BAUDRATE;

    USART_Init(DEMO_USART, &config, DEMO_USART_CLK_FREQ);

    PRINTF("USART Receive Double Buffer Example. The USART will echo the double buffer after each 8 bytes :\n\r");

    /* Configure DMA. */
    DMA_Init(DMA0);
    DMA_EnableChannel(DMA0, USART_RX_DMA_CHANNEL);
    DMA_CreateHandle(&g_DMA_Handle, DMA0, USART_RX_DMA_CHANNEL);
    DMA_SetCallback(&g_DMA_Handle, DMA_Callback, NULL);
    DMA_PrepareTransfer(&transferConfig, (void *)&DEMO_USART->RXDAT, g_data_buffer, 1, 8, kDMA_PeripheralToMemory,
                        &g_pingpong_desc[1]);
    DMA_SubmitTransfer(&g_DMA_Handle, &transferConfig);
    transferConfig.xfercfg.intA = false;
    transferConfig.xfercfg.intB = true;
    DMA_CreateDescriptor(&g_pingpong_desc[1], &transferConfig.xfercfg, (void *)&DEMO_USART->RXDAT, &g_data_buffer[8],
                         &g_pingpong_desc[0]);
    transferConfig.xfercfg.intA = true;
    transferConfig.xfercfg.intB = false;
    DMA_CreateDescriptor(&g_pingpong_desc[0], &transferConfig.xfercfg, (void *)&DEMO_USART->RXDAT, &g_data_buffer[0],
                         &g_pingpong_desc[1]);

    DMA_StartTransfer(&g_DMA_Handle);

    while (1)
    {
        __WFI();
    }
}
