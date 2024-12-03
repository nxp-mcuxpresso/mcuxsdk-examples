/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_flexspi.h"
#include "app.h"
#include "fsl_debug_console.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define NAND_CMD_LUT_SEQ_IDX_READCACHE      0
#define NAND_CMD_LUT_SEQ_IDX_READCACHE2     8
#define NAND_CMD_LUT_SEQ_IDX_READCACHE4     9
#define NAND_CMD_LUT_SEQ_IDX_READSTATUS     1
#define NAND_CMD_LUT_SEQ_IDX_WRITEENABLE    2
#define NAND_CMD_LUT_SEQ_IDX_ERASEBLOCK     3
#define NAND_CMD_LUT_SEQ_IDX_PROGRAMLOAD    4
#define NAND_CMD_LUT_SEQ_IDX_PROGRAMLOAD4   10
#define NAND_CMD_LUT_SEQ_IDX_READPAGE       5
#define NAND_CMD_LUT_SEQ_IDX_READECCSTAT    6
#define NAND_CMD_LUT_SEQ_IDX_PROGRAMEXECUTE 7
#define NAND_CMD_LUT_SEQ_IDX_READJEDECID    11
#define NAND_CMD_LUT_SEQ_IDX_UNLOCKALL      12

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
flexspi_device_config_t deviceconfig = {
    .flexspiRootClk       = 30000000U, /* 30MHZ SPI serial clock */
    .flashSize            = FLASH_SIZE,
    .CSIntervalUnit       = kFLEXSPI_CsIntervalUnit1SckCycle,
    .CSInterval           = 1000,
    .CSHoldTime           = 3,
    .CSSetupTime          = 3,
    .dataValidTime        = 1,
    .columnspace          = 12,
    .enableWordAddress    = 0,
    .AWRSeqIndex          = 0,
    .AWRSeqNumber         = 0,
    .ARDSeqIndex          = NAND_CMD_LUT_SEQ_IDX_READCACHE,
    .ARDSeqNumber         = 1,
    .AHBWriteWaitUnit     = kFLEXSPI_AhbWriteWaitUnit2AhbCycle,
    .AHBWriteWaitInterval = 0,
};

