/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_common.h"
/*${header:end}*/

extern void FTM0_CH0_IRQHandler(void);

void FTM0_CH1_IRQHandler(void)
{
    FTM0_CH0_IRQHandler();
}

void FTM0_OVERFLOW_IRQHandler(void)
{
    FTM0_CH0_IRQHandler();
}

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitDEBUG_UARTPins();
    BOARD_InitFTM_INPUTPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    EnableIRQ(FTM0_CH1_IRQn);
    EnableIRQ(FTM0_OVERFLOW_IRQn);
}
/*${function:end}*/


