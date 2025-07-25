/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "fsl_common.h"
#include "pin_mux.h"
#include "board.h"
#include "fsl_cache.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU(); // This API enables the CACHE
    L1CACHE_DisableDCache();
    BOARD_InitBootPins();
    BOARD_InitBootClocks();

    BOARD_InitDebugConsole();

    /* Compared to PSA-based MbedTLS3.x examples, we do not need to format our
     * key catalogs, as the mbedtls3x_selftest is running purely on software.
     */
}
/*${function:end}*/
