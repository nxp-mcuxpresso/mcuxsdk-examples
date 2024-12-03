/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_xspi.h"
#include "app.h"
#if (defined CACHE_MAINTAIN) && (CACHE_MAINTAIN == 1)
#include "fsl_cache.h"
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 *****************************************************************************/
extern xspi_device_config_t deviceconfig;
extern const uint32_t customLUT[CUSTOM_LUT_LENGTH];

/*******************************************************************************
 * Code
 ******************************************************************************/


status_t xspi_nor_write_enable(XSPI_Type *base, uint32_t baseAddr)
{
    xspi_transfer_t flashXfer;
    status_t status;

    /* Write enable */
    flashXfer.deviceAddress = EXAMPLE_XSPI_AMBA_BASE + baseAddr;
    flashXfer.cmdType       = kXSPI_Command;
    flashXfer.seqIndex      = NOR_CMD_LUT_SEQ_IDX_WRITE_ENABLE;
    flashXfer.targetGroup   = kXSPI_TargetGroup0;

    status = XSPI_TransferBlocking(base, &flashXfer);

    return status;
}

status_t xspi_nor_wait_bus_busy(XSPI_Type *base)
{
    /* Wait status ready. */
    bool isBusy;
    uint32_t readValue;
    status_t status;
    xspi_transfer_t flashXfer;

    flashXfer.deviceAddress = EXAMPLE_XSPI_AMBA_BASE;
    flashXfer.cmdType       = kXSPI_Read;
    flashXfer.seqIndex      = NOR_CMD_LUT_SEQ_IDX_READ_STATUS_REG;
    flashXfer.targetGroup   = kXSPI_TargetGroup0;
    flashXfer.data          = &readValue;
    flashXfer.dataSize      = 1;

    do
    {
        status = XSPI_TransferBlocking(base, &flashXfer);

        if (status != kStatus_Success)
        {
            return status;
        }
        if (FLASH_BUSY_STATUS_POL)
        {
            if (readValue & (1U << FLASH_BUSY_STATUS_OFFSET))
            {
                isBusy = true;
            }
            else
            {
                isBusy = false;
            }
        }
        else
        {
            if (readValue & (1U << FLASH_BUSY_STATUS_OFFSET))
            {
                isBusy = false;
            }
            else
            {
                isBusy = true;
            }
        }

    } while (isBusy);

    return status;
}


status_t xspi_nor_flash_erase_sector(XSPI_Type *base, uint32_t address)
{
    status_t status;
    xspi_transfer_t flashXfer;

    /* Write enable */
    flashXfer.deviceAddress = EXAMPLE_XSPI_AMBA_BASE + address;
    flashXfer.cmdType       = kXSPI_Command;
    flashXfer.seqIndex      = NOR_CMD_LUT_SEQ_IDX_WRITE_ENABLE;
    flashXfer.targetGroup   = kXSPI_TargetGroup0;

    status = XSPI_TransferBlocking(base, &flashXfer);

    if (status != kStatus_Success)
    {
        return status;
    }

    flashXfer.deviceAddress = EXAMPLE_XSPI_AMBA_BASE + address;
    flashXfer.cmdType       = kXSPI_Command;
    flashXfer.seqIndex      = NOR_CMD_LUT_SEQ_IDX_ERASE_32KB;
    flashXfer.targetGroup   = kXSPI_TargetGroup0;
    status                  = XSPI_TransferBlocking(base, &flashXfer);

    if (status != kStatus_Success)
    {
        return status;
    }

    status = xspi_nor_wait_bus_busy(base);


    return status;
}

status_t xspi_nor_flash_read(XSPI_Type *base, uint32_t dstAddr, const uint32_t *src, uint32_t length)
{
    status_t status;
    xspi_transfer_t flashXfer;

    /* Write enable */
    status = xspi_nor_write_enable(base, dstAddr);

    if (status != kStatus_Success)
    {
        return status;
    }

    /* Prepare page program command */
    flashXfer.deviceAddress = EXAMPLE_XSPI_AMBA_BASE + dstAddr;
    flashXfer.cmdType       = kXSPI_Read;
    flashXfer.seqIndex      = NOR_CMD_LUT_SEQ_IDX_DDR_READ_FAST_QUAD;
    flashXfer.targetGroup   = kXSPI_TargetGroup0;
    flashXfer.data          = (uint32_t *)src;
    flashXfer.dataSize      = length;
    status                  = XSPI_TransferBlocking(base, &flashXfer);

    if (status != kStatus_Success)
    {
        return status;
    }

    status = xspi_nor_wait_bus_busy(base);

    return status;
}

