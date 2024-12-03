/*
 * Copyright 2020-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_flash.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define BUFFER_LEN 4U

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
/*! @brief Flash cache driver Structure */
// static ftfx_cache_config_t s_cacheDriver;
/*! @brief Buffer for program */
static uint32_t s_buffer[BUFFER_LEN];

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
    (void)PRINTF("\r\n\r\n\r\n\t---- HALTED DUE TO FLASH ERROR! ----");
    while (true)
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
    (void)PRINTF("\r\n End of PFlash Example \r\n");
    while (true)
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
    flash_security_state_t securityStatus = kFLASH_SecurityStateNotSecure; /* Return protection status */
    status_t result;      /* Return code from each flash driver function */
    uint32_t destAddress; /* Address of the target location */
    uint32_t i, failAddress, failDat;

    uint32_t pflashBlockBase  = 0;
    uint32_t pflashTotalSize  = 0;
    uint32_t pflashSectorSize = 0;

    /* Init hardware */
    BOARD_InitHardware();

    /* Clean up Flash, Cache driver Structure*/
    (void)memset(&s_flashDriver, 0, sizeof(flash_config_t));

    /* Setup flash driver structure for device and initialize variables. */
    result = FLASH_Init(&s_flashDriver);
    if (kStatus_FLASH_Success != result)
    {
        error_trap();
    }

    /* Get flash properties*/
    (void)FLASH_GetProperty(&s_flashDriver, kFLASH_PropertyPflashBlockBaseAddr, &pflashBlockBase);
    (void)FLASH_GetProperty(&s_flashDriver, kFLASH_PropertyPflashTotalSize, &pflashTotalSize);
    (void)FLASH_GetProperty(&s_flashDriver, kFLASH_PropertyPflashSectorSize, &pflashSectorSize);

    /* print welcome message */
    (void)PRINTF("\r\n PFlash Example Start \r\n");
    /* Print flash information - PFlash. */
    (void)PRINTF("\r\n PFlash Information: ");
    (void)PRINTF("\r\n Total Program Flash Size:\t%ld KB, Hex: (0x%lx)", (pflashTotalSize / 1024U), pflashTotalSize);
    (void)PRINTF("\r\n Program Flash Sector Size:\t%ld KB, Hex: (0x%lx) ", (pflashSectorSize / 1024U),
                 pflashSectorSize);

    /* Check security status. */
    result = FLASH_GetSecurityState(&s_flashDriver, &securityStatus);
    if (kStatus_FLASH_Success != result)
    {
        error_trap();
    }
    /* Print security status. */
    switch (securityStatus)
    {
        case kFLASH_SecurityStateNotSecure:
            (void)PRINTF("\r\n Flash is UNSECURE!");
            break;
        case kFLASH_SecurityStateBackdoorEnabled:
            (void)PRINTF("\r\n Flash is SECURE, BACKDOOR is ENABLED!");
            break;
        case kFLASH_SecurityStateBackdoorDisabled:
            (void)PRINTF("\r\n Flash is SECURE, BACKDOOR is DISABLED!");
            break;
        default:
            /* Do nothing */
            break;
    }
    (void)PRINTF("\r\n");

    /* Test pflash basic opeation only if flash is unsecure. */
    if (kFLASH_SecurityStateNotSecure == securityStatus)
    {
        /* Debug message for user. */
        /* Erase several sectors on upper pflash block where there is no code */
        (void)PRINTF("\r\n Erase a sector of flash");

/* In case of the protected sectors at the end of the pFlash just select
the block from the end of pFlash to be used for operations
SECTOR_INDEX_FROM_END = 1 means the last sector,
SECTOR_INDEX_FROM_END = 2 means (the last sector - 1) ...
in case of FSL_FEATURE_FLASH_HAS_PFLASH_BLOCK_SWAP it is
SECTOR_INDEX_FROM_END = 1 means the last 2 sectors with width of 2 sectors,
SECTOR_INDEX_FROM_END = 2 means the last 4 sectors back
with width of 2 sectors ...
*/
#ifndef SECTOR_INDEX_FROM_END
#define SECTOR_INDEX_FROM_END 1U
#endif
        destAddress = pflashBlockBase + (pflashTotalSize - (SECTOR_INDEX_FROM_END * pflashSectorSize));

        result = FLASH_Erase(&s_flashDriver, destAddress, pflashSectorSize, kFLASH_ApiEraseKey);
        if (kStatus_FLASH_Success != result)
        {
            error_trap();
        }

        /* Verify sector if it's been erased. */
        result = FLASH_VerifyErase(&s_flashDriver, destAddress, pflashSectorSize, kFLASH_MarginValueUser);
        if (kStatus_FLASH_Success != result)
        {
            error_trap();
        }

        /* Print message for user. */
        (void)PRINTF("\r\n Successfully Erased Sector 0x%lx -> 0x%lx\r\n", destAddress,
                     (destAddress + pflashSectorSize));

        /* Print message for user. */
        (void)PRINTF("\r\n Program a buffer to a sector of flash ");
        /* Prepare user buffer. */
        for (i = 0; i < BUFFER_LEN; i++)
        {
            s_buffer[i] = i;
        }

        /* Program user buffer into flash*/
        result = FLASH_Program(&s_flashDriver, destAddress, (uint8_t *)s_buffer, sizeof(s_buffer));
        if (kStatus_FLASH_Success != result)
        {
            error_trap();
        }

        /* Verify programming by Program Check command with user margin levels */
        result = FLASH_VerifyProgram(&s_flashDriver, destAddress, sizeof(s_buffer), (const uint8_t *)s_buffer,
                                     kFLASH_MarginValueUser, &failAddress, &failDat);
        if (kStatus_FLASH_Success != result)
        {
            error_trap();
        }

        (void)PRINTF("\r\n Successfully Programmed and Verified Location 0x%lx -> 0x%lx \r\n", destAddress,
                     (destAddress + sizeof(s_buffer)));

        /* Erase the context we have progeammed before*/
        /* Note: we should make sure that the sector which will be set as swap indicator should be blank*/
        result = FLASH_Erase(&s_flashDriver, destAddress, pflashSectorSize, kFLASH_ApiEraseKey);
        if (kStatus_FLASH_Success != result)
        {
            error_trap();
        }
    }
    else
    {
        (void)PRINTF("\r\n Erase/Program opeation will not be executed, as Flash is SECURE!");
    }

    app_finalize();

    return 0;
}
