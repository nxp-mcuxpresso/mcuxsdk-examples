/*
 * Copyright 2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_debug_console.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* clang-format off */

    const clock_root_config_t lpuartClkCfg = {
        .clockOff = false,
	.mux = 0,
	.div = 1
    };
    /* clang-format on */
    BOARD_InitBootPins();
    BOARD_BootClockRUN();

    CLOCK_SetRootClock(kCLOCK_Root_Lpuart2, &lpuartClkCfg);
    CLOCK_EnableClock(kCLOCK_Lpuart2);
}

uint32_t LPUART2_GetFreq(void)
{
    return CLOCK_GetIpFreq(kCLOCK_Root_Lpuart2);
}

/*${function:end}*/
