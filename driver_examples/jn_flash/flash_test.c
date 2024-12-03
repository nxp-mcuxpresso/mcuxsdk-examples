/*
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"
#include "app.h"
#include "fsl_flash.h"
#include "fsl_debug_console.h"
#include <stdlib.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define FLASH_BUFF_SIZE     128
#define FLASH_START_ADDRESS 0x10000
#define FLASH_START_PAGE    (FLASH_START_ADDRESS / FLASH_PAGE_SIZE)
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
uint32_t au32Data[FLASH_BUFF_SIZE];
uint32_t readData[FLASH_BUFF_SIZE] = {0};

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Main function
 */
int main(void)
{
    BOARD_InitHardware();

    PRINTF("Internal Flash Test\r\n");

    /* setup some test data */
    srand(0xBADD10DE);
    for (uint32_t i = 0; i < FLASH_BUFF_SIZE; i++)
    {
        au32Data[i] = (uint32_t)(rand());
    }

    /* Setup Flash Controller */
    FLASH_Init(FLASH);

    PRINTF("Flash initialised\r\n");

    FLASH_ErasePages(FLASH, FLASH_START_PAGE, 1);

    PRINTF("Flash erased \r\n");

    uint32_t *pu32Start = (uint32_t *)(FLASH_START_ADDRESS);

    PRINTF("Programming %d words\r\n", FLASH_BUFF_SIZE);
    FLASH_Program(FLASH, pu32Start, au32Data, FLASH_BUFF_SIZE * sizeof(uint32_t));

    PRINTF("Flash programmed %d words\r\n", FLASH_BUFF_SIZE);

    /* Read back data */
    uint32_t temp[4];
    for (uint32_t i = 0; i < FLASH_BUFF_SIZE; i += 4)
    {
        FLASH_Read(FLASH, (uint8_t *)(pu32Start + i), 0, (uint32_t *)temp);
        memcpy(&readData[i], (void *)temp, sizeof(temp));
    }

    if (!memcmp((void *)FLASH_START_ADDRESS, au32Data, sizeof(readData)))
    {
        PRINTF("%d bytes verified successfully via direct flash read\r\n", FLASH_BUFF_SIZE * 4);
    }
    else
    {
        PRINTF("Data error\r\n");
    }

    if (!memcmp(readData, au32Data, sizeof(readData)))
    {
        PRINTF("%d bytes verified successfully via API flash read\r\n", FLASH_BUFF_SIZE * 4);
    }
    else
    {
        PRINTF("Data error\r\n");
    }

    while (1)
    {
    }
}
