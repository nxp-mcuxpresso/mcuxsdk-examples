/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "app.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "mx25l_flash.h"
#include "fsl_queued_spi.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define BUFFER_SIZE 64

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
char g_buffer[BUFFER_SIZE] = {0};
struct mx25l_instance mx25l;

/*******************************************************************************
 * Code
 ******************************************************************************/
static void flash_transfer_cb(
    void *transfer_prv, uint8_t *u8MasterTxDatas, uint8_t *u8MasterRxDatas, size_t u16TransferSize, bool eof)
{
    qspi_transfer_t sMasterXfer = {0};
    /* Configure master transfer. */
    sMasterXfer.pTxData       = u8MasterTxDatas;
    sMasterXfer.pRxData       = u8MasterRxDatas;
    sMasterXfer.u16DataSize   = u16TransferSize;
    sMasterXfer.u8ConfigFlags = kQSPI_MasterPCSContinous;
    if (!eof)
    {
        sMasterXfer.u8ConfigFlags |= kQSPI_MasterActiveAfterTransfer;
    }

    /* Start transfer. */
    QSPI_MasterTransferBlocking(EXAMPLE_QSPI_MASTER_BASEADDR, &sMasterXfer);
}

static int flash_init(void)
{
    qspi_master_config_t sMasterConfig;
    QSPI_MasterGetDefaultConfig(&sMasterConfig, EXAMPLE_QSPI_CLK_FREQ);
    sMasterConfig.u32BaudRateBps = 100000UL;
    sMasterConfig.eClkPolarity   = kQSPI_ClockPolarityActiveFallingEdge;
    sMasterConfig.eClkPhase      = kQSPI_ClockPhaseSlaveSelectLowBetweenWords;
    QSPI_MasterInit(EXAMPLE_QSPI_MASTER_BASEADDR, &sMasterConfig);

    mx25l_init(&mx25l, flash_transfer_cb, EXAMPLE_QSPI_MASTER_BASEADDR);
    return mx25l_err_ok;
}

static mx25l_err_t flash_is_sector_aligned(uint32_t address)
{
    return (address % SECTOR_SIZE) ? mx25l_err_alignement : mx25l_err_ok;
}

static bool flash_is_buffer_empty(char *buffer, uint32_t size)
{
    assert(NULL != buffer);
    int i = 0;
    for (; ((i < size) && (buffer[i] == 0xFF)); i++)
    {
    }
    return i == size ? true : false;
}

static int flash_read_buffer(uint32_t address, char *buffer, uint32_t size)
{
    /* check sector alignement */
    int status = flash_is_sector_aligned(address);
    if (mx25l_err_ok != status)
    {
        return status;
    }
    /* read command */
    status = mx25l_cmd_read(&mx25l, address, (uint8_t *)buffer, size);
    if (mx25l_err_ok != status)
    {
        PRINTF("'mx25l_cmd_read' failed \r\n");
    }
    return status;
}

static int flash_erase_sector(uint32_t address)
{
    /* check sector alignement */
    int status = flash_is_sector_aligned(address);
    if (mx25l_err_ok != status)
    {
        return status;
    }
    /* erase command */
    status = mx25l_cmd_sector_erase(&mx25l, address);
    if (mx25l_err_ok != status)
    {
        PRINTF("'mx25l_cmd_sector_erase' failed \r\n");
    }
    return status;
}

static int flash_write_buffer(uint32_t address, char *buffer, uint32_t size)
{
    /* check sector alignement */
    int status = flash_is_sector_aligned(address);
    if (mx25l_err_ok != status)
    {
        return status;
    }
    /* write command */
    status = mx25l_cmd_write(&mx25l, address, (uint8_t *)buffer, size);
    if (mx25l_err_ok != status)
    {
        PRINTF("'mx25l_cmd_write' failed \r\n");
    }
    return status;
}

static void read_string(char *buffer, int size)
{
    int character;
    int i = 0;
    for (; i + 1 < size; i++)
    {
        character = GETCHAR();
        if (('\n' == character) || ('\r' == character))
        {
            break;
        }
        buffer[i] = character;
        PUTCHAR(character);
    }
    buffer[i] = 0;
}

static int app(void)
{
    int status;
    if (mx25l_err_ok != flash_init())
    {
        return -1;
    }

    /* read buffer from flash */
    status = flash_read_buffer(SECTOR_ADDR, g_buffer, BUFFER_SIZE);
    if (mx25l_err_ok != status)
    {
        return status;
    }
    /* consider buffer as empty */
    if (flash_is_buffer_empty(g_buffer, BUFFER_SIZE))
    {
        PRINTF("Flash at 0x%x of size %d B is empty \r\n", SECTOR_ADDR, BUFFER_SIZE);
    }
    else
    {
        PRINTF("Flash at 0x%x of size %d B has message '%s' \r\n", SECTOR_ADDR, BUFFER_SIZE, g_buffer);
    }

    /* read user message */

    PRINTF("Write new message (max %d chars) to flash: \r\n", BUFFER_SIZE - 1);
    read_string(g_buffer, BUFFER_SIZE);
    PRINTF("\r\nmessage is: '%s' \r\n", g_buffer);

    if (!flash_is_buffer_empty(g_buffer, BUFFER_SIZE))
    {
        status = flash_erase_sector(SECTOR_ADDR);
        if (mx25l_err_ok != status)
        {
            return status;
        }
    }

    status = flash_write_buffer(0x0, g_buffer, BUFFER_SIZE);
    if (mx25l_err_ok != status)
    {
        return status;
    }

    return 0;
}

int main(void)
{
    /* Init the boards */
    BOARD_InitHardware();
    PRINTF("*****app*start***** \r\n");

    if (0 == app())
    {
        PRINTF("Write succeed, please restart the board to see the written message \r\n");
    }
    else
    {
        PRINTF("Write failed \r\n");
    }
    PRINTF("*****app*end***** \r\n");
    while (1)
    {
    }
}
