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

/******************************************************************
 * Following declearation is needed in developing, but not needed
 * after merging hardware_init.c and apps.h with epit.c, and will be
 * discarded after packaging.
 */
void EXAMPLE_GPT_IRQHandler(void);
/********************** End of forword declaration. ***************/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* Board specific RDC settings */
    BOARD_RdcInit();

    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    BOARD_InitMemory();
}
/*${function:end}*/
