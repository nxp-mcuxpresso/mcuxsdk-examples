/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_spi_filter.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_flexspi.h"

extern status_t flexspi_nor_flash_erase_sector(FLEXSPI_Type *base, uint32_t address);
extern status_t flexspi_nor_flash_page_program(FLEXSPI_Type *base, uint32_t dstAddr, const uint32_t *src);
extern status_t flexspi_nor_get_vendor_id(FLEXSPI_Type *base, uint8_t *vendorId);
extern status_t flexspi_nor_enable_quad_mode(FLEXSPI_Type *base);
extern status_t flexspi_nor_erase_chip(FLEXSPI_Type *base);
extern void flexspi_nor_flash_init(FLEXSPI_Type *base);

static uint8_t s_nor_program_buffer[256];
static uint8_t s_nor_read_buffer[256];

int main(void)
{
    spi_filter_config_t cfg;
    status_t status;
    uint32_t opCode;

    BOARD_InitHardware();

    PRINTF("MCUX SDK version: %s\r\n", MCUXSDK_VERSION_FULL_STR);
    PRINTF("SPI Filter example start\r\n");

    flexspi_nor_flash_init(EXAMPLE_FLEXSPI);

    SPI_FILTER_GetDefaultConfig(&cfg);
    cfg.portConfig[kSPI_FilterPort1].enableFilter = true;
    cfg.portConfig[kSPI_FilterPort1].bypassMode   = kSPI_FilterBypassNone;
    cfg.portConfig[kSPI_FilterPort1].fixedAddrMode = false;
    cfg.portConfig[kSPI_FilterPort1].flashMode = kSPI_FilterSingleFlashCS0;
    cfg.portConfig[kSPI_FilterPort1].manufacturer = EXAMPLE_SPI_FILTER_MANUFACTURER;
    cfg.portConfig[kSPI_FilterPort1].maxFlashSize = 0x800000;
    SPI_FILTER_Init(EXAMPLE_SPI_FILTER_BASE, &cfg);

    uint8_t vendorID = 0;
    status = flexspi_nor_get_vendor_id(EXAMPLE_FLEXSPI, &vendorID);
    if (status != kStatus_Success)
    {
        return status;
    }
    PRINTF("Get Vendor ID 0x%X\r\n", vendorID);

    PRINTF("==== Opcode filter test ====\r\n");
    spi_filter_opcode_t opCodeCfg;
    opCodeCfg.opcode = 0x95;
    opCodeCfg.state = 1U;
    opCodeCfg.filter8thBit = false;

    status = SPI_FILTER_ConfigureOpcode(EXAMPLE_SPI_FILTER_BASE, 0U, &opCodeCfg);
    if (status != kStatus_Success)
    {
        PRINTF("SPI_FILTER_ConfigureOpcode failed: %d\r\n", status);
        return -1;
    }
    PRINTF("Configured opcode 0x%02X\r\n", opCodeCfg.opcode);

    flexspi_transfer_t flashXfer = {0};
    flashXfer.deviceAddress = 0;
    flashXfer.port          = FLASH_PORT;
    flashXfer.cmdType       = kFLEXSPI_Command;
    flashXfer.SeqNumber     = 1;
    flashXfer.seqIndex      = NOR_CMD_LUT_SEQ_IDX_TEST_0x95;
    status = FLEXSPI_TransferBlocking(EXAMPLE_FLEXSPI, &flashXfer);
    if (status != kStatus_Success)
    {
        PRINTF("FlexSPI transfer issue %d\r\n", status);
        return -1;
    }

    opCode = SPI_FILTER_GetBlockedOpcode(EXAMPLE_SPI_FILTER_BASE, kSPI_FilterPort1);
    if (opCode != 0U)
    {
        PRINTF("<E> Opcode filter feature doesn't work.\r\n");
        return -1;
    }
    PRINTF("Command 0x95 is not blocked. Opcode status 0x%02X\r\n", opCode);

    opCodeCfg.opcode = 0;

    status = SPI_FILTER_ConfigureOpcode(EXAMPLE_SPI_FILTER_BASE, 0U, &opCodeCfg);
    if (status != kStatus_Success)
    {
        PRINTF("SPI_FILTER_ConfigureOpcode failed: %d\r\n", status);
        return -1;
    }
    PRINTF("Configured opcode 0x%02X\r\n", opCodeCfg.opcode);

    status = FLEXSPI_TransferBlocking(EXAMPLE_FLEXSPI, &flashXfer);
    if (status != kStatus_Success)
    {
        PRINTF("FlexSPI transfer issue %d\r\n", status);
        return -1;
    }

    opCode = SPI_FILTER_GetBlockedOpcode(EXAMPLE_SPI_FILTER_BASE, kSPI_FilterPort1);
    if (opCode != 0x95U)
    {
        PRINTF("<E> Opcode block feature doesn't work.\r\n");
        return -1;
    }
    PRINTF("Command 0x%02X is blocked\r\n", opCode);

    PRINTF("==== Memory region filter test ====\r\n");
    spi_filter_region_t region;
    region.startAddr = EXAMPLE_FILTER_PERMITTED_ADDR; /* Must be 64KB aligned */
    region.endAddr   = EXAMPLE_FILTER_BLOCKED_ADDR; /* Must be 64KB aligned */

    status = SPI_FILTER_SetRegion(EXAMPLE_SPI_FILTER_BASE, kSPI_FilterPort1, 1U, &region);
    if (status != kStatus_Success)
    {
        PRINTF("SPI_FILTER_SetRegion failed: %d\r\n", status);
        return -1;
    }
    PRINTF("Set filter region: 0x%08X - 0x%08X\r\n", region.startAddr, region.endAddr);

    PRINTF("Touch memory address: 0x%08X\r\n", EXAMPLE_FILTER_PERMITTED_ADDR);
    status = flexspi_nor_flash_erase_sector(EXAMPLE_FLEXSPI, EXAMPLE_FILTER_PERMITTED_ADDR);
    if (status != kStatus_Success)
    {
        PRINTF("Erase sector failure !\r\n");
        return -1;
    }
    memset(s_nor_program_buffer, 0xFFU, sizeof(s_nor_program_buffer));
    memcpy(s_nor_read_buffer, (void *)(EXAMPLE_FLEXSPI_AMBA_BASE + EXAMPLE_FILTER_PERMITTED_ADDR),
           sizeof(s_nor_read_buffer));

    if (memcmp(s_nor_program_buffer, s_nor_read_buffer, sizeof(s_nor_program_buffer)))
    {
        PRINTF("Erase data - read out data value incorrect !\r\n ");
        return -1;
    }
    else
    {
        PRINTF("Erase data - successfully. \r\n");
    }

    for (uint32_t i = 0; i < 0xFFU; i++)
    {
        s_nor_program_buffer[i] = i;
    }

    status =
        flexspi_nor_flash_page_program(EXAMPLE_FLEXSPI, EXAMPLE_FILTER_PERMITTED_ADDR, (void *)s_nor_program_buffer);
    if (status != kStatus_Success)
    {
        PRINTF("Page program failure!\r\n");
        return -1;
    }

    memcpy(s_nor_read_buffer, (void *)(EXAMPLE_FLEXSPI_AMBA_BASE + EXAMPLE_FILTER_PERMITTED_ADDR),
           sizeof(s_nor_read_buffer));

    if (memcmp(s_nor_read_buffer, s_nor_program_buffer, sizeof(s_nor_program_buffer)) != 0)
    {
        PRINTF("Program data - read out data value incorrect !\r\n ");
        return -1;
    }
    else
    {
        PRINTF("Program data - successfully. \r\n");
    }

    PRINTF("Touch memory address: 0x%08X\r\n", EXAMPLE_FILTER_BLOCKED_ADDR);
    status = flexspi_nor_flash_erase_sector(EXAMPLE_FLEXSPI, EXAMPLE_FILTER_BLOCKED_ADDR);
    if (status != kStatus_Success)
    {
        PRINTF("Erase sector failure !\r\n");
        return -1;
    }

    status = flexspi_nor_flash_page_program(EXAMPLE_FLEXSPI, EXAMPLE_FILTER_BLOCKED_ADDR, (void *)s_nor_program_buffer);
    if (status != kStatus_Success)
    {
        PRINTF("Page program failure!\r\n");
        return -1;
    }

    memcpy(s_nor_read_buffer, (void *)(EXAMPLE_FLEXSPI_AMBA_BASE + EXAMPLE_FILTER_BLOCKED_ADDR),
           sizeof(s_nor_read_buffer));

    if (memcmp(s_nor_read_buffer, s_nor_program_buffer, sizeof(s_nor_program_buffer)) != 0)
    {
        PRINTF("Program data - Blocked by SPI Filter.\r\n");
    }
    else
    {
        PRINTF("<E> Program data - successfully. It's not as expexted!\r\n");
        return -1;
    }

    SPI_FILTER_ClearAllRegions(EXAMPLE_SPI_FILTER_BASE, kSPI_FilterPort0);
    SPI_FILTER_Deinit(EXAMPLE_SPI_FILTER_BASE);

    PRINTF("SPI Filter example finished\r\n");

    while(1)
    {
    }
}
