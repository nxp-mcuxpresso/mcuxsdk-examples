/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
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
    /* Use 16 MHz clock for the FLEXCOMM2 */
    CLOCK_AttachClk(kSFRO_to_FLEXCOMM2);

    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
}

uint32_t I2C2_GetFreq(void)
{
    return CLOCK_GetFlexCommClkFreq(2U);
}

/*${function:end}*/