uint32_t customLUT[56] = {
    /* Read Cache 1X */
    [4 * NAND_CMD_LUT_SEQ_IDX_READCACHE] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x03, kFLEXSPI_Command_CADDR_SDR, kFLEXSPI_1PAD, 0x10),
    [4 * NAND_CMD_LUT_SEQ_IDX_READCACHE + 1] = FLEXSPI_LUT_SEQ(
        kFLEXSPI_Command_DUMMY_SDR, kFLEXSPI_1PAD, 0x08, kFLEXSPI_Command_READ_SDR, kFLEXSPI_1PAD, 0x80),

    [4 * NAND_CMD_LUT_SEQ_IDX_READCACHE2] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x3B, kFLEXSPI_Command_CADDR_SDR, kFLEXSPI_1PAD, 0x10),
    [4 * NAND_CMD_LUT_SEQ_IDX_READCACHE2 + 1] = FLEXSPI_LUT_SEQ(
        kFLEXSPI_Command_DUMMY_SDR, kFLEXSPI_1PAD, 0x08, kFLEXSPI_Command_READ_SDR, kFLEXSPI_2PAD, 0x80),

    [4 * NAND_CMD_LUT_SEQ_IDX_READCACHE4] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x6B, kFLEXSPI_Command_CADDR_SDR, kFLEXSPI_1PAD, 0x10),
    [4 * NAND_CMD_LUT_SEQ_IDX_READCACHE4 + 1] = FLEXSPI_LUT_SEQ(
        kFLEXSPI_Command_DUMMY_SDR, kFLEXSPI_4PAD, 0x08, kFLEXSPI_Command_READ_SDR, kFLEXSPI_4PAD, 0x80),

    /* Read Status */
    [4 * NAND_CMD_LUT_SEQ_IDX_READSTATUS] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x0F, kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0xC0),
    [4 * NAND_CMD_LUT_SEQ_IDX_READSTATUS + 1] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_READ_SDR, kFLEXSPI_1PAD, 0x01, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0),

    /* Write Enable */
    [4 * NAND_CMD_LUT_SEQ_IDX_WRITEENABLE] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x06, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0),

    /* Erase block */
    [4 * NAND_CMD_LUT_SEQ_IDX_ERASEBLOCK] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0xD8, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_1PAD, 0x18),

    /* Page Program Load */
    [4 * NAND_CMD_LUT_SEQ_IDX_PROGRAMLOAD] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x02, kFLEXSPI_Command_CADDR_SDR, kFLEXSPI_1PAD, 0x10),
    [4 * NAND_CMD_LUT_SEQ_IDX_PROGRAMLOAD + 1] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_WRITE_SDR, kFLEXSPI_1PAD, 0x40, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0),
    [4 * NAND_CMD_LUT_SEQ_IDX_PROGRAMLOAD4] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x32, kFLEXSPI_Command_CADDR_SDR, kFLEXSPI_1PAD, 0x10),
    [4 * NAND_CMD_LUT_SEQ_IDX_PROGRAMLOAD4 + 1] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_WRITE_SDR, kFLEXSPI_4PAD, 0x40, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0),

    /* Page Program Execute */
    [4 * NAND_CMD_LUT_SEQ_IDX_PROGRAMEXECUTE] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x10, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_1PAD, 0x18),

    /* Read Page */
    [4 * NAND_CMD_LUT_SEQ_IDX_READPAGE] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x13, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_1PAD, 0x18),

    /* Read ECC status */
    [4 * NAND_CMD_LUT_SEQ_IDX_READECCSTAT] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x0F, kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0xC0),
    [4 * NAND_CMD_LUT_SEQ_IDX_READECCSTAT + 1] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_READ_SDR, kFLEXSPI_1PAD, 0x01, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0),

    /* Read JEDEC ID */
    [4 * NAND_CMD_LUT_SEQ_IDX_READJEDECID] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x9F, kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0),
    [4 * NAND_CMD_LUT_SEQ_IDX_READJEDECID + 1] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_READ_SDR, kFLEXSPI_1PAD, 0x01, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0),

    /* Unlock all blocks */
    [4 * NAND_CMD_LUT_SEQ_IDX_UNLOCKALL] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x1F, kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0xA0),
    [4 * NAND_CMD_LUT_SEQ_IDX_UNLOCKALL + 1] =
        FLEXSPI_LUT_SEQ(kFLEXSPI_Command_WRITE_SDR, kFLEXSPI_1PAD, 0x00, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0),
};

static uint8_t s_nand_program_buffer[2048];
static uint8_t s_nand_read_buffer[2048];

/*******************************************************************************
 * Code
 ******************************************************************************/

status_t flexspi_nand_write_enable(FLEXSPI_Type *base, uint32_t baseAddr)
{
    flexspi_transfer_t flashXfer;
    status_t status;

    /* Write neable */
    flashXfer.deviceAddress = baseAddr;
    flashXfer.port          = kFLEXSPI_PortA1;
    flashXfer.cmdType       = kFLEXSPI_Command;
    flashXfer.SeqNumber     = 1;
    flashXfer.seqIndex      = NAND_CMD_LUT_SEQ_IDX_WRITEENABLE;

    status = FLEXSPI_TransferBlocking(base, &flashXfer);

    return status;
}

