/*
 * Copyright 2018 - 2020,2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "fsl_flash.h"
#include "fsl_dsc_fmu_flash.h"
#include "app.h"

#include <ctype.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define BUFFER_LEN FLASH_FEATURE_PHRASE_SIZE_IN_WORD
#define HEXDUMPLEN 64

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

void error_trap(void);
void app_finalize(void);
void hexdump_dsc(uint32_t addr, size_t size);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*! @brief Flash driver Structure */
static flash_config_t s_flashDriver;
/*! @brief Buffer for program */
static uint32_t s_buffer[BUFFER_LEN];
/*! @brief Buffer for readback */
static uint32_t s_buffer_rbc[BUFFER_LEN];

/*******************************************************************************
 * Code
 ******************************************************************************/

typedef  uint8_t * __attribute__((far)) uint8farp_t;


void hexdump_dsc(uint32_t addr, size_t size)
{
    const uint8farp_t src8 = (uint8farp_t)addr;
    const int CNT = 16;

    for (size_t i = 0; i < size; i++)
    {
        int n = i % CNT;
        if (n == 0)
            PRINTF("%08lx  ", (uint32_t)addr+i);
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

/*
 * @brief Gets called when an error occurs.
 *
 * @details Print error message and trap forever.
 */
void error_trap(void)
{
    PRINTF("\r\n\r\n\r\n\t---- HALTED DUE TO FLASH ERROR! ----");
    while (1)
    {
    }
}

/*
 * @brief Gets called when the app is complete.
 *
 * @details Print finshed message and trap forever.
 */
void app_finalize(void)
{
    /* Print finished message. */
    PRINTF("\r\nEnd of PFlash Example \r\n");
    while (1)
    {
    }
}

/*!
 * @brief Use Standard Software Drivers (SSD) to modify pflash.
 *
 * @details This function uses SSD to demonstrate flash mode:
 *            + Check pflash information.
 *            + Erase a sector and verify.
 *            + Program a sector and verify.
 */
int main(void)
{
    status_t result;    /* Return code from each flash driver function */
    uint32_t destAddr; /* Address of the target location */

    uint32_t pflashBlock0Base = 0;
    uint32_t pflashBlock1Base = 0;
    uint32_t pflashBlock0Size = 0;
    uint32_t pflashSectorSize = 0;


    /* Init hardware */
    BOARD_InitHardware();

    PRINTF("MCUX SDK version: %s\r\n", MCUXSDK_VERSION_FULL_STR);

    /* Clean up Flash, Cache driver Structure*/
    memset(&s_flashDriver, 0, sizeof(flash_config_t));

    /* Setup flash driver structure for device and initialize variables. */
    result = FLASH_Init(&s_flashDriver);
    if (kStatus_FLASH_Success != result)
    {
        error_trap();
    }

    /* Get flash properties*/
    FLASH_GetProperty(&s_flashDriver, kFLASH_PropertyPflash0BlockBaseAddr, &pflashBlock0Base);
    FLASH_GetProperty(&s_flashDriver, kFLASH_PropertyPflash1BlockBaseAddr, &pflashBlock1Base);
    FLASH_GetProperty(&s_flashDriver, kFLASH_PropertyPflash0BlockSize, &pflashBlock0Size);
    FLASH_GetProperty(&s_flashDriver, kFLASH_PropertyPflash0SectorSize, &pflashSectorSize);

    /* print welcome message */
    PRINTF("\r\nPFlash Example Start \r\n");
    /* Print flash information - PFlash. */
    PRINTF("\r\nFlash Information for Flash 0 Array: ");
    PRINTF("\r\n Program Flash block bass address:\t%lu, Hex: (0x%lx)", (pflashBlock0Base / 1024), pflashBlock0Base);
    PRINTF("\r\n Program Flash block Size:\t\t%lu KB, Hex: (0x%lx)", (pflashBlock0Size / 1024), pflashBlock0Size);
    PRINTF("\r\n Program Flash block Sector Size:\t%lu KB, Hex: (0x%lx)", (pflashSectorSize / 1024), pflashSectorSize);
    PRINTF("\r\nFlash Information for Flash 1 Array: ");
    PRINTF("\r\n Program Flash block bass address:\t%lu, Hex: (0x%lx)\r\n", (pflashBlock1Base / 1024), pflashBlock1Base);


/* In case of the protected sectors at the end of the pFlash just select
the block from the end of pFlash to be used for operations
SECTOR_INDEX_FROM_END = 1 means the last sector,
SECTOR_INDEX_FROM_END = 2 means (the last sector - 1) ...
*/
#ifndef SECTOR_INDEX_FROM_END
#define SECTOR_INDEX_FROM_END 1U
#endif

    /* Erase a sector from destAdrss. */
    destAddr = pflashBlock1Base + (pflashBlock0Size - (SECTOR_INDEX_FROM_END * pflashSectorSize));

    PRINTF("\r\nErase a sector of flash at address %lx\r\n\r\n", destAddr);
    PRINTF("Sector content:\r\n");

    hexdump_dsc(destAddr, HEXDUMPLEN);

    PRINTF("Verify the sector is already erased: ");

    result = FLASH_VerifyEraseSector(&s_flashDriver, FLASH, destAddr, pflashSectorSize);
    PRINTF("%s\r\n", result == kStatus_FLASH_Success ? "ERASED" : "NOT_ERASED");

    if (result != kStatus_FLASH_Success)
    {
        PRINTF("Erasing the sector...\r\n");

        result = FLASH_Erase(&s_flashDriver, FLASH, destAddr, pflashSectorSize, kFLASH_ApiEraseKey);
        if (kStatus_FLASH_Success != result)
        {
            error_trap();
        }

        PRINTF("Sector content after erase:\r\n");
        hexdump_dsc(destAddr, HEXDUMPLEN);


        PRINTF("Verify the sector has been erased: ");
        result = FLASH_VerifyEraseSector(&s_flashDriver, FLASH, destAddr, pflashSectorSize);
        PRINTF("%s\r\n", result == kStatus_FLASH_Success ? "ERASED" : "NOT_ERASED");

        if (kStatus_FLASH_Success != result)
        {
            error_trap();
        }

        PRINTF("Successfully erased sector 0x%lx -> 0x%lx\r\n", destAddr, (destAddr + pflashSectorSize));
    }

    PRINTF("\r\nProgram a buffer to a phrase of flash\r\n\r\n");

    /* Prepare user buffer. */
    for (int i = 0; i < BUFFER_LEN; i++)
    {
        s_buffer[i] = i;
    }

    PRINTF("Buffer content:\r\n");
    hexdump_dsc((uint32_t)(uint8farp_t)s_buffer, sizeof(s_buffer));

    /* Program user buffer into flash */
    result = FLASH_Program(&s_flashDriver, FLASH, destAddr, s_buffer, sizeof(s_buffer));
    if (kStatus_FLASH_Success != result)
    {
        error_trap();
    }

    PRINTF("Flash content after programming:\r\n");
    hexdump_dsc(destAddr, HEXDUMPLEN);


    result = memcmp(s_buffer, (uint8farp_t)destAddr, sizeof(s_buffer));

    if (result != 0)
    {
        PRINTF("Failed to verify programmed data!\r\n");
        error_trap();
    }

    PRINTF("Successfully Programmed and Verified Location 0x%lx -> 0x%lx \r\n\r\n", destAddr,
           (destAddr + sizeof(s_buffer)));

    PRINTF("Test erase verify:\r\n");
    PRINTF(" Programmed SECTOR:");
    result = FLASH_VerifyEraseSector(&s_flashDriver, FLASH, destAddr, pflashSectorSize);
    PRINTF("\t%s\r\n", result == kStatus_FLASH_Success ? "ERASED" : "NOT_ERASED");

    if (result == kStatus_FLASH_Success)
    {
        error_trap();
    }

    PRINTF(" Programmed PAGE:");
    result = FLASH_VerifyErasePage(&s_flashDriver, FLASH, destAddr, FLASH_FEATURE_PAGE_SIZE);
    PRINTF("\t%s\r\n", result == kStatus_FLASH_Success ? "ERASED" : "NOT_ERASED");

    if (result == kStatus_FLASH_Success)
    {
        error_trap();
    }

    PRINTF(" Programmed PHRASE:");
    result = FLASH_VerifyErasePhrase(&s_flashDriver, FLASH, destAddr, FLASH_FEATURE_PHRASE_SIZE);
    PRINTF("\t%s\r\n", result == kStatus_FLASH_Success ? "ERASED" : "NOT_ERASED");

    if (result == kStatus_FLASH_Success)
    {
        error_trap();
    }

    PRINTF(" Following PAGE:");
    result = FLASH_VerifyErasePage(&s_flashDriver, FLASH, destAddr+FLASH_FEATURE_PAGE_SIZE, FLASH_FEATURE_PAGE_SIZE);
    PRINTF("\t%s\r\n", result == kStatus_FLASH_Success ? "ERASED" : "NOT_ERASED");

    if (result != kStatus_FLASH_Success)
    {
        error_trap();
    }

    PRINTF(" Following PHRASE:");
    result = FLASH_VerifyErasePhrase(&s_flashDriver, FLASH, destAddr+FLASH_FEATURE_PHRASE_SIZE, FLASH_FEATURE_PHRASE_SIZE);
    PRINTF("\t%s\r\n", result == kStatus_FLASH_Success ? "ERASED" : "NOT_ERASED");

    if (result != kStatus_FLASH_Success)
    {
        error_trap();
    }

    app_finalize();

    return 0;
}
