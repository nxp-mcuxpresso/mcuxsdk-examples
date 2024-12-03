/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016 - 2017 , NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "ble_general.h"
#include "app.h"
#include "app_srtm.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    CLOCK_EnableClock(kCLOCK_PctlA);
    CLOCK_EnableClock(kCLOCK_PctlB);
    CLOCK_EnableClock(kCLOCK_Rgpio2p0);

    BOARD_InitPins();
    BOARD_BootClockRUN();
    APP_SRTM_I2C_ReleaseBus();
    BOARD_I2C_ConfigurePins();
    BOARD_InitDebugConsole();

    CLOCK_SetIpSrc(kCLOCK_Lpi2c3, kCLOCK_IpSrcSircAsync);
    CLOCK_SetIpSrc(kCLOCK_Lpi2c0, kCLOCK_IpSrcSystem);

    /* Use SIRC async clock to avoid baudrate impact from CPU freq change */
    CLOCK_SetIpSrc(kCLOCK_Lpuart2, kCLOCK_IpSrcSircAsync);

    /* Use AUXPLL main clock source */
    CLOCK_SetIpSrcDiv(kCLOCK_Sai0, kCLOCK_IpSrcRtcAuxPllAsync, 0, 0);
}
/*${function:end}*/
