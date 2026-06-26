/*
 * Copyright 2025-2026 NXP
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
    BOARD_ConfigMPU();       // This API enables the DCache...
    L1CACHE_DisableDCache(); // ...but HSEB does not like that
    /* Disable flash prefetch; see the ERR052645 errata for more information */
    PFLASH->PFCR[0] &= ~(PFLASH_PFCR_P0_DPFEN_MASK);
    PFLASH->PFCR[1] &= ~(PFLASH_PFCR_P1_DPFEN_MASK);

    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
}
/*${function:end}*/
