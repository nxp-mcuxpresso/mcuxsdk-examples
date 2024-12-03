/*
 * Copyright 2023 NXP
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
    pcal6524_handle_t handle;
    /* clang-format off */
    const clock_root_config_t flexcanClkCfg = {
        .clockOff = false,
	.mux = 2,
	.div = 10
    };
    const clock_root_config_t lpi2cClkCfg = {
        .clockOff = false,
        .mux = 0, // 24MHZ oscillator source
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

    BOARD_InitPCAL6524_B(&handle);

    PCAL6524_SetDirection(&handle, (1 << BOARD_PCAL6524_B_EXP_SEL), kPCAL6524_Output);
    PCAL6524_SetDirection(&handle, (1 << BOARD_PCAL6524_B_CAN2_EN), kPCAL6524_Output);
    PCAL6524_SetDirection(&handle, (1 << BOARD_PCAL6524_B_CAN2_STBY_B), kPCAL6524_Output);

    /* Route GPIO_IO25,GPIO_IO27 to CAN2 */
    PCAL6524_ClearPins(&handle, (1 << BOARD_PCAL6524_B_EXP_SEL));

    /* Initialize TJA1463AT with normal mode */
    /* Clear CAN STBY_N and EN signal of TJA1463AT */
    PCAL6524_ClearPins(&handle, (1 << BOARD_PCAL6524_B_CAN2_STBY_B) | (1 << BOARD_PCAL6524_B_CAN2_EN));
    SDK_DelayAtLeastUs(100U, SystemCoreClock);

    /* Setup CAN EN and STBY_N signal of TJA1436AT */
    PCAL6524_SetPins(&handle, (1 << BOARD_PCAL6524_B_CAN2_STBY_B) | (1 << BOARD_PCAL6524_B_CAN2_EN));
}
/*${function:end}*/
