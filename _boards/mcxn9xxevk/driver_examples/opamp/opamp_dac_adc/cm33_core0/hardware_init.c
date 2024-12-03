/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "fsl_common.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom4Clk, 1u);
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    /* attach FRO HF to DAC0 */
    CLOCK_SetClkDiv(kCLOCK_DivDac0Clk, 1u);
    CLOCK_AttachClk(kFRO_HF_to_DAC0);

    /* attach FRO HF to ADC1 */
    CLOCK_SetClkDiv(kCLOCK_DivAdc1Clk, 1u);
    CLOCK_AttachClk(kFRO_HF_to_ADC1);

    /* enable analog module */
    SPC0->ACTIVE_CFG1 |= 0x111;

    BOARD_InitPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
}
/*${function:end}*/
