/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_iap.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void error_trap();

/*******************************************************************************
 * Variables
 ******************************************************************************/
#define BUFFER_LEN (FLASH_PAGE_SIZE / 4)
static uint32_t d_buffer[BUFFER_LEN];
static uint32_t s_buffer[BUFFER_LEN];
/*******************************************************************************
 * Code
 ******************************************************************************/
AT_QUICKACCESS_SECTION_CODE(void qspi_init(quadspi_nor_config_t *config, serial_nor_config_option_t *option))
{
    if (IAP_QspiGetConfig(config, option) == kStatus_Success)
    {
        if (IAP_QspiInit(config) == kStatus_Success)
        {
            PRINTF("Flash Init successfull!\r\n");
        }
        else
        {
            error_trap();
        }
    }
    else
    {
        error_trap();
    }
}
int main()
{
    quadspi_nor_config_t flashConfig;
    serial_nor_config_option_t configOption = EXAMPLE_CONFIG_OPTION;
    static uint32_t status;
    uint32_t i;
    uint32_t err = 0;
    /* Init board hardware. */
    BOARD_InitHardware();

    /* Initialize data buffer */
    for (i = 0; i < BUFFER_LEN; i++)
    {
        s_buffer[i] = i;
    }

    PRINTF("\r\nIAP QSPI FLASH example\r\n");
    /* Initialize flash driver */
    PRINTF("Initializing flash driver...\r\n");
    qspi_init(&flashConfig, &configOption);

    PRINTF("Calling Erase() API...\r\n");
    status = IAP_QspiErase(&flashConfig, EXAMPLE_TEST_DEST_ADDRESS, EXAMPLE_FLASH_ERASE_SIZE);
    if (status == kStatus_Success)
    {
        PRINTF("FLASH Erase successful!\r\n");
    }
    else
    {
        error_trap();
    }

    /* Start programming specified flash region */
    PRINTF("Calling Page Program() API...\r\n");
    status = IAP_QspiPageProgram(&flashConfig, EXAMPLE_TEST_DEST_ADDRESS, s_buffer);
    if (status == kStatus_Success)
    {
        PRINTF("FLASH Page Program successful!\r\n");
    }
    else
    {
        error_trap();
    }

    /* Read data from programmed flash region */
    PRINTF("Calling Read() API...\r\n");
    status = IAP_QspiRead(&flashConfig, d_buffer, EXAMPLE_TEST_DEST_ADDRESS, sizeof(d_buffer));
    for (i = 0; i < BUFFER_LEN; i++)
    {
        if (((uint32_t *)d_buffer)[i] != s_buffer[i % 64])
        {
            PRINTF("The data in %d is wrong!!\r\n", i);
            PRINTF("The flash value in %d is %d\r\n", i, ((uint32_t *)d_buffer)[i]);
            err++;
        }
    }
    if ((status == kStatus_Success) && (err == 0))
    {
        PRINTF("FLASH Read successful!\r\n");
    }
    else
    {
        error_trap();
    }

    /* Verify programming by reading back from flash directly */
    err = 0;
    for (i = 0; i < BUFFER_LEN; i++)
    {
        if ((*(volatile uint32_t *)(EXAMPLE_TEST_DEST_ADDRESS + i * 4)) != s_buffer[i])
        {
            PRINTF("The data in %d is wrong!!\r\n", i);
            PRINTF("The flash value in %d is %d\r\n", i, ((uint32_t *)d_buffer)[i]);
            err++;
        }
    }
    if (err == 0)
    {
        PRINTF("Successfully Programmed and Verified Location 0x%x -> 0x%x \r\n", EXAMPLE_TEST_DEST_ADDRESS,
               (EXAMPLE_TEST_DEST_ADDRESS + sizeof(s_buffer)));
    }
    else
    {
    }

    /* resume flash memory status */
    status = IAP_QspiErase(&flashConfig, EXAMPLE_TEST_DEST_ADDRESS, EXAMPLE_FLASH_ERASE_SIZE);
    if (status == kStatus_Success)
    {
        PRINTF("Resume FLASH memory status successful!\r\n");
    }
    else
    {
        error_trap();
    }

    PRINTF("Done!\r\n");

    while (1)
    {
    }
}

/*
 * @brief Gets called when an error occurs.
 */
void error_trap(void)
{
    PRINTF("\r\n\r\n\r\n\t---- HALTED DUE TO FLASH ERROR! ----");
    while (1)
    {
    }
}
