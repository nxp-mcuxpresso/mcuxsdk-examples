/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_common.h"
/*${header:end}*/

void DEMO_PWM_IRQHandler(void);

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* Board specific RDC settings */
    BOARD_RdcInit();

    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    BOARD_InitMemory();

    CLOCK_SetRootMux(kCLOCK_RootPdm, kCLOCK_PdmRootmuxOsc24M); /* Set PDM source from OSC 25MHZ */
    CLOCK_SetRootDivider(kCLOCK_RootPdm, 1U, 1U);              /* Set root clock to 25MHZ */
}
/*${function:end}*/