status_t xspi_nor_flash_program(XSPI_Type *base, uint32_t dstAddr, const uint32_t *src, uint32_t length)
{
    status_t status;
    xspi_transfer_t flashXfer;

    /* Write enable */
    status = xspi_nor_write_enable(base, dstAddr);

    if (status != kStatus_Success)
    {
        return status;
    }

    /* Prepare page program command */
    flashXfer.deviceAddress = EXAMPLE_XSPI_AMBA_BASE + dstAddr;
    flashXfer.cmdType       = kXSPI_Write;
    flashXfer.seqIndex      = NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_OCTAL;
    flashXfer.targetGroup   = kXSPI_TargetGroup0;
    flashXfer.data          = (uint32_t *)src;
    flashXfer.dataSize      = length;
    status                  = XSPI_TransferBlocking(base, &flashXfer);

    if (status != kStatus_Success)
    {
        return status;
    }

    status = xspi_nor_wait_bus_busy(base);

    return status;
}

status_t xspi_nor_flash_page_program(XSPI_Type *base, uint32_t dstAddr, const uint32_t *src)
{
    status_t status;
    xspi_transfer_t flashXfer;

    /* To make sure external flash be in idle status, added wait for busy before program data for
        an external flash without RWW(read while write) attribute.*/
    status = xspi_nor_wait_bus_busy(base);

    if (kStatus_Success != status)
    {
        return status;
    }

    /* Write enable. */
    status = xspi_nor_write_enable(base, dstAddr);

    if (status != kStatus_Success)
    {
        return status;
    }

    /* Prepare page program command */
    flashXfer.deviceAddress = EXAMPLE_XSPI_AMBA_BASE + dstAddr;
    flashXfer.cmdType       = kXSPI_Write;
    flashXfer.seqIndex      = NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_OCTAL;
    flashXfer.targetGroup   = kXSPI_TargetGroup0;
    flashXfer.data          = (uint32_t *)src;
    flashXfer.dataSize      = FLASH_PAGE_SIZE;
    status                  = XSPI_TransferBlocking(base, &flashXfer);

    if (status != kStatus_Success)
    {
        return status;
    }

    status = xspi_nor_wait_bus_busy(base);

    return status;
}

status_t xspi_nor_get_vendor_id(XSPI_Type *base, uint8_t *vendorId)
{
    uint32_t temp;
    xspi_transfer_t flashXfer;
    flashXfer.deviceAddress = EXAMPLE_XSPI_AMBA_BASE;
    flashXfer.cmdType       = kXSPI_Read;
    flashXfer.seqIndex      = NOR_CMD_LUT_SEQ_IDX_READ_ID;
    flashXfer.targetGroup   = kXSPI_TargetGroup0;
    flashXfer.data          = &temp;
    flashXfer.dataSize      = 1;

    status_t status = XSPI_TransferBlocking(base, &flashXfer);

    *vendorId = temp;

    return status;
}

status_t xspi_nor_get_sfdp(XSPI_Type *base, uint32_t *sfdp)
{
    uint32_t temp;
    xspi_transfer_t flashXfer;
    flashXfer.deviceAddress = EXAMPLE_XSPI_AMBA_BASE;
    flashXfer.cmdType       = kXSPI_Read;
    flashXfer.seqIndex      = NOR_CMD_LUT_SEQ_IDX_SFDP;
    flashXfer.targetGroup   = kXSPI_TargetGroup0;
    flashXfer.data          = &temp;
    flashXfer.dataSize      = 1;

    status_t status = XSPI_TransferBlocking(base, &flashXfer);

    *sfdp = temp;

    return status;
}

status_t xspi_nor_erase_chip(XSPI_Type *base)
{
    status_t status;
    xspi_transfer_t flashXfer;

    /* Write enable */
    status = xspi_nor_write_enable(base, 0);

    if (status != kStatus_Success)
    {
        return status;
    }

    flashXfer.deviceAddress = EXAMPLE_XSPI_AMBA_BASE;
    flashXfer.cmdType       = kXSPI_Command;
    flashXfer.seqIndex      = NOR_CMD_LUT_SEQ_IDX_ERASE_32KB;
    flashXfer.targetGroup   = kXSPI_TargetGroup0;

    status = XSPI_TransferBlocking(base, &flashXfer);

    if (status != kStatus_Success)
    {
        return status;
    }

    status = xspi_nor_wait_bus_busy(base);

    return status;
}

void xspi_nor_flash_init(XSPI_Type *base)
{
    xspi_config_t config;
    /* To store custom's LUT table in local. */
    uint32_t tempLUT[CUSTOM_LUT_LENGTH] = {0x00U};

    /* Copy LUT information from flash region into RAM region, because LUT update maybe corrupt read sequence(LUT[0])
     * and load wrong LUT table from FLASH region. */
    memcpy(tempLUT, customLUT, sizeof(tempLUT));

    xspi_clock_init();

    /*Get XSPI default settings and configure the xspi. */
    XSPI_GetDefaultConfig(base, &config, &deviceconfig);

    /*Set AHB buffer size for reading data through AHB bus. */
    config.rxSampleClock                  = EXAMPLE_XSPI_RX_SAMPLE_CLOCK;
    XSPI_Init(base, &config);

    /* Configure flash settings according to serial flash feature. */
    XSPI_SetFlashConfig(base, &deviceconfig);

    /*update LUT*/
    XSPI_UpdateLUT(base, 0, tempLUT, CUSTOM_LUT_LENGTH);

}
