/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"

#include "board.h"
#include "cmsis.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
  BOARD_InitDebugConsole();
}

void SystemInit(void)
{
}
/*${function:end}*/
