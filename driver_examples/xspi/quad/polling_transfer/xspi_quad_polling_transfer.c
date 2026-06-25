/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_xspi.h"
#include "app.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_cache.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
extern status_t xspi_quad_flash_erase_sector(XSPI_Type *base, uint32_t address);
extern status_t xspi_quad_flash_program(XSPI_Type *base, uint32_t dstAddr, const uint32_t *src, uint32_t length);
extern status_t xspi_quad_flash_read(XSPI_Type *base, uint32_t address, uint32_t *dst, uint32_t length);
extern status_t xspi_quad_full_init(XSPI_Type *base, uint8_t *vendorId);
extern void xspi_quad_reset_and_halt(XSPI_Type *base);
/*******************************************************************************
 * Variables
 ******************************************************************************/
#define MAX_TEST_SIZE 512U
SDK_ALIGN(static uint8_t s_nor_program_buffer[MAX_TEST_SIZE], 4);
SDK_ALIGN(static uint8_t s_nor_read_buffer[MAX_TEST_SIZE], 4);

/*******************************************************************************
 * Code
 ******************************************************************************/

int main(void)
{
    uint32_t i = 0;
    status_t status;
    uint8_t vendorID     = 0;
    uint32_t testSizes[] = {64U, 128U, 256U, 512U};
    uint32_t numTests    = sizeof(testSizes) / sizeof(testSizes[0]);
    uint32_t t;

    BOARD_InitHardware();

    PRINTF("MCUX SDK version: %s\r\n", MCUXSDK_VERSION_FULL_STR);
    PRINTF("XSPI Quad I/O polling example started!\r\n");

    status = xspi_quad_full_init(EXAMPLE_XSPI, &vendorID);
    if (status != kStatus_Success)
    {
        while (1)
        {
        }
    }

    PRINTF("4-byte address mode entered.\r\n");
    PRINTF("Quad I/O mode enabled.\r\n");
    PRINTF("Flash vendor ID: 0x%02X\r\n", vendorID);

    for (t = 0U; t < numTests; t++)
    {
        uint32_t testSize = testSizes[t];
        PRINTF("\r\n--- Test: %u bytes ---\r\n", testSize);

        /* Prepare test pattern. */
        for (i = 0U; i < testSize; i++)
        {
            s_nor_program_buffer[i] = (uint8_t)i;
        }

        /* Erase sector. */
        status = xspi_quad_flash_erase_sector(EXAMPLE_XSPI, EXAMPLE_SECTOR * SECTOR_SIZE);
        if (status != kStatus_Success)
        {
            PRINTF("  Erase fail!\r\n");
            return -1;
        }

        /* Program. */
        status = xspi_quad_flash_program(EXAMPLE_XSPI, EXAMPLE_SECTOR * SECTOR_SIZE,
                                         (const uint32_t *)(void *)s_nor_program_buffer, testSize);
        if (status != kStatus_Success)
        {
            PRINTF("  Program fail!\r\n");
            return -1;
        }

        /* IP read and verify. */
        memset(s_nor_read_buffer, 0, testSize);
        status = xspi_quad_flash_read(EXAMPLE_XSPI, EXAMPLE_SECTOR * SECTOR_SIZE, (uint32_t *)(void *)s_nor_read_buffer,
                                      testSize);
        if (status != kStatus_Success)
        {
            PRINTF("  IP read fail!\r\n");
            return -1;
        }

        if (memcmp(s_nor_read_buffer, s_nor_program_buffer, testSize) != 0)
        {
            PRINTF("  IP read verify FAIL!\r\n");
        }
        else
        {
            PRINTF("  IP read verify: PASS\r\n");
        }

        /* AHB read and verify. */
        DEMO_INVALIDATE_CACHES;
        memset(s_nor_read_buffer, 0, testSize);
        memcpy(s_nor_read_buffer, (void *)(EXAMPLE_XSPI_AMBA_BASE + EXAMPLE_SECTOR * SECTOR_SIZE), testSize);

        if (memcmp(s_nor_read_buffer, s_nor_program_buffer, testSize) != 0)
        {
            PRINTF("  AHB read verify FAIL!\r\n");
        }
        else
        {
            PRINTF("  AHB read verify: PASS\r\n");
        }
    }

    PRINTF("\r\nXSPI Quad I/O polling example finished.\r\n");
    PRINTF("Flash reset to default SPI mode.\r\n");
    xspi_quad_reset_and_halt(EXAMPLE_XSPI);

    return 0;
}