status_t flexspi_nand_check_ecc_status(FLEXSPI_Type *base, uint32_t address, bool *isCheckPassed)
{
    flexspi_transfer_t flashXfer;
    status_t status;
    uint32_t eccStatus;

    /* Write neable */
    flashXfer.deviceAddress = address;
    flashXfer.port          = kFLEXSPI_PortA1;
    flashXfer.cmdType       = kFLEXSPI_Read;
    flashXfer.SeqNumber     = 1;
    flashXfer.seqIndex      = NAND_CMD_LUT_SEQ_IDX_READECCSTAT;
    flashXfer.data          = &eccStatus;
    flashXfer.dataSize      = 1;

    status = FLEXSPI_TransferBlocking(base, &flashXfer);
    if (status != kStatus_Success)
    {
        return status;
    }

    *isCheckPassed = false;

    if ((eccStatus & FLASH_ECC_STATUS_MASK) != FLASH_ECC_ERR_MASK)
    {
        *isCheckPassed = true;
    }

    return status;
}

status_t flexspi_nand_wait_bus_busy(FLEXSPI_Type *base, uint32_t baseAddr)
{
    /* Wait status ready. */
    bool isBusy;
    uint32_t readValue;
    status_t status;
    flexspi_transfer_t flashXfer;

    flashXfer.deviceAddress = baseAddr;
    flashXfer.port          = kFLEXSPI_PortA1;
    flashXfer.cmdType       = kFLEXSPI_Read;
    flashXfer.SeqNumber     = 1;
    flashXfer.seqIndex      = NAND_CMD_LUT_SEQ_IDX_READSTATUS;
    flashXfer.data          = &readValue;
    flashXfer.dataSize      = 1;

    do
    {
        status = FLEXSPI_TransferBlocking(base, &flashXfer);

        if (status != kStatus_Success)
        {
            return status;
        }

        if (readValue & (1U << FLASH_BUSY_STATUS_OFFSET))
        {
            isBusy = true;
        }
        else
        {
            isBusy = false;
        }

    } while (isBusy);

    return status;
}

status_t flexspi_nand_read_page(
    FLEXSPI_Type *base, uint32_t pageId, uint32_t *buffer, uint32_t length, bool checkEccStatus)
{
    status_t status;
    flexspi_transfer_t flashXfer;
    uint32_t readAddress = pageId * FLASH_PAGE_SIZE;

    /* Read page. */
    flashXfer.deviceAddress = readAddress;
    flashXfer.port          = kFLEXSPI_PortA1;
    flashXfer.cmdType       = kFLEXSPI_Command;
    flashXfer.SeqNumber     = 1;
    flashXfer.seqIndex      = NAND_CMD_LUT_SEQ_IDX_READPAGE;

    status = FLEXSPI_TransferBlocking(base, &flashXfer);

    if (status != kStatus_Success)
    {
        return status;
    }

    status = flexspi_nand_wait_bus_busy(base, readAddress);
    if (status != kStatus_Success)
    {
        return status;
    }

    /* Clear cache by do a software reset. */
    FLEXSPI_SoftwareReset(base);

    flashXfer.deviceAddress = readAddress;
    flashXfer.port          = kFLEXSPI_PortA1;
    flashXfer.cmdType       = kFLEXSPI_Read;
    flashXfer.SeqNumber     = 1;
    flashXfer.seqIndex      = NAND_CMD_LUT_SEQ_IDX_READCACHE;
    flashXfer.data          = buffer;
    flashXfer.dataSize      = length;

    status = FLEXSPI_TransferBlocking(base, &flashXfer);

    if (status != kStatus_Success)
    {
        return status;
    }

    if (checkEccStatus)
    {
        bool isCheckPassed;
        status = flexspi_nand_check_ecc_status(base, readAddress, &isCheckPassed);

        if (isCheckPassed == false)
        {
            status = kStatus_Fail;
        }

        if (status != kStatus_Success)
        {
            return status;
        }
    }

    return status;
}

