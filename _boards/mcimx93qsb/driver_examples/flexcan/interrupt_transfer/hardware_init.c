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

    const clock_root_config_t flexcanClkCfg = {
        .clockOff = false,
	.mux = 2,
	.div = 10
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

    CLOCK_SetRootClock(FLEXCAN_CLOCK_ROOT, &flexcanClkCfg);
    CLOCK_EnableClock(FLEXCAN_CLOCK_GATE);
    CLOCK_SetRootClock(BOARD_PCAL6524_I2C_CLOCK_ROOT, &lpi2cClkCfg);
    CLOCK_EnableClock(BOARD_PCAL6524_I2C_CLOCK_GATE);

    /* Select CAN1 signals */
    pcal6524_handle_t handle1;
    BOARD_InitPCAL6524(&handle1);
    PCAL6524_SetDirection(&handle1, (1 << BOARD_PCAL6524_MIC_CAN_SEL), kPCAL6524_Output);
    PCAL6524_SetPins(&handle1, (1 << BOARD_PCAL6524_MIC_CAN_SEL));

    /* Select CAN_STBY signals */
    pcal6524_handle_t handle;
    BOARD_InitPCAL6524(&handle);
    PCAL6524_SetDirection(&handle, (1 << BOARD_PCAL6524_CAN_STBY), kPCAL6524_Output);
    PCAL6524_ClearPins(&handle, (1 << BOARD_PCAL6524_CAN_STBY));
}
/*${function:end}*/
