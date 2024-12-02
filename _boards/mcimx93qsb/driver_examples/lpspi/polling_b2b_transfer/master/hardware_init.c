/*
 * Copyright 2022 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "app.h"
#include "board.h"
#include "pin_mux.h"
#include "fsl_common.h"
#include "clock_config.h"
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
    const clock_root_config_t lpi2cClkCfg = {
        .clockOff = false,
        .mux = 0, // 24MHz oscillator source
        .div = 1
    };
    /* clang-format on */
    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    CLOCK_SetRootClock(LPSPI_MASTER_CLOCK_ROOT, &lpspiClkCfg);
    CLOCK_SetRootClock(BOARD_PCAL6524_I2C_CLOCK_ROOT, &lpi2cClkCfg);
    CLOCK_EnableClock(BOARD_PCAL6524_I2C_CLOCK_GATE);

    /* Select SPI3 signals */
    pcal6524_handle_t handle;
    BOARD_InitPCAL6524(&handle);
    PCAL6524_SetDirection(&handle, (1 << BOARD_PCAL6524_EXP_SEL), kPCAL6524_Output);
    PCAL6524_SetPins(&handle, (1 << BOARD_PCAL6524_EXP_SEL));
}
/*${function:end}*/