status_t flexspi_nand_erase_block(FLEXSPI_Type *base, uint32_t blockId, bool checkEccStatus)
{
    status_t status;
    flexspi_transfer_t flashXfer;
    uint32_t address = blockId * FLASH_PAGE_PER_BLOCK * FLASH_PAGE_SIZE;

    /* Write enable */
    status = flexspi_nand_write_enable(base, address);
    if (status != kStatus_Success)
    {
        return status;
    }

    /* Send erase command. */
    flashXfer.deviceAddress = address;
    flashXfer.port          = kFLEXSPI_PortA1;
    flashXfer.cmdType       = kFLEXSPI_Command;
    flashXfer.SeqNumber     = 1;
    flashXfer.seqIndex      = NAND_CMD_LUT_SEQ_IDX_ERASEBLOCK;

    status = FLEXSPI_TransferBlocking(base, &flashXfer);
    if (status != kStatus_Success)
    {
        return status;
    }

    flexspi_nand_wait_bus_busy(base, address);

    if (checkEccStatus)
    {
        bool isCheckPassed;
        status = flexspi_nand_check_ecc_status(base, address, &isCheckPassed);

        if (isCheckPassed == false)
        {
            status = kStatus_Fail;
        }

        if (status != kStatus_Success)
        {
            return status;
        }
    }

    return status;
}

status_t flexspi_nand_program_page(
    FLEXSPI_Type *base, uint32_t pageId, uint32_t *buffer, uint32_t length, bool checkEccStatus)
{
    status_t status;
    flexspi_transfer_t flashXfer;
    uint32_t address = pageId * FLASH_PAGE_SIZE;

    /* Write enable. */
    status = flexspi_nand_write_enable(base, address);
    if (status != kStatus_Success)
    {
        return status;
    }

    /* Send page load command. */
    flashXfer.deviceAddress = address;
    flashXfer.port          = kFLEXSPI_PortA1;
    flashXfer.cmdType       = kFLEXSPI_Write;
    flashXfer.SeqNumber     = 1;
    flashXfer.seqIndex      = NAND_CMD_LUT_SEQ_IDX_PROGRAMLOAD;
    flashXfer.data          = buffer;
    flashXfer.dataSize      = length;

    status = FLEXSPI_TransferBlocking(base, &flashXfer);
    if (status != kStatus_Success)
    {
        return status;
    }

    /* Send program execute command. */
    flashXfer.deviceAddress = address;
    flashXfer.port          = kFLEXSPI_PortA1;
    flashXfer.cmdType       = kFLEXSPI_Command;
    flashXfer.SeqNumber     = 1;
    flashXfer.seqIndex      = NAND_CMD_LUT_SEQ_IDX_PROGRAMEXECUTE;
    status                  = FLEXSPI_TransferBlocking(base, &flashXfer);
    if (status != kStatus_Success)
    {
        return status;
    }

    flexspi_nand_wait_bus_busy(base, address);

    if (checkEccStatus)
    {
        bool isCheckPassed;
        status = flexspi_nand_check_ecc_status(base, address, &isCheckPassed);

        if (isCheckPassed == false)
        {
            status = kStatus_Fail;
        }

        if (status != kStatus_Success)
        {
            return status;
        }
    }

    return status;
}

status_t flexspi_nand_unlock_all_blocks(FLEXSPI_Type *base)
{
    uint32_t temp;
    flexspi_transfer_t flashXfer;
    status_t status = kStatus_Success;

    /* unlock all blocks. */
    temp                    = 0;
    flashXfer.deviceAddress = 0;
    flashXfer.port          = kFLEXSPI_PortA1;
    flashXfer.cmdType       = kFLEXSPI_Write;
    flashXfer.SeqNumber     = 1;
    flashXfer.seqIndex      = NAND_CMD_LUT_SEQ_IDX_UNLOCKALL;
    flashXfer.data          = &temp;
    flashXfer.dataSize      = 1;

    status = FLEXSPI_TransferBlocking(base, &flashXfer);

    return status;
}

