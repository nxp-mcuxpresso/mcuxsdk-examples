/*
 * Copyright 2021 NXP
 * All rights reserved.
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
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
}

void BOARD_ConfigTriggerSource(void)
{
    /* Structure of initialize XBAR. */
    xbar_control_config_t xbarConfig;

    /* Configure SIM for ADC hw trigger source selection */
    SIM->CTRL_REG |= 0x8U;

    /* Init xbar module. */
    XBAR_Init(XBAR);
    /* Configure the XBAR signal connections. */
    XBAR_SetSignalsConnection(XBAR, kXBAR_InputLptmrOutput, kXBAR_OutputAdcTrgA);
    /* Configure the XBAR interrupt. */
    xbarConfig.activeEdge  = kXBAR_EdgeRising;
    xbarConfig.requestType = kXBAR_RequestDisable;
    XBAR_SetOutputSignalConfig(XBAR, kXBAR_OutputXbDmaInt, &xbarConfig);
}
/*${function:end}*/
