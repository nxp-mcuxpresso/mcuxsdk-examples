/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "fsl_common.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_spc.h"
#include "app.h"
#include "fsl_vref.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    vref_config_t vrefConfig;

    /* attach FRO 12M to FLEXCOMM4 (debug console) */
    CLOCK_SetClkDiv(kCLOCK_DivFlexcom4Clk, 1U);
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    /* attach FRO HF to ADC1 */
    CLOCK_SetClkDiv(kCLOCK_DivAdc1Clk, 1U);
    CLOCK_AttachClk(kFRO_HF_to_ADC1);

    /* enable VREF */
    SPC_EnableActiveModeAnalogModules(DEMO_SPC_BASE, kSPC_controlVref);

    VREF_GetDefaultConfig(&vrefConfig);
    /* Initialize VREF module, the VREF module provides reference voltage and bias current for LPADC. */
    VREF_Init(DEMO_VREF_BASE, &vrefConfig);
    /* Set VREF output voltage to 1.8V. */
    VREF_SetTrim21Val(DEMO_VREF_BASE, DEMO_VREF_OUTPUT_VOLTAGE);

    BOARD_InitPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
}
/*${function:end}*/
