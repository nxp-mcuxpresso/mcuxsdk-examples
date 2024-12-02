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
#include "fsl_gpio.h"
#include "fsl_iomuxc.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_I2C_ConfigurePins();

    BOARD_InitDebugConsole();

    CLOCK_SetIpSrc(kCLOCK_Lpi2c3, kCLOCK_IpSrcSysOscAsync);
}
