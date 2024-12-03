/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
#include "fsl_vref.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    vref_config_t vrefConfig;

    /* attach FRO 12M to FLEXCOMM4 (debug console) */
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom4Clk, 1u);
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    /* attach FRO 12M to CMP0 */
    CLOCK_SetClkDiv(kCLOCK_DivCmp0FClk, 1u);
    CLOCK_AttachClk(kFRO12M_to_CMP0F);

    /* enable VREF */
    SPC0->ACTIVE_CFG1 |= 0x1;
    /* enable CMP0 */
    SPC0->ACTIVE_CFG1 |= (0x1 << 16u);

    VREF_GetDefaultConfig(&vrefConfig);
    /* Initialize the VREF mode. */
    VREF_Init(DEMO_VREF_BASE, &vrefConfig);
    /* Get a 1.8V reference voltage. */
    VREF_SetTrim21Val(DEMO_VREF_BASE, 8U);

    BOARD_InitPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
}
/*${function:end}*/
