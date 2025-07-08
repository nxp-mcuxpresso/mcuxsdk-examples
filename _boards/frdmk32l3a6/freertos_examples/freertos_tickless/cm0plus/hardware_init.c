/*
 * Copyright 2017-2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "fsl_device_registers.h"
#include "fsl_common.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "FreeRTOS.h"
#include "fsl_intmux.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
    INTMUX_Init(INTMUX1);
    /* Enable PORTA intmux source on INTMUX channel 0 */
    INTMUX_EnableInterrupt(INTMUX1, 0, PORTA_IRQn);
}
/*${function:end}*/
