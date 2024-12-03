/*
 * Copyright (c) 2014, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <stdlib.h>
#include <string.h>
#include "board.h"
#include "app.h"
#include "fsl_nor_flash.h"
#include "fsl_spi_adapter.h"
#include "fsl_common.h"
#include "fsl_debug_console.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define VERIFY_PATTERN 0x5A
#define ERASE_32K_SIZE 32 * 1024
#define ERASE_64K_SIZE 64 * 1024
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*!
 * @brief Show the menu for user.
 *
 */
static uint8_t show_menu(void);

/*******************************************************************************
 * variables
 ******************************************************************************/

static char s_StrInputAddress[] = "\r\nInput the %s address(HEX), 0x";
static char s_StrInputLength[]  = "\r\nInput the %s length:";
static char s_StrInvalid[]      = "Invalid input!\r\n";
static char s_StrEraseFail[]    = "Erase failed!\r\n";
static char s_StrEraseOk[]      = "Erase OK!\r\n";

extern flash_handle_t handle;
extern flash_config_t flashConfig;

/*******************************************************************************
 * Code
 ******************************************************************************/

static uint8_t show_menu(void)
{
    char input;

    PRINTF("\r\n1 - Erase entire chip\r\n");
    PRINTF("2 - Erase sectors\r\n");
    PRINTF("3 - Erase block\r\n");
    PRINTF("4 - Program one page with pattern (0x%x) and verify\r\n", VERIFY_PATTERN);
    PRINTF("5 - Read byte\r\n");
    PRINTF("Please enter your choice (1-5): ");

    input = GETCHAR();
    PRINTF("%c\r\n", input);

    return input - '0';
}

/*Error trap function*/
void ErrorTrap(void)
{
    PRINTF("\n\rError occurred. Please check the configurations.\n\r");
    while (1)
    {
        ;
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    char input, buf[1024];
    uint32_t ret;
    uint32_t blkSize;
    uint32_t blockSize;
    uint32_t offset = 0x00U;
    uint32_t length = 0x00U;
    uint32_t i      = 0U;

    BOARD_InitHardware();

    PRINTF("\r\n***SPI Flash Demo***\r\n");

    NOR_Flash_Init(&handle, &flashConfig);

    while (1)
    {
        /* showing the UI */
        input = show_menu();

        offset = 0x00U;

        switch (input)
        {
            case 1:
                /* erase all chips */
                ret = NOR_Flash_ChipErase(&handle);
                if (ret)
                {
                    PRINTF(s_StrEraseFail);
                }
                else
                {
                    PRINTF(s_StrEraseOk);
                }
                break;

            case 2:
                PRINTF(s_StrInputAddress, "sector");
                SCANF("%x", &offset);

                PRINTF(s_StrInputLength, "sector");
                SCANF("%d", &length);
                PRINTF("\r\n");

                if ((offset >= handle.totalSize) || (length <= 0))
                {
                    PRINTF(s_StrInvalid);
                    break;
                }

                /* erase sector */
                ret = NOR_Flash_EraseBlock(&handle, offset, length);
                if (ret)
                {
                    PRINTF(s_StrEraseFail);
                }
                else
                {
                    PRINTF(s_StrEraseOk);
                }
                break;

            case 3:
                PRINTF(s_StrInputAddress, "block");
                SCANF("%x", &offset);
                PRINTF("\r\n1. 32K block size");
                PRINTF("\r\n2. 64K block size");
                PRINTF("\r\nSelect the block size:");
                blkSize = GETCHAR();
                blkSize -= '0';
                PRINTF("%d\r\n", blkSize);

                if ((offset >= handle.totalSize) || ((blkSize != 1) && (blkSize != 2)))
                {
                    PRINTF(s_StrInvalid);
                    break;
                }
                blockSize = (blkSize == 1 ? ERASE_32K_SIZE : ERASE_64K_SIZE);
                if ((offset % blockSize))
                {
                    PRINTF(s_StrInvalid);
                    break;
                }

                /* erase one block */
                ret = NOR_Flash_EraseBlock(&handle, offset, blockSize);
                if (ret)
                {
                    PRINTF(s_StrEraseFail);
                }
                else
                {
                    PRINTF(s_StrEraseOk);
                }
                break;

            case 4:
                /* get the program address and length */
                PRINTF(s_StrInputAddress, "program");
                SCANF("%x", &offset);
                PRINTF("\r\n");

                if (offset > (handle.totalSize - handle.pageSize))
                {
                    PRINTF(s_StrInvalid);
                    break;
                }

                /* erase sectors first */
                ret = NOR_Flash_EraseBlock(&handle, (offset & ~(handle.sectorSize - 1)), handle.sectorSize);
                if (ret)
                {
                    PRINTF(s_StrEraseFail);
                    break;
                }

                /* fill the write buffer with pattern */
                memset(buf, VERIFY_PATTERN, handle.pageSize);
                ret = NOR_Flash_WriteData(&handle, offset, (void *)buf, handle.pageSize);
                if (ret)
                {
                    PRINTF("Program page failed!\r\n");
                    break;
                }

                /* read back and verify */
                memset(buf, 0, handle.pageSize);
                ret = NOR_Flash_ReadData(&handle, offset, (void *)buf, handle.pageSize);
                if (ret)
                {
                    PRINTF("Read page failed!\r\n");
                    break;
                }

                /* compare */
                for (i = 0; i < handle.pageSize; i++)
                {
                    if (buf[i] != VERIFY_PATTERN)
                    {
                        PRINTF("Program and verify failed!\r\n");
                        break;
                    }
                }

                PRINTF("Program and verify done!\r\n");
                break;

            case 5:
                /* get the read address */
                PRINTF(s_StrInputAddress, "read");
                SCANF("%x", &offset);
                PRINTF("\r\n");

                if (offset >= handle.totalSize)
                {
                    PRINTF(s_StrInvalid);
                    break;
                }

                ret = NOR_Flash_ReadData(&handle, offset, (void *)buf, 1);
                if (ret)
                {
                    PRINTF("Read byte failed!\r\n");
                    break;
                }
                PRINTF("0x%x = 0x%x\r\n", offset, buf[0]);
                break;

            default:
                PRINTF(s_StrInvalid);
                break;
        }
    }
}
