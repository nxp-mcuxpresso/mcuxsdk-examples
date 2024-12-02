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
extern void ADC_Configuration();
/*${header:end}*/

/******************************************************************
 * Following declearation is needed in developing, but not needed
 * after merging hardware_init.c and apps.h with tsc.c, and will be
 * discarded after packaging.
 */
void EXAMPLE_TSC_IRQHandler(void);
/********************** End of forword declaration. ***************/

/*${function:start}*/

void TSC_DIG_IRQHandler()
{
    EXAMPLE_TSC_IRQHandler();
}

void BOARD_InitHardware(void)
{
    BOARD_InitPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    ADC_Configuration();
    EnableIRQ(TSC_DIG_IRQn);
}
/*${function:end}*/
