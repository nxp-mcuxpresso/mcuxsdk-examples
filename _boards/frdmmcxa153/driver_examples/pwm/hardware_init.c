/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "fsl_device_registers.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_inputmux.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
	
	INPUTMUX_Init(INPUTMUX0);
	INPUTMUX_AttachSignal(INPUTMUX0, 0U, kINPUTMUX_TrigIn0ToFlexPwm0Fault);
	INPUTMUX_AttachSignal(INPUTMUX0, 1U, kINPUTMUX_TrigIn1ToFlexPwm0Fault);
	INPUTMUX_AttachSignal(INPUTMUX0, 2U, kINPUTMUX_TrigIn2ToFlexPwm0Fault);	
	INPUTMUX_AttachSignal(INPUTMUX0, 3U, kINPUTMUX_TrigIn3ToFlexPwm0Fault);
	
}
/*${function:end}*/
