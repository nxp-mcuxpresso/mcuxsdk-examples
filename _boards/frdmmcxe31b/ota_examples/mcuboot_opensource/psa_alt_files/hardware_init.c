/*
 * Copyright 2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_cache.h"
#include "hse_default_config.h"
#include "hse_interface.h"
#include "hse_host_format_key_catalogs.h"
#include "hse_keys_allocator.h"
/*${header:end}*/

const hseKeyGroupCfgEntry_t nvmKeyCatalog[] = {HSE_NVM_KEY_CATALOG_CFG};
const hseKeyGroupCfgEntry_t ramKeyCatalog[] = {HSE_RAM_KEY_CATALOG_CFG};


/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();

    L1CACHE_DisableDCache();

    /* Bootloader uses default clock setup

       It was observed that calling BOARD_InitBootClocks() in the bootloader and then again
       in the app causes a reboot. This is because it creates an incompatible clock configuration sequence.

     */

    BOARD_InitDebugConsole();

    /* crypto-backend init calls */
    FormatKeyCatalogs(nvmKeyCatalog, ramKeyCatalog);
    HKF_Init(nvmKeyCatalog, ramKeyCatalog);
}
/*${function:end}*/
