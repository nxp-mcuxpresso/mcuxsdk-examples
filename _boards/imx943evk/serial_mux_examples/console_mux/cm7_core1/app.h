/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "fsl_clock.h"
/*${header:end}*/

/*${macro:start}*/
/* UART-MUX line rate (board-common; must match the hub). */
#define MUX_BAUDRATE 115200U

/* Virtual console client core (cm7_core1), logical channel 1. */
static inline uint64_t CLOCK_GetCoreSysClkFreq(void)
{
    return CLOCK_GetRate(kCLOCK_M71);
}
/*${macro:end}*/

/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
