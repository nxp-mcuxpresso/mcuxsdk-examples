/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "app.h"
#include "clock_config.h"
#include "board.h"
/*${header:end}*/

/*${variable:start}*/
/*${variable:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    SystemPlatformInit();
    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    BOARD_ConfigMPU();
}

void APP_InitDomainConfig(void)
{
}

void APP_DeinitDomainConfig(void)
{
}
/*${function:end}*/
