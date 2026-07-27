/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "app.h"
#include "pin_mux.h"
#include "fsl_clock.h"
#include "board.h"
#include "display_support.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_CommonSetting();

#if (DEMO_PANEL == DEMO_PANEL_LCM_RGB_5INCH)
    BOARD_InitDcifDpiPins();
#elif (DEMO_PANEL == DEMO_PANEL_LCD_PAR_S035)
    BOARD_InitDcifDbiPins();
#elif (DEMO_PANEL == DEMO_PANEL_RK055MHD091A0)
    BOARD_InitMIPIPanelPins();
#endif
}
/*${function:end}*/