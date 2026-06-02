/*
 * Copyright 2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_clock.h"
#include "fsl_inputmux.h"
#include "fsl_spc.h"
#include "fsl_debug_console.h"
/*${header:end}*/

/*${variable:start}*/

/*${variable:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_InitI2CPins();
    BOARD_InitFlexioMculcdPins();
    BOARD_InitFlexioCameraPins();
    BOARD_InitFlexioCameraSmartdmaTrigger();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /* Enable clock for PCLK. */
    CLOCK_AttachClk(kFRO_HF_DIV_to_CLKOUT);
    CLOCK_SetClockDiv(kCLOCK_DivCLKOUT, 15U);

    /* Enable flexio clock */
    CLOCK_AttachClk(kFRO_HF_to_FLEXIO0);
    CLOCK_SetClockDiv(kCLOCK_DivFLEXIO0, 1u);
    CLOCK_EnableClock(kCLOCK_GateFLEXIO0);

    /* Init camera I2C clock. */
    CLOCK_AttachClk(kFRO_LF_DIV_to_LPI2C2);
    CLOCK_EnableClock(kCLOCK_GateLPI2C2);
    CLOCK_SetClockDiv(kCLOCK_DivLPI2C2, 1u);
}

/*${function:end}*/
