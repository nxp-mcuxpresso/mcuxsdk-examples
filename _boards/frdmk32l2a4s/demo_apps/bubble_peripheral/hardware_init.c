/*
 * Copyright 2019-2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/* ${header:start} */
#include "app.h"
#include "pin_mux.h"
#include "peripherals.h"
#include "board.h"
#include "fsl_gpio.h"
#include "fsl_port.h"
/* ${header:end} */

/* ${function:start} */
void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_I2C_ConfigurePins();
    BOARD_InitDebugConsole();
    BOARD_InitPeripherals();
}
/*${function:end}*/
