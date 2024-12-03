/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "board.h"
#include <stdbool.h>
#include "fsl_clock.h"
#include "fsl_reset.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* attach 12 MHz clock to FLEXCOMM0 (debug console) */
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    /* enable clock to OTP and reset it */
    CLOCK_EnableClock(kCLOCK_Otp);
    RESET_PeripheralReset(kOTP_RST_SHIFT_RSTn);

    BOARD_InitBootPins();

    /* main clock needs to be set to 12 MHz in order to use OTP */
    BOARD_BootClockFRO12M();

    BOARD_InitDebugConsole();
}
/*${function:end}*/
