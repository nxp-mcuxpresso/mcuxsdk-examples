/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"
#include "pin_mux.h"
#include "app.h"

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
