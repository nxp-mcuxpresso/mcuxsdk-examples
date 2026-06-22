/*
 * Copyright 2017,2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
#include "fsl_flash.h"
#include "fsl_flash_cmsis.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define BUFFER_LEN 4

#define Driver_FLASH ARM_Driver_Flash_(0) /* First instance maps to the main flash */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

void error_trap(void);
void app_finalize(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*! @brief Buffer for program */
static uint32_t s_buffer[BUFFER_LEN];
/*! @brief Buffer for readback */
static uint32_t s_buffer_rbc[BUFFER_LEN];

/*******************************************************************************
 * Code
 ******************************************************************************/

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
    PRINTF("\r\n End of PFlash CMSIS Example \r\n");
    while (1)
    {
    }
}


int main(void)
{
    int32_t result;     /* Return code from each flash driver function */
    uint32_t destAddr; /* Address of the target location */

    ARM_FLASH_INFO *armFlashInfo;
    uint32_t pflashBaseAdrr = FSL_FEATURE_FLASH_PFLASH_START_ADDRESS;
    uint32_t pflashTotalSize  = 0;
    uint32_t pflashSectorSize = 0;

    /* Init hardware */
    BOARD_InitHardware();

    /* Setup flash driver structure for device and initialize variables. */
    result = Driver_FLASH.Initialize(0);
    if (ARM_DRIVER_OK != result)
    {
        error_trap();
    }

    armFlashInfo     = Driver_FLASH.GetInfo();
    pflashTotalSize  = armFlashInfo->sector_count * armFlashInfo->sector_size;
    pflashSectorSize = armFlashInfo->sector_size;

    /* print welcome message */
    PRINTF("\r\n PFlash CMSIS Example Start \r\n");
    /* Print flash information - PFlash. */
    PRINTF("\r\n PFlash Information: ");
    PRINTF("\r\n Total Program Flash Size:\t%d KB, Hex: (0x%x)", (pflashTotalSize / 1024), pflashTotalSize);
    PRINTF("\r\n Program Flash Sector Size:\t%d KB, Hex: (0x%x) ", (pflashSectorSize / 1024), pflashSectorSize);

    /* Debug message for user. */
    /* Erase several sectors on upper pflash block where there is no code */
    PRINTF("\r\n\r\n Erase a sector of flash");


#ifndef SECTOR_INDEX_FROM_END
#define SECTOR_INDEX_FROM_END 1U
#endif

    destAddr =  pflashTotalSize - (SECTOR_INDEX_FROM_END * pflashSectorSize);

    result = Driver_FLASH.EraseSector(destAddr);
    if (ARM_DRIVER_OK != result)
    {
        error_trap();
    }

    /* Verify erase by reading back */
    for (uint32_t i = 0; i < BUFFER_LEN; i++)
    {
        if (0xffffffff != *(volatile uint32_t *)(pflashBaseAdrr + destAddr + i * 4))
        {
            error_trap();
        }
    }

    PRINTF("\r\n Successfully Erased Sector 0x%x -> 0x%x\r\n", destAddr, (destAddr + pflashSectorSize));

    PRINTF("\r\n Program a buffer to a sector of flash ");

    /* Prepare user buffer. */
    for (uint32_t i = 0; i < BUFFER_LEN; i++)
    {
        s_buffer[i] = i;
    }

    /* Program user buffer into flash */
    result = Driver_FLASH.ProgramData(destAddr, s_buffer, sizeof(s_buffer));
    if (sizeof(s_buffer) != result)
    {
        error_trap();
    }

#if defined(__DCACHE_PRESENT) && __DCACHE_PRESENT
    /* Clean the D-Cache before reading the flash data*/
    SCB_CleanInvalidateDCache();
#endif

    /* Verify programming by reading back from flash directly*/
    for (uint32_t i = 0; i < BUFFER_LEN; i++)
    {
        s_buffer_rbc[i] = *(volatile uint32_t *)(pflashBaseAdrr + destAddr + i * 4);
        if (s_buffer_rbc[i] != s_buffer[i])
        {
            error_trap();
        }
    }

    PRINTF("\r\n Successfully Programmed and Verified location 0x%x -> 0x%x \r\n", destAddr,
           (destAddr + sizeof(s_buffer)));

    app_finalize();

    return 0;
}
