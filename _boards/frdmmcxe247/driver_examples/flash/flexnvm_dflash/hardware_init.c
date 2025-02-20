/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
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
    
}
/*${function:end}*/
