/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "board.h"
#include "pin_mux.h"
#include "fsl_ccm32k.h"
#include "clock_config.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /* Enable FRO32K so that WDOG0 operates correctly.
     * WDOG0 requires FRO32K to be enabled since it is configured as the default clock source.*/
    CCM32K_Enable32kFro(CCM32K, true);
    CLOCK_EnableClock(kCLOCK_Wdog0);
}
/*${function:end}*/
