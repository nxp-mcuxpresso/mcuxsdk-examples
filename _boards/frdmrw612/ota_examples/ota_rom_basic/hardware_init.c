/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
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
    CLOCK_EnableClock(kCLOCK_Otp);
    CLOCK_EnableClock(kCLOCK_Els);
    CLOCK_EnableClock(kCLOCK_ElsApb);
    RESET_PeripheralReset(kOTP_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kELS_APB_RST_SHIFT_RSTn);
}

/*${function:end}*/
