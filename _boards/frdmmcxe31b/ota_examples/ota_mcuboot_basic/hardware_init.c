/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "fsl_clock.h"
#include "board.h"
#include "mflash_drv.h"
#include "flash_partitioning.h"
#include "fsl_c40_flash.h"
#include <stdbool.h>
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /* Enable erase/write in candidate slot */

    /* Either the size of the slot must be aligned to super sector size or it must also clear
       the protection on the last partly used sector. The current setup uses aligned slots */

    assert(BOOT_FLASH_ACT_SIZE % MFLASH_SUPER_SECTOR_SIZE == 0);

    for (int i=0; i < BOOT_FLASH_ACT_SIZE/MFLASH_SUPER_SECTOR_SIZE; i++)
    {
        FLASH_SetSectorProtection(NULL, BOOT_FLASH_CAND_APP + i*MFLASH_SUPER_SECTOR_SIZE, false);
    }
}
/*${function:end}*/