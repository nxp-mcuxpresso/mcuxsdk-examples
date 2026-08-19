/*
 * Copyright 2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "fsl_common.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_debug_console.h"
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

    CLOCK_SetRootClock(kCLOCK_Root_Lpspi3, &lpspiClkCfg);
    CLOCK_EnableClock(kCLOCK_Lpspi3);
    CLOCK_SetRootClock(kCLOCK_Root_Lpi2c2, &lpi2cClkCfg);
    CLOCK_EnableClock(kCLOCK_Lpi2c2);

    /* Select SPI3 signals */
    pcal6524_handle_t handle;
    BOARD_InitPCAL6524_B(&handle);
    PCAL6524_SetDirection(&handle, (1 << BOARD_PCAL6524_B_EXP_SEL), kPCAL6524_Output);
    PCAL6524_SetPins(&handle, (1 << BOARD_PCAL6524_B_EXP_SEL));
}

uint32_t LPSPI3_GetFreq(void)
{
    return CLOCK_GetIpFreq(kCLOCK_Root_Lpspi3);
}

/*${function:end}*/
