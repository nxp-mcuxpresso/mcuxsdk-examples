/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "fsl_clock.h"
#include "fsl_reset.h"
#include "board.h"
#include <stdbool.h>
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
    
    /* Disable prefetch buffer and flash cache */
    
    SYSCON->NVM_CTRL |= SYSCON_NVM_CTRL_DIS_MBECC_ERR_INST_MASK | SYSCON_NVM_CTRL_DIS_MBECC_ERR_DATA_MASK;
    SYSCON->NVM_CTRL |= SYSCON_NVM_CTRL_DIS_FLASH_SPEC_MASK | SYSCON_NVM_CTRL_DIS_DATA_SPEC_MASK;
    SYSCON->LPCAC_CTRL |= SYSCON_LPCAC_CTRL_DIS_LPCAC_MASK;
}
/*${function:end}*/