/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "app.h"
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "mflash_drv.h"
#include "fsl_c40_flash.h"
#include "flash_partitioning.h"
#include "boot.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Types
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */

int main(void)
{
    BOARD_InitHardware();

    /* Unlock application slots for flash erase/write.
     * On MCXE31B sectors within first 768kB block have super-sector
     * granularity of 64kB.
     */

    for (int i=0; i < BOOT_FLASH_ACT_SIZE/MFLASH_SUPER_SECTOR_SIZE; i++)
    {
        FLASH_SetSectorProtection(NULL, BOOT_FLASH_ACT_APP + i*MFLASH_SUPER_SECTOR_SIZE, false);
        FLASH_SetSectorProtection(NULL, BOOT_FLASH_CAND_APP + i*MFLASH_SUPER_SECTOR_SIZE, false);
    }

    PRINTF("hello sbl.\r\n");

    PRINTF("DES %x; FES %x\n", (*(volatile uint32_t *)(0x4028c000)), (*(volatile uint32_t *)(0x4028c008)));

    (void)sbl_boot_main();

    return 0;
}

void SBL_DisablePeripherals(void)
{
    DbgConsole_Deinit();
    SCB_DisableICache();

}

status_t CRYPTO_InitHardware(void)
{
    return kStatus_Success;
}

