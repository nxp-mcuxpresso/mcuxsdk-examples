/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "fsl_gpio.h"
#include "fsl_iocon.h"
#include "clock_config.h"
#include "board.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
}

void SystemInit(void)
{
    extern void *__Vectors;
    SCB->VTOR = (uint32_t)&__Vectors;

    BOARD_InitHardware();
}
/*${function:end}*/
