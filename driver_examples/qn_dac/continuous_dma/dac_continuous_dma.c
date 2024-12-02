/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "app.h"
#include <stdlib.h>

#include "fsl_debug_console.h"
#include "fsl_dac.h"
#include "fsl_power.h"
#include "fsl_dma.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_DAC_ARRAY_SIZE 16U
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void DAC_ConvertDMACallback(dma_handle_t *handle, void *param, bool transferDone, uint32_t intmode);
/*******************************************************************************
 * Variables
 ******************************************************************************/
dma_handle_t g_adcDmaHandle;
volatile bool g_flag;
uint32_t g_dacDataArray[DEMO_DAC_ARRAY_SIZE] = {0U,   50U,  98U,  142U, 180U, 212U, 235U, 250U,
                                                255U, 250U, 235U, 212U, 180U, 142U, 98U,  50U};
/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Main function
 */
int main(void)
{
    dac_config_t config;
    dma_transfer_config_t transferConfig;

    /* Power on DAC. */
    POWER_DisablePD(kPDRUNCFG_PD_DAC);

    BOARD_InitHardware();

    PRINTF("\r\nDAC continuous DMA example.\r\n");

    /* Configure DMA. */
    DMA_Init(DMA0);
    DMA_EnableChannel(DMA0, DEMO_DAC_DMA_CHANNEL);
    DMA_CreateHandle(&g_adcDmaHandle, DMA0, DEMO_DAC_DMA_CHANNEL);
    DMA_SetCallback(&g_adcDmaHandle, DAC_ConvertDMACallback, NULL);
    DMA_PrepareTransfer(&transferConfig, (void *)g_dacDataArray, (void *)&DEMO_DAC_BASE->DIN, sizeof(uint32_t),
                        4 * DEMO_DAC_ARRAY_SIZE, kDMA_MemoryToPeripheral, NULL);
    DMA_SubmitTransfer(&g_adcDmaHandle, &transferConfig);

    /* Configure DAC. */
    DAC_GetDefaultConfig(&config);
    config.clk_div      = 159U;
    config.trg_cfg.src  = kDAC_TriggerSelectSoftware;
    config.trg_cfg.mode = kDAC_TriggerModeContinueMode;
    config.trg_cfg.edge = kDAC_TriggerEdgeSelectBothEdge;
    DAC_Init(DEMO_DAC_BASE, &config);
    DAC_Enable(DEMO_DAC_BASE, true);

    DMA_StartTransfer(&g_adcDmaHandle);

    PRINTF("Please probe the DAC output with a oscilloscope.\r\n");
    while (1)
    {
    }
}

static void DAC_ConvertDMACallback(dma_handle_t *handle, void *param, bool transferDone, uint32_t intmode)
{
#if 0
    DMA_PrepareTransfer(&transferConfig, (void *)g_dacDataArray, (void *)&DEMO_DAC_BASE->DIN, sizeof(uint32_t), 4*DEMO_DAC_ARRAY_SIZE,
                        kDMA_MemoryToPeripheral, NULL);
    DMA_SubmitTransfer(&g_adcDmaHandle, &transferConfig);
#endif
    DMA_StartTransfer(&g_adcDmaHandle);
}
