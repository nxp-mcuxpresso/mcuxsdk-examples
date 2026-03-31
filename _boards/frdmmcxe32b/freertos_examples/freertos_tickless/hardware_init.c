/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include <stdbool.h>
#include "FreeRTOS.h"
#include "fsl_power.h"
#if configUSE_TICKLESS_IDLE == 2
#include "app.h"
#include "tickless_api.h"
#endif
/*${header:end}*/

/*${variable:start}*/

/*${variable:end}*/
/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_InitEXTIRQPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

}
/*${function:end}*/
