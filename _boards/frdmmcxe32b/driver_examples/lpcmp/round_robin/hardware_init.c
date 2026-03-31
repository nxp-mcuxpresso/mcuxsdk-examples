/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "fsl_common.h"
#include "pin_mux.h"
#include "board.h"
#include "fsl_rtc.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitCMPPins();
    BOARD_InitBootClocks();

    SXOSC->SXOSC_CTRL |= SXOSC_SXOSC_CTRL_OSCON_MASK;

    RTC_EnableAPI(RTC);
    RTC_EnableRTC(RTC);
    RTC_EnableAnalogComparatorTrigger(RTC);
    RTC_SetAPIValue(RTC, 32768U);
      
    BOARD_InitDebugConsole();
}
/*${function:end}*/