status_t flexspi_nand_get_vendor_id(FLEXSPI_Type *base, uint8_t *vendorId)
{
    uint32_t temp;
    flexspi_transfer_t flashXfer;
    flashXfer.deviceAddress = 0;
    flashXfer.port          = kFLEXSPI_PortA1;
    flashXfer.cmdType       = kFLEXSPI_Read;
    flashXfer.SeqNumber     = 1;
    flashXfer.seqIndex      = NAND_CMD_LUT_SEQ_IDX_READJEDECID;
    flashXfer.data          = &temp;
    flashXfer.dataSize      = 2;

    status_t status = FLEXSPI_TransferBlocking(base, &flashXfer);

    // Winbond       :   0xEF
    // Micron        :   0x2C
    // GigaDevice    :   0xC8
    // MXIC          :   0xC2
    // FM            :   0xA1
    // Toshiba       :   0x98
    *vendorId = temp;

    return status;
}

int main(void)
{
    uint32_t i = 0;
    flexspi_config_t config;
    status_t status;
    uint8_t vendorID = 0;

    BOARD_InitHardware();

    PRINTF("FLEXSPI example started!\r\n");

    /* Get FLEXSPI default settings and configure the flexspi. */
    FLEXSPI_GetDefaultConfig(&config);

    /* Init FLEXSPI. */
    FLEXSPI_Init(EXAMPLE_FLEXSPI, &config);

    /* Configure flash settings according to serial flash feature. */
    FLEXSPI_SetFlashConfig(EXAMPLE_FLEXSPI, &deviceconfig, kFLEXSPI_PortA1);

    /* Update LUT table. */
    FLEXSPI_UpdateLUT(EXAMPLE_FLEXSPI, 0, customLUT, ARRAY_SIZE(customLUT));

    /* Do software reset. */
    FLEXSPI_SoftwareReset(EXAMPLE_FLEXSPI);

    /* Unlock all blocks. */
    status = flexspi_nand_unlock_all_blocks(EXAMPLE_FLEXSPI);
    if (status != kStatus_Success)
    {
        return status;
    }
    /* Get vendor ID. */
    status = flexspi_nand_get_vendor_id(EXAMPLE_FLEXSPI, &vendorID);
    if (status != kStatus_Success)
    {
        return status;
    }
    PRINTF("Vendor ID: %d \r\n", vendorID);

    /* Erase block. */
    PRINTF("Erasing Serial Nand over FlexSPI...\r\n");
    status = flexspi_nand_erase_block(EXAMPLE_FLEXSPI, EXAMPLE_BLOCK, false);
    if (status != kStatus_Success)
    {
        PRINTF("Erase sector failure !\r\n");
        return -1;
    }

    status = flexspi_nand_read_page(EXAMPLE_FLEXSPI, EXAMPLE_BLOCK * FLASH_PAGE_PER_BLOCK,
                                    (uint32_t *)s_nand_read_buffer, 2048, false);
    if (status != kStatus_Success)
    {
        PRINTF("Read page failure !\r\n");
        return -1;
    }

    memset(s_nand_program_buffer, 0xFF, sizeof(s_nand_program_buffer));

    if (memcmp(s_nand_program_buffer, s_nand_read_buffer, sizeof(s_nand_program_buffer)))
    {
        PRINTF("Erase data -  read out data value incorrect !\r\n ");
    }
    else
    {
        PRINTF("Erase data - successfully. \r\n");
    }

    for (i = 0; i < sizeof(s_nand_program_buffer); i++)
    {
        s_nand_program_buffer[i] = (uint8_t)(i & 0xFF);
    }

    status = flexspi_nand_program_page(EXAMPLE_FLEXSPI, EXAMPLE_BLOCK * FLASH_PAGE_PER_BLOCK,
                                       (void *)s_nand_program_buffer, 2048, false);
    if (status != kStatus_Success)
    {
        PRINTF("Page program failure !\r\n");
        return -1;
    }

    status = flexspi_nand_read_page(EXAMPLE_FLEXSPI, EXAMPLE_BLOCK * FLASH_PAGE_PER_BLOCK,
                                    (uint32_t *)s_nand_read_buffer, 2048, false);
    if (status != kStatus_Success)
    {
        PRINTF("Read page failure !\r\n");
        return -1;
    }

    if (memcmp(s_nand_read_buffer, s_nand_program_buffer, sizeof(s_nand_program_buffer)) != 0)
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
