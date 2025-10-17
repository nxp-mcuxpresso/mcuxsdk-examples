/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "app.h"
#include "board.h"
#include "fsl_common.h"
#include "fsl_debug_console.h"
#include "fsl_siul2.h"
#include "pin_mux.h"
#include "fsl_edma.h"
#include "fsl_dmamux.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
#define BUFFER_LENGTH 4

AT_NONCACHEABLE_SECTION_INIT(uint32_t srcAddr[BUFFER_LENGTH])  = {0x01, 0x02, 0x03, 0x04};
AT_NONCACHEABLE_SECTION_INIT(uint32_t destAddr[BUFFER_LENGTH]) = {0x00, 0x00, 0x00, 0x00};
volatile bool g_Transfer_Done                                  = false;

edma_handle_t g_DMA_Handle;
/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Interrupt service fuction of switch.
 *
 * This function toggles the LED
 */
/* User callback function for EDMA transfer. */
void DMA_Callback(edma_handle_t *handle, void *userData, bool transferDone, uint32_t tcds)
{
    if (transferDone)
    {
        g_Transfer_Done = true;
    }

    /* Clear EIRQ flag. */
    SIUL2_ClearExtDmaInterruptStatusFlags(BOARD_SIUL2_BASE, 1U << BOARD_SW_EIRQ); 
}

/*!
 * @brief Main function
 */
int main(void)
{
    edma_transfer_config_t transferConfig;
    edma_config_t userConfig;

    BOARD_InitHardware();

    PRINTF("MCUX SDK version: %s\r\n", MCUXSDK_VERSION_FULL_STR);

    /* Print a note to terminal. */
    PRINTF("\r\nSIUL2 input trigger EDMA example\r\n");
    PRINTF("\r\nPress %s to trigger a EDMA transfer\r\n", BOARD_SW_NAME);

    /* Init input switch EIRQ. */
    SIUL2_SetGlitchFilterPrescaler(BOARD_SIUL2_BASE, 1U);
    SIUL2_EnableExtDma(BOARD_SIUL2_BASE, BOARD_SW_EIRQ, kSIUL2_InterruptFallingEdge, 2U);

    DMAMUX_Init(EXAMPLE_DMAMUX_BASEADDR);
    DMAMUX_SetSource(EXAMPLE_DMAMUX_BASEADDR, EXAMPLE_DMAMUX_CHANNEL, EXAMPLE_DMA_REQUEST);
    DMAMUX_EnableChannel(EXAMPLE_DMAMUX_BASEADDR, EXAMPLE_DMAMUX_CHANNEL);

    /* Configure EDMA channel for one shot transfer */
    EDMA_GetDefaultConfig(&userConfig);
    EDMA_Init(EXAMPLE_DMA_BASEADDR, &userConfig);
    EDMA_CreateHandle(&g_DMA_Handle, EXAMPLE_DMA_BASEADDR, DEMO_DMA_CHANNEL);
    EDMA_SetCallback(&g_DMA_Handle, DMA_Callback, NULL);
    EDMA_PrepareTransfer(&transferConfig, srcAddr, sizeof(srcAddr[0]), destAddr, sizeof(destAddr[0]),
                         sizeof(uint32_t) * BUFFER_LENGTH, sizeof(destAddr), kEDMA_MemoryToMemory);

    do
    {
        EDMA_SubmitTransfer(&g_DMA_Handle, &transferConfig);
        EDMA_StartTransfer(&g_DMA_Handle);
        
        /* Wait for EDMA transfer finish */
         while (g_Transfer_Done != true)
         {
         }
         g_Transfer_Done = false;

          /* Print destination buffer */
          PRINTF("\r\nEDMA memory to memory transfer finish.\r\n");
    }
    while(1);
}
