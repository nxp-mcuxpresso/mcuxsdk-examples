/*
 * Copyright 2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
    BOARD_ConfigTriggerSource();
}

void BOARD_ConfigTriggerSource(void)
{
    /* Configure PDB to trigger ADC directly. */
    SIM->MISC_CTL |= SIM_MISC_CTL_PDBADCTRG_MASK;
}
/*${function:end}*/
