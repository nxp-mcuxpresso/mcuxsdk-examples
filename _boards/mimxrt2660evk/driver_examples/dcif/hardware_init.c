/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/

#include "app.h"
#include "pin_mux.h"
#include "fsl_clock.h"
#include "board.h"
#include <stdbool.h>
#include "fsl_debug_console.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* Board common setting: MPU, Power and Clock Tree, TRDC, and Debug Console init. */
    BOARD_CommonSetting();

#if (DEMO_INTERFACE_TYPE == DEMO_INTERFACE_DPI)
    BOARD_InitDcifDpiPins();
#elif (DEMO_INTERFACE_TYPE == DEMO_INTERFACE_DBI)
    BOARD_InitDcifDbiPins();
#endif
    BOARD_InitDcifPowerClockReset();
}
/*${function:end}*/
