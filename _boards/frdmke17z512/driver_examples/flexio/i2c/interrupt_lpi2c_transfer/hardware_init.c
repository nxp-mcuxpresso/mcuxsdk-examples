/*
 * Copyright 2023 NXP
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
    BOARD_I2C_ConfigurePins();
    BOARD_InitDebugConsole();

    CLOCK_SetIpSrc(SLAVE_LPI2C_CLOCK_NAME, SLAVE_LPI2C_CLOCK_SOURCE);
    CLOCK_SetIpSrc(MASTER_FLEXIO_CLOCK_NAME, MASTER_FLEXIO_CLOCK_SOURCE);
}
/*${function:end}*/
