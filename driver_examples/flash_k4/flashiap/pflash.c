/*
 * Copyright 2018 - 2020 NXP
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
#include "app.h"
#include "fsl_flashiap.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define BUFFER_LEN 4

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

void error_trap(void);
void app_finalize(void);

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
    PRINTF("\r\n End of PFlash Example \r\n");
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
    uint32_t destAdrss; /* Address of the target location */

    uint32_t pflash0BlockBase  = 0;
    uint32_t pflash0BlockSize  = 0;
    uint32_t pflash0SectorSize = 0;
    uint32_t pflash0BlockCount = 0;
    uint32_t pflash0TotalSize  = 0;
    /* Init hardware */
    BOARD_InitHardware();

    /* Clean up Flash, Cache driver Structure*/
    memset(&s_flashDriver, 0, sizeof(flash_config_t));

    /* Setup flash driver structure for device and initialize variables. */
    result = FLASH_Init(&s_flashDriver);
    if (kStatus_FLASH_Success != result)
    {
        error_trap();
    }

#if defined(SMSCM_CACHE_CLEAR_MASK) && SMSCM_CACHE_CLEAR_MASK
    /* disable flash cache/Prefetch */
    FLASH_CACHE_Disable();
#endif /* SMSCM_CACHE_CLEAR_MASK */

    /* Get flash properties*/
    FLASH_GetProperty(&s_flashDriver, kFLASH_PropertyPflash0BlockBaseAddr, &pflash0BlockBase);
    FLASH_GetProperty(&s_flashDriver, kFLASH_PropertyPflash0BlockSize, &pflash0BlockSize);
    FLASH_GetProperty(&s_flashDriver, kFLASH_PropertyPflash0SectorSize, &pflash0SectorSize);
    FLASH_GetProperty(&s_flashDriver, kFLASH_PropertyPflash0BlockCount, &pflash0BlockCount);
    FLASH_GetProperty(&s_flashDriver, kFLASH_PropertyPflash0TotalSize, &pflash0TotalSize);

    /* print welcome message */
    PRINTF("\r\n PFlash Example Start\r\n");
    /* Print flash information - PFlash. */
    PRINTF("\r\n PFlash Information:");
    PRINTF("\r\n Program Flash block bass address:\t%d KB, Hex: (0x%x)", (pflash0BlockBase / 1024), pflash0BlockBase);
    PRINTF("\r\n Program Flash block Size:\t\t%d KB, Hex: (0x%x)", (pflash0BlockSize / 1024), pflash0BlockSize);
    PRINTF("\r\n Program Flash block Sector Size:\t%d KB, Hex: (0x%x)", (pflash0SectorSize / 1024), pflash0SectorSize);
    PRINTF("\r\n Program Flash block count:\t%d", pflash0BlockCount);
    PRINTF("\r\n Total Program Flash Size:\t\t%d KB, Hex: (0x%x)\r\n", (pflash0TotalSize / 1024), pflash0TotalSize);

    /* Debug message for user. */
    /* Erase several sectors on upper pflash block where there is no code */
    PRINTF("\r\n Erase a sector of flash");
/* In case of the protected sectors at the end of the pFlash just select
the block from the end of pFlash to be used for operations
SECTOR_INDEX_FROM_END = 1 means the last sector,
SECTOR_INDEX_FROM_END = 2 means (the last sector - 1) ...
*/
#ifndef SECTOR_INDEX_FROM_END
#define SECTOR_INDEX_FROM_END 1U
#endif
    /* Erase a sector from destAdrss. */
    destAdrss = pflash0BlockBase + (pflash0BlockSize - (SECTOR_INDEX_FROM_END * pflash0SectorSize));
    result    = FLASH_Erase(&s_flashDriver, destAdrss, pflash0SectorSize, kFLASH_ApiEraseKey);
    if (kStatus_FLASH_Success != result)
    {
        error_trap();
    }

    /* Verify sector if it's been erased. */
    result = FLASH_VerifyEraseSector(&s_flashDriver, destAdrss, pflash0SectorSize);
    if (kStatus_FLASH_Success != result)
    {
        error_trap();
    }
    /* Print message for user. */
    PRINTF("\r\n Successfully Erased Sector 0x%x -> 0x%x\r\n", destAdrss, (destAdrss + pflash0SectorSize));

    /* Print message for user. */
    PRINTF("\r\n Program a buffer to a phrase of flash");
    /* Prepare user buffer. */
    for (uint32_t i = 0; i < BUFFER_LEN; i++)
    {
        s_buffer[i] = i;
    }

    /* Program user buffer into flash*/
    result = FLASH_Program(&s_flashDriver, destAdrss, (const uint32_t *)s_buffer, sizeof(s_buffer));
    if (kStatus_FLASH_Success != result)
    {
        error_trap();
    }

    /* Verify programming by reading back from flash directly*/
    for (uint32_t i = 0; i < BUFFER_LEN; i++)
    {
        s_buffer_rbc[i] = *(volatile uint32_t *)(destAdrss + i * 4);
        if (s_buffer_rbc[i] != s_buffer[i])
        {
            error_trap();
        }
    }

    PRINTF("\r\n Successfully Programmed and Verified Location 0x%x -> 0x%x \r\n", destAdrss,
           (destAdrss + sizeof(s_buffer)));

    app_finalize();

    return 0;
}
