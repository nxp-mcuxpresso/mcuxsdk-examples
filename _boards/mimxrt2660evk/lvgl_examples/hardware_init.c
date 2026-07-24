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
// #include "fsl_reset.h"
#include <stdbool.h>

/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* Board common setting: MPU, Power and Clock Tree, TRDC, and Debug Console init. */
    BOARD_CommonSetting();
#if (DEMO_PANEL == DEMO_PANEL_LCM_RGB_5INCH)
    BOARD_InitDcifDpiPins();
    BOARD_InitTouchPins();
#elif (DEMO_PANEL == DEMO_PANEL_LCD_PAR_S035)
    BOARD_InitDcifDbiPins();
#endif
}
/*${function:end}*/
