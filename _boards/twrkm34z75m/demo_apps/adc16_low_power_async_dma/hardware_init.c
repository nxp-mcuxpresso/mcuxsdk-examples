/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "clock_config.h"
#include "pin_mux.h"
#include "board.h"
#include "fsl_xbar.h"

/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
}

void BOARD_ConfigTriggerSource(void)
{
    /* Configure SIM for ADC hw trigger source selection */
    SIM->CTRL_REG |= 0x8U;

    /* Init xbar module. */
    XBAR_Init(XBAR);
    /* Configure the XBAR signal connections. */
    XBAR_SetSignalsConnection(XBAR, kXBAR_InputLptmr0Output, kXBAR_OutputAdcTrgA);
}
/*${function:end}*/
