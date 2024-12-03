/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "app.h"
#include <stdlib.h>

#include "fsl_debug_console.h"
#include "fsl_flash.h"
#include "fsl_common.h"
#include "fsl_dma.h"
#include "fsl_flash_dma.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
flash_dma_handle_t g_flashDmaHandle;
dma_handle_t g_flashWriteDMAHandle;
dma_handle_t g_flashreadDMAHandle;
volatile bool g_Transfer_Done        = false;
uint32_t data_buf[TEST_MEM_SIZE / 4] = {0};
volatile bool pass                   = false;
/*******************************************************************************
 * Code
 ******************************************************************************/

/* User callback function for DMA transfer. */
void FLASH_UserCallback(flash_dma_handle_t *handle, status_t status, void *userData)
{
    if (kStatus_FLASH_WriteDmaIdle == status)
    {
        g_Transfer_Done = true;
        /*User must to check write result*/
        if (!memcmp((void *)TEST_START_ADDR, (uint8_t *)&data_buf[0], TEST_MEM_SIZE))
        {
            pass = true;
        }
        else
        {
            pass = false;
            /*Try write again*/
        }
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    flash_config_t config;

    status_t result = 0;
    uint32_t i      = 0;

    /* Board pin, clock, debug console init */
    BOARD_InitHardware();
    /* Configure Flash  */
    FLASH_GetDefaultConfig(&config);

    /*Flash module init*/
    FLASH_Init(&config);
    PRINTF("FLASH_Init\r\n");
    /* Configure DMA. */
    DMA_Init(DMA0);
    DMA_EnableChannel(DMA0, FLASH_WRITE_DMA_CHANNEL);
    DMA_EnableChannel(DMA0, FLASH_READ_DMA_CHANNEL);

    DMA_CreateHandle(&g_flashWriteDMAHandle, DMA0, FLASH_WRITE_DMA_CHANNEL);
    DMA_CreateHandle(&g_flashreadDMAHandle, DMA0, FLASH_READ_DMA_CHANNEL);

    FLASH_CreateHandleDMA(&g_flashDmaHandle, FLASH_UserCallback, NULL, &g_flashWriteDMAHandle, &g_flashreadDMAHandle);

    /* erase test page*/
    result = FLASH_PageErase(&config, FLASH_ADDR_TO_PAGE(TEST_START_ADDR));
    if (result)
    {
        PRINTF("FLASH_PageErase error  result = %d \r\n", result);
        while (1)
        {
        }
    }

    /*Set test data */
    for (i = 0; i < TEST_MEM_SIZE / 4; i++)
    {
        data_buf[i] = (uint8_t)i;
    }
    PRINTF("FLASH_StartWriteDMA addr = 0x%x size = %d \r\n", TEST_START_ADDR, TEST_MEM_SIZE);
    FLASH_StartWriteDMA(&g_flashDmaHandle, &config, TEST_START_ADDR, (uint32_t *)data_buf, TEST_MEM_SIZE / 4);

    while (g_Transfer_Done != true)
    {
    }
    g_Transfer_Done = false;

    /*Show result*/
    if (pass)
    {
        PRINTF("Passed!\r\n");
    }
    else
    {
        PRINTF("Failed!\r\n");
    }

    while (1)
    {
    }
}
