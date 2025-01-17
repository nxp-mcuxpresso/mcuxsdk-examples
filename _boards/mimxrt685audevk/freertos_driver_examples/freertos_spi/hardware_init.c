/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "board.h"
#include <stdbool.h>
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* attach clock to SPI5 */
    CLOCK_AttachClk(kSFRO_to_FLEXCOMM5);

    /* attach clock to SPI4 */
    CLOCK_AttachClk(kSFRO_to_FLEXCOMM4);

    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
}
/*${function:end}*/
