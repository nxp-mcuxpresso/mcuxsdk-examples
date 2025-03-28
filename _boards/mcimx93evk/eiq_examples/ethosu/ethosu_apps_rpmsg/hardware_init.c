/*
 * Copyright 2021-2022 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "clock_config.h"
#include "pin_mux.h"
#include "rsc_table.h"
#include "board.h"

/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    const clock_root_config_t mlClkCfg = {
	.clockOff = false,
	.mux = 1, // 1000Mhz source
	.div = 1
    };

    const clock_root_config_t mlapbClkCfg = {
	.clockOff = false,
	.mux = 1, // 500Mhz source
	.div = 1
    };

    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    copyResourceTable();
    CLOCK_SetRootClock(kCLOCK_Root_Ml, &mlClkCfg);
    CLOCK_SetRootClock(kCLOCK_Root_MlApb, &mlapbClkCfg);
}
/*${function:end}*/
