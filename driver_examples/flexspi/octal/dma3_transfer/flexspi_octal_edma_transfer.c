/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_flexspi.h"
#include "app.h"
#include "fsl_debug_console.h"
#include "fsl_flexspi_edma.h"
#include "fsl_edma.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
extern status_t flexspi_nor_flash_erase_sector(FLEXSPI_Type *base, uint32_t address);
extern status_t flexspi_nor_flash_page_program(FLEXSPI_Type *base, uint32_t dstAddr, const uint32_t *src);
extern status_t flexspi_nor_read_data(FLEXSPI_Type *base, uint32_t startAddress, uint32_t *buffer, uint32_t length);
extern status_t flexspi_nor_get_vendor_id(FLEXSPI_Type *base, uint8_t *vendorId);
extern status_t flexspi_nor_enable_octal_mode(FLEXSPI_Type *base);
extern void flexspi_nor_flash_init(FLEXSPI_Type *base);
/*******************************************************************************
 * Variables
 ******************************************************************************/
/*Default flexspi+dma driver uses 32-bit data width configuration for transfer,
this requires data buffer address should be aligned to 32-bit. */
AT_NONCACHEABLE_SECTION_ALIGN(static uint8_t s_nor_program_buffer[256], 4);
static uint8_t s_nor_read_buffer[256];

edma_handle_t dmaTxHandle;
edma_handle_t dmaRxHandle;
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

    PRINTF("\r\nFLEXSPI edma example started!\r\n");

    /* EDMA init */
    /*
     * userConfig.enableRoundRobinArbitration = false;
     * userConfig.enableHaltOnError = true;
     * userConfig.enableContinuousLinkMode = false;
     * userConfig.enableDebugMode = false;
     */
    EDMA_GetDefaultConfig(&userConfig);
    EDMA_Init(EXAMPLE_FLEXSPI_DMA, &userConfig);

    /* Create the EDMA channel handles */
    EDMA_CreateHandle(&dmaTxHandle, EXAMPLE_FLEXSPI_DMA, FLEXSPI_TX_DMA_CHANNEL);
    EDMA_CreateHandle(&dmaRxHandle, EXAMPLE_FLEXSPI_DMA, FLEXSPI_RX_DMA_CHANNEL);
#if defined(FSL_FEATURE_EDMA_HAS_CHANNEL_MUX) && FSL_FEATURE_EDMA_HAS_CHANNEL_MUX
    EDMA_SetChannelMux(EXAMPLE_FLEXSPI_DMA, FLEXSPI_TX_DMA_CHANNEL, FLEXSPI_TX_DMA_REQUEST_SOURCE);
    EDMA_SetChannelMux(EXAMPLE_FLEXSPI_DMA, FLEXSPI_RX_DMA_CHANNEL, FLEXSPI_RX_DMA_REQUEST_SOURCE);
#endif

    /* FLEXSPI init */
    flexspi_nor_flash_init(EXAMPLE_FLEXSPI);

    /* Adesto's octal flash has an limitation for read ID, which is that the Read Manufacturer and Device ID command is
     * limited to a maximum clock frequency of fCLK. */
#if defined(FLASH_ADESTO) && FLASH_ADESTO
    /* Get vendor ID. */
    status = flexspi_nor_get_vendor_id(EXAMPLE_FLEXSPI, &vendorID);
    if (status != kStatus_Success)
    {
        return status;
    }
    PRINTF("Vendor ID: 0x%x\r\n", vendorID);

    /* Enter quad mode. */
    status = flexspi_nor_enable_octal_mode(EXAMPLE_FLEXSPI);
    if (status != kStatus_Success)
    {
        return status;
    }
#else
    /* Enter quad mode. */
    status = flexspi_nor_enable_octal_mode(EXAMPLE_FLEXSPI);
    if (status != kStatus_Success)
    {
        return status;
    }

    /* Get vendor ID. */
    status = flexspi_nor_get_vendor_id(EXAMPLE_FLEXSPI, &vendorID);
    if (status != kStatus_Success)
    {
        return status;
    }
    PRINTF("Vendor ID: 0x%x\r\n", vendorID);
