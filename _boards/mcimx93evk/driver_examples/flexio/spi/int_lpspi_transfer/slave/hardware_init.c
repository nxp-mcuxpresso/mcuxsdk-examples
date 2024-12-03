/*
 * Copyright 2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "board.h"
#include "pin_mux.h"
#include "app.h"
#include "clock_config.h"
#include "fsl_iomuxc.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* clang-format off */

    const clock_root_config_t lpspiClkCfg = {
        .clockOff = false,
        .mux = 0,
        .div = 1
    };

    const clock_root_config_t flexioClkCfg = {
        .clockOff = false,
	.mux = 0, /* 24MHz oscillator source */
	.div = 1
    };

    const clock_root_config_t lpi2cClkCfg = {
        .clockOff = false,
        .mux = 0, /* 24MHz oscillator source */
        .div = 1
    };
    /* clang-format on */

    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    CLOCK_SetRootClock(kCLOCK_Root_Flexio1, &flexioClkCfg);
    CLOCK_SetRootClock(kCLOCK_Root_Lpspi3, &lpspiClkCfg);
    CLOCK_SetRootClock(BOARD_ADP5585_I2C_CLOCK_ROOT, &lpi2cClkCfg);

    /* Select SPI3 signals */
    adp5585_handle_t handle;
    BOARD_InitADP5585(&handle);
    ADP5585_SetDirection(&handle, (1 << BOARD_ADP5585_EXP_SEL), kADP5585_Output);
    ADP5585_SetPins(&handle, (1 << BOARD_ADP5585_EXP_SEL));
}
/*${function:end}*/
