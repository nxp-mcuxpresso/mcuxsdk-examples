/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include <stdbool.h>
#include "clock_config.h"
#include "board.h"
#include "app.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    CLOCK_SetIpSrc(kCLOCK_Lpi2c0, kCLOCK_Pcc1BusIpSrcSysOscDiv2);

    CLOCK_SetIpSrc(kCLOCK_Lpi2c1, kCLOCK_Pcc1BusIpSrcSysOscDiv2);

    /* Set IRQ priority for freertos_lpi2c */
    NVIC_SetPriority(EXAMPLE_I2C_MASTER_IRQN, 3);
    NVIC_SetPriority(EXAMPLE_I2C_SLAVE_IRQN, 2);
}
/*${function:end}*/