#endif

    /* Erase sectors. */
    PRINTF("Erasing Serial NOR over FlexSPI...\r\n");

    /* Disable I cache to avoid cache pre-fatch instruction with branch prediction from flash
       and application operate flash synchronously in multi-tasks. */
#if defined(__ICACHE_PRESENT) && (__ICACHE_PRESENT == 1U)
    volatile bool ICacheEnableFlag = false;
    /* Disable I cache. */
    if (SCB_CCR_IC_Msk == (SCB_CCR_IC_Msk & SCB->CCR))
    {
        SCB_DisableICache();
        ICacheEnableFlag = true;
    }
#endif /* __ICACHE_PRESENT */

    status = flexspi_nor_flash_erase_sector(EXAMPLE_FLEXSPI, EXAMPLE_SECTOR * SECTOR_SIZE);

#if defined(__ICACHE_PRESENT) && (__ICACHE_PRESENT == 1U)
    if (ICacheEnableFlag)
    {
        /* Enable I cache. */
        SCB_EnableICache();
        ICacheEnableFlag = false;
    }
#endif /* __ICACHE_PRESENT */

    if (status != kStatus_Success)
    {
        PRINTF("Erase sector failure !\r\n");
        return -1;
    }

#if defined(CACHE_MAINTAIN) && CACHE_MAINTAIN
    DCACHE_InvalidateByRange(EXAMPLE_FLEXSPI_AMBA_BASE + EXAMPLE_SECTOR * SECTOR_SIZE, FLASH_PAGE_SIZE);
#endif

    memset(s_nor_program_buffer, 0xFFU, sizeof(s_nor_program_buffer));
    memcpy(s_nor_read_buffer, (void *)(EXAMPLE_FLEXSPI_AMBA_BASE + EXAMPLE_SECTOR * SECTOR_SIZE),
           sizeof(s_nor_read_buffer));

    if (memcmp(s_nor_program_buffer, s_nor_read_buffer, sizeof(s_nor_program_buffer)))
    {
        PRINTF("Erase data -  read out data value incorrect !\r\n ");
        return -1;
    }
    else
    {
        PRINTF("Erase data - successfully. \r\n");
    }

    for (i = 0; i < FLASH_PAGE_SIZE; i++)
    {
        s_nor_program_buffer[i] = i;
    }

#if defined(__ICACHE_PRESENT) && (__ICACHE_PRESENT == 1U)
    /* Disable I cache. */
    if (SCB_CCR_IC_Msk == (SCB_CCR_IC_Msk & SCB->CCR))
    {
        SCB_DisableICache();
        ICacheEnableFlag = true;
    }
#endif /* __ICACHE_PRESENT */

    status =
        flexspi_nor_flash_page_program(EXAMPLE_FLEXSPI, EXAMPLE_SECTOR * SECTOR_SIZE, (void *)s_nor_program_buffer);

#if defined(__ICACHE_PRESENT) && (__ICACHE_PRESENT == 1U)
    if (ICacheEnableFlag)
    {
        /* Enable I cache. */
        SCB_EnableICache();
        ICacheEnableFlag = false;
    }
#endif /* __ICACHE_PRESENT */

    if (status != kStatus_Success)
    {
        PRINTF("Page program failure !\r\n");
        return -1;
    }

#if defined(CACHE_MAINTAIN) && CACHE_MAINTAIN
    DCACHE_InvalidateByRange(EXAMPLE_FLEXSPI_AMBA_BASE + EXAMPLE_SECTOR * SECTOR_SIZE, FLASH_PAGE_SIZE);
#endif

    memcpy(s_nor_read_buffer, (void *)(EXAMPLE_FLEXSPI_AMBA_BASE + EXAMPLE_SECTOR * SECTOR_SIZE),
           sizeof(s_nor_read_buffer));

    if (memcmp(s_nor_read_buffer, s_nor_program_buffer, sizeof(s_nor_program_buffer)) != 0)
    {
        PRINTF("Program data -  read out data value incorrect !\r\n ");
        return -1;
    }
    else
    {
        PRINTF("Program data - successfully. \r\n");
    }

    while (1)
    {
    }
}
