/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "fsl_clock.h"
#include "board.h"
#include <stdbool.h>
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /* Disable Cache for Flash operations*/

    /* set command to invalidate all ways and write GO bit to initiate command */
    LMEM->PCCCR |= LMEM_PCCCR_INVW1_MASK | LMEM_PCCCR_INVW0_MASK;
    LMEM->PCCCR |= LMEM_PCCCR_GO_MASK;
    /* Wait until the command completes */
    while (LMEM->PCCCR & LMEM_PCCCR_GO_MASK) {
    }

    /* Disable cache */
    LMEM->PCCCR &= ~LMEM_PCCCR_ENCACHE_MASK;
    __ISB();


    /* disable flash prefetch buffer for code flash */
    MSCM->OCMDR[0] |= 0x3 << MSCM_OCMDR_OCM1_SHIFT;

}
/*${function:end}*/