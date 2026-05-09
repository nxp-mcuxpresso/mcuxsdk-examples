/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_xspi.h"
#include "app.h"
#include "fsl_debug_console.h"
#include "fsl_xspi_edma.h"
#include "fsl_edma.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
extern status_t xspi_quad_flash_erase_sector(XSPI_Type *base, uint32_t address);
extern status_t xspi_quad_flash_page_program(XSPI_Type *base, uint32_t dstAddr, const uint32_t *src);
extern status_t xspi_quad_flash_read(XSPI_Type *base, uint32_t srcAddr, uint32_t *dst, uint32_t length);
extern void xspi_callback(XSPI_Type *base, xspi_edma_handle_t *handle, status_t status, void *userData);
extern status_t xspi_quad_full_init(XSPI_Type *base, uint8_t *vendorId);
extern void xspi_quad_reset_and_halt(XSPI_Type *base);
/*******************************************************************************
 * Variables
 ******************************************************************************/
/* Default xspi+edma driver uses 32-bit data width configuration for transfer,
   this requires data buffer address should be aligned to 32-bit. */
AT_NONCACHEABLE_SECTION_ALIGN(static uint8_t s_nor_program_buffer[256], 4);
AT_NONCACHEABLE_SECTION_ALIGN(static uint8_t s_nor_read_buffer[256], 4);
edma_handle_t dmaTxHandle;
edma_handle_t dmaRxHandle;
xspi_edma_handle_t xspiHandle;
/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Main function
 */
int main(void)
{
    uint32_t i = 0;
    status_t status;
    uint8_t vendorID = 0;
    edma_config_t userConfig;

    BOARD_InitHardware();

    PRINTF("\r\nXSPI QPI SDR (4-4-4) EDMA example started!\r\n");

    /* EDMA init */
    EDMA_GetDefaultConfig(&userConfig);
    userConfig.enableMasterIdReplication = true;
    EDMA_Init(EXAMPLE_XSPI_DMA, &userConfig);
    EDMA_EnableChannelMasterIDReplication(EXAMPLE_XSPI_DMA, XSPI_TX_DMA_CHANNEL, true);
    EDMA_CreateHandle(&dmaTxHandle, EXAMPLE_XSPI_DMA, XSPI_TX_DMA_CHANNEL);
    EDMA_CreateHandle(&dmaRxHandle, EXAMPLE_XSPI_DMA, XSPI_RX_DMA_CHANNEL);

#if defined(FSL_FEATURE_EDMA_HAS_CHANNEL_MUX) && FSL_FEATURE_EDMA_HAS_CHANNEL_MUX
    EDMA_SetChannelMux(EXAMPLE_XSPI_DMA, XSPI_TX_DMA_CHANNEL, XSPI_TX_DMA_REQUEST_SOURCE);
    EDMA_SetChannelMux(EXAMPLE_XSPI_DMA, XSPI_RX_DMA_CHANNEL, XSPI_RX_DMA_REQUEST_SOURCE);
#endif

    /* Create handle for xspi. */
    XSPI_TransferCreateHandleEDMA(EXAMPLE_XSPI, &xspiHandle, xspi_callback, NULL, &dmaTxHandle, &dmaRxHandle);

    /* Combined init runs entirely from RAM: XSPI controller init + flash reset +
     * 4-byte address mode + QPI (4-4-4) entry + JEDEC ID verification.
     * XIP is broken during this call and restored when QPI mode is established. */
    status = xspi_quad_full_init(EXAMPLE_XSPI, &vendorID);
    if (status != kStatus_Success)
    {
        /* XIP may be broken — loop without calling flash-resident code. */
        while (1)
        {
        }
    }

    PRINTF("4-byte address mode entered.\r\n");
    PRINTF("QPI (4-4-4) mode enabled.\r\n");
    PRINTF("Flash vendor ID: 0x%02X\r\n", vendorID);

    /* Erase sector */
    PRINTF("Erasing Serial NOR over XSPI...\r\n");
    status = xspi_quad_flash_erase_sector(EXAMPLE_XSPI, EXAMPLE_SECTOR * SECTOR_SIZE);
    if (status != kStatus_Success)
    {
        PRINTF("Erase sector failure !\r\n");
        return -1;
    }
    PRINTF("Erase done.\r\n");

    /* Step 1: EDMA read of the erased sector */
    memset(s_nor_program_buffer, 0xFFU, sizeof(s_nor_program_buffer));

    status = xspi_quad_flash_read(EXAMPLE_XSPI, EXAMPLE_SECTOR * SECTOR_SIZE, (uint32_t *)(void *)s_nor_read_buffer,
                                  sizeof(s_nor_read_buffer));
    if (status != kStatus_Success)
    {
        PRINTF("EDMA read after erase failure !\r\n");
        return -1;
    }

    if (memcmp(s_nor_program_buffer, s_nor_read_buffer, sizeof(s_nor_program_buffer)) != 0)
    {
        PRINTF("Erase data - read out data value incorrect !\r\n ");
        return -1;
    }
    else
    {
        PRINTF("Erase verify (EDMA read) - OK.\r\n");
    }

    /* Step 2: Program + EDMA read */
    for (i = 0; i < FLASH_PAGE_SIZE; i++)
    {
        s_nor_program_buffer[i] = (uint8_t)i;
    }

    status = xspi_quad_flash_page_program(EXAMPLE_XSPI, EXAMPLE_SECTOR * SECTOR_SIZE,
                                          (const uint32_t *)(void *)s_nor_program_buffer);
    if (status != kStatus_Success)
    {
        PRINTF("Page program failure !\r\n");
        return -1;
    }
    PRINTF("Page program done.\r\n");

    status = xspi_quad_flash_read(EXAMPLE_XSPI, EXAMPLE_SECTOR * SECTOR_SIZE, (uint32_t *)(void *)s_nor_read_buffer,
                                  sizeof(s_nor_read_buffer));
    if (status != kStatus_Success)
    {
        PRINTF("EDMA read after program failure !\r\n");
        return -1;
    }

    if (memcmp(s_nor_read_buffer, s_nor_program_buffer, sizeof(s_nor_program_buffer)) != 0)
    {
        PRINTF("Program data - read out data value incorrect !\r\n ");
        return -1;
    }
    else
    {
        PRINTF("==> EDMA program+read: PASS.\r\n");
    }

    /* Step 3: AHB read of the programmed data */
    DEMO_INVALIDATE_CACHES;
    memset(s_nor_read_buffer, 0x55U, sizeof(s_nor_read_buffer));
    memcpy(s_nor_read_buffer, (void *)(EXAMPLE_XSPI_AMBA_BASE + EXAMPLE_SECTOR * SECTOR_SIZE),
           sizeof(s_nor_read_buffer));
    if (memcmp(s_nor_program_buffer, s_nor_read_buffer, sizeof(s_nor_read_buffer)) == 0)
    {
        PRINTF("==> AHB program+read: PASS.\r\n");
    }
    else
    {
        PRINTF("==> AHB program+read: MISMATCH.\r\n");
    }

    /* Print final messages while XIP still works, then reset flash and halt.
     * xspi_quad_reset_and_halt runs from RAM and never returns — after it resets
     * the flash to SPI mode, XIP is permanently broken. */
    PRINTF("XSPI QPI SDR (4-4-4) EDMA example finished.\r\n");
    PRINTF("Flash reset to default SPI mode.\r\n");
    xspi_quad_reset_and_halt(EXAMPLE_XSPI);

    /* Never reached. */
    return 0;
}
