/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "fsl_common.h"
#include "pin_mux.h"
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
    BOARD_ConfigMPU(); // This API enables the CACHE
    L1CACHE_DisableDCache();
    BOARD_InitBootPins();
    BOARD_InitBootClocks();

    BOARD_InitDebugConsole();
    /* This example requires the user to format the key catalogs and initialize
     * the key management allocator.
     */
    FormatKeyCatalogs(nvmKeyCatalog, ramKeyCatalog);
    HKF_Init(nvmKeyCatalog, ramKeyCatalog);
}
/*${function:end}*/
