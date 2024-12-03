/*
 * Copyright 2022-2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "fsl_iomuxc.h"
#include "fsl_lpuart.h"
#include "fsl_lptmr.h"
#include "power_mode_switch.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "lpm.h"
#include "fsl_mu.h"
#include "rsc_table.h"
/*${header:end}*/

/*${function:start}*/

void BOARD_InitHardware(void)
{
    /* clang-format off */
    const clock_root_config_t lpi2cClkCfg = {
        .clockOff = false,
	.mux = 0, // 24MHz oscillator source
	.div = 1
    };
    const clock_root_config_t lptmrClkCfg = {
        .clockOff = false,
        .mux = 0, // 24MHz oscillator source
        .div = 1
    };

    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    CLOCK_SetRootClock(kCLOCK_Root_Lptmr1, &lptmrClkCfg);
    CLOCK_EnableClock(kCLOCK_Lptmr1);
    CLOCK_SetRootClock(kCLOCK_Root_Lptmr2, &lptmrClkCfg);
    CLOCK_EnableClock(kCLOCK_Lptmr2);
    CLOCK_SetRootClock(kCLOCK_Root_Lpi2c1, &lpi2cClkCfg);
    CLOCK_EnableClock(kCLOCK_Lpi2c1);
    CLOCK_SetRootClock(BOARD_PCAL6524_I2C_CLOCK_ROOT, &lpi2cClkCfg);
    CLOCK_EnableClock(BOARD_PCAL6524_I2C_CLOCK_GATE);

    /* copy resource table to destination address(TCM) */
    copyResourceTable();
}
