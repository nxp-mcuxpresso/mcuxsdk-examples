/*
 * Copyright 2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "app.h"
#include "fsl_debug_console.h"
#include "eeprom_emulation.h"

#include <ctype.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define TEST(x) \
    if (x) \
    { \
        PRINTF("FAILED with %d\r\n", x); \
        error_trap(); \
    } \

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

void error_trap(void);
void app_finalize(void);

int eeprom_emu_flashapi_init(struct eeprom_emu_flashapi *api);

/*******************************************************************************
 * Variables
 ******************************************************************************/

uint8_t eeprom_buf[64];
uint8_t eeprom_buf_aux[sizeof(eeprom_buf)];

/*******************************************************************************
 * Code
 ******************************************************************************/

void error_trap(void)
{
    PRINTF("\r\n\r\n\r\n\t---- HALTED DUE TO ERROR! ----");
    while (1)
    {
    }
}

static void hexdump(const void *src, size_t size)
{
    const unsigned char *src8 = src;
    const int CNT             = 16;

    for (size_t i = 0; i < size; i++)
    {
        int n = i % CNT;
        if (n == 0)
            PRINTF("%08x  ", (uint32_t)src+i);
        PRINTF("%02X ", src8[i]);
        if ((i && n == CNT - 1) || (i + 1 == size))
        {
            int rem = CNT - 1 - n;
            for (int j = 0; j < rem; j++)
                PRINTF("   ");
            PRINTF("|");
            for (int j = n; j >= 0; j--)
                PUTCHAR(isprint(src8[i - j]) ? src8[i - j] : '.');
            PRINTF("|\n");
        }
    }
    PUTCHAR('\n');
}

int main(void)
{
    status_t result;
    uint32_t num_sectors;

    /* Init hardware */
    BOARD_InitHardware();

    PRINTF("\r\nMCUX SDK version: %s\r\n", MCUXSDK_VERSION_FULL_STR);

    PRINTF("\r\n-- EEPROM EMULATION EXAMPLE --\r\n\r\n");

    /* Configure EEPROM emulation*/

    struct eeprom_emu_dev eeprom_emu_dev =
    {
        .config.size = sizeof(eeprom_buf),
        .config.readonly = false,
        .config.page_size = FLASH_SECTOR_SIZE, /* in this case Page == Sector */
        .config.flash_offset = FLASH_BASE + FLASH_SIZE - 2*FLASH_SECTOR_SIZE,
        .config.flash_size = 2*FLASH_SECTOR_SIZE,
        .config.partitionerase = false,
        .config.flash_cbs = FLASH_WRITE_SIZE,
        .config.rambuf = eeprom_buf_aux,
    };

    num_sectors = eeprom_emu_dev.config.flash_size / eeprom_emu_dev.config.page_size;

    PRINTF("Using the following setup:\r\n");
    PRINTF("  EEPROM size:\t%u\r\n", eeprom_emu_dev.config.size);
    PRINTF("  FLASH offset:\t0x%x\r\n", eeprom_emu_dev.config.flash_offset);
    PRINTF("  FLASH sector size:\t%u\r\n", eeprom_emu_dev.config.page_size);
    PRINTF("  FLASH sectors used:\t%u\r\n", num_sectors);
    PRINTF("  Change Block size:\t%u\r\n", eeprom_emu_dev.config.flash_cbs);
    PRINTF("  RAM buffer used:\t%s\r\n", eeprom_emu_dev.config.rambuf ? "YES" : "NO");
    PRINTF("\r\n");

    if (eeprom_emu_dev.config.rambuf == NULL && num_sectors < 2)
    {
        /* When only a single sector is used it is necessary to use the auxilary RAM buffer.
           However, it is possible to lose data when reset occurs during page rollover */
        PRINTF("Single sector operation is only possible with auxilary RAM buffer\r\n");
        error_trap();
    }

    /* Initialize flash binding */
    result = eeprom_emu_flashapi_init(&eeprom_emu_dev.config.flashapi);
    if (result)
    {   PRINTF("eeprom_emu_flashapi_init FAILED with %d\r\n", result);
        error_trap();
    }

    PRINTF("Erasing flash area used for EEPROM emulation...\r\n\r\n");
    result = eeprom_emu_dev.config.flashapi.erase(eeprom_emu_dev.config.flash_offset, eeprom_emu_dev.config.flash_size);
    TEST(result);

    /* Initialize EEPROM emulation */
    result = eeprom_emu_init(&eeprom_emu_dev);
    if (result)
    {   PRINTF("eeprom_emu_init FAILED with %d\r\n", result);
        error_trap();
    }

    PRINTF("EEPROM readback of the erased FLASH area:\r\n");
    result = eeprom_emu_read(&eeprom_emu_dev, 0, eeprom_buf, sizeof(eeprom_buf));
    TEST(result);
    hexdump(eeprom_buf, sizeof(eeprom_buf));

    PRINTF("Initializing EEPROM content with the first write...\r\n\r\n");
    memset(eeprom_buf, 0, sizeof(eeprom_buf));
    result = eeprom_emu_write(&eeprom_emu_dev, 0, &eeprom_buf, sizeof(eeprom_buf));
    TEST(result);

    PRINTF("EEPROM readback of the initialized FLASH area:\r\n");
    result = eeprom_emu_read(&eeprom_emu_dev, 0, eeprom_buf, sizeof(eeprom_buf));
    TEST(result);
    hexdump(eeprom_buf, sizeof(eeprom_buf));

    char s[] = "SOME RANDOM EEPROM CONTENT";

    PRINTF("EEPROM write of length %u...\r\n", sizeof(s));
    result = eeprom_emu_write(&eeprom_emu_dev, 0, s, sizeof(s));
    TEST(result);

    PRINTF("EEPROM readback and verify:\r\n");
    result = eeprom_emu_read(&eeprom_emu_dev, 0, eeprom_buf, sizeof(eeprom_buf));
    TEST(result);
    hexdump(eeprom_buf, sizeof(eeprom_buf));

    if (memcmp(s, eeprom_buf, sizeof(s)) != 0)
    {
        PRINTF("FAILED to verify data\r\n");
        error_trap();
    }
    PRINTF("Verified OK\r\n\r\n");

    int num_writes_to_rollover = eeprom_emu_dev.config.page_size / eeprom_emu_dev.config.flash_cbs;

    PRINTF("Now going to test EEPROM rollover to next FLASH sector\r\n\r\n");
    PRINTF("Doing %u writes to the last EEPROM byte...\r\n", num_writes_to_rollover);

    for (int i=0; i < num_writes_to_rollover; i++)
    {
        uint8_t val = (uint8_t)i;
        result = eeprom_emu_write(&eeprom_emu_dev, sizeof(eeprom_buf)-1, &val, sizeof(val));
        if (result)
        {
            PRINTF("FAILED to write at index %d, return value %d\r\n", i, result);
            error_trap();
        }
    }

    PRINTF("EEPROM readback and verify of the original data:\r\n");
    result = eeprom_emu_read(&eeprom_emu_dev, 0, eeprom_buf, sizeof(eeprom_buf));
    TEST(result);
    hexdump(eeprom_buf, sizeof(eeprom_buf));

    if (memcmp(s, eeprom_buf, sizeof(s)) != 0)
    {
        PRINTF("FAILED to verify data\r\n");
        error_trap();
    }
    PRINTF("Verified OK\r\n\r\n");

    PRINTF("-- END OF EXAMPLE --\r\n");

    return 0;
}
