/*
 * Copyright 2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "clock_config.h"
#include "pin_mux.h"
#include "board.h"
#include "fsl_xbar.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
}

void BOARD_ConfigTriggerSource(void)
{
    /* Configure SIM for ADC trigger clock source selection */
    CLOCK_SetAdcTriggerClock(1U);

    /* Init xbar module. */
    XBAR_Init(XBAR);
    /* Configure the XBAR signal connections. */
    XBAR_SetSignalsConnection(XBAR, kXBAR_InputLptmr0Output, kXBAR_OutputAdcTrgA);
}
/*${function:end}*/
