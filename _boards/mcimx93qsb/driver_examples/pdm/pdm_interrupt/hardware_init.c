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

    const clock_root_config_t pdmClkCfg = {
        .clockOff = false,
	.mux = 1, // select audiopll1out source(393216000 Hz)
	.div = 2 // output 196.608 Mhz
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

    CLOCK_SetRootClock(PDM_CLOCK_ROOT, &pdmClkCfg);
    CLOCK_EnableClock(PDM_CLOCK_GATE);
    CLOCK_SetRootClock(BOARD_PCAL6524_I2C_CLOCK_ROOT, &lpi2cClkCfg);
    CLOCK_EnableClock(BOARD_PCAL6524_I2C_CLOCK_GATE);

    /* Select PDM signals */
    pcal6524_handle_t handle1;
    BOARD_InitPCAL6524(&handle1);
    PCAL6524_SetDirection(&handle1, (1 << BOARD_PCAL6524_MIC_CAN_SEL), kPCAL6524_Output);
    PCAL6524_ClearPins(&handle1, (1 << BOARD_PCAL6524_MIC_CAN_SEL));
}
/*${function:end}*/
