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
    /* clang-format off */

    const clock_root_config_t lpuartClkCfg = {
        .clockOff = false,
	.mux = 0,
	.div = 1
    };

    /* 250MHz DMA clock */
    const clock_root_config_t dmaClkCfg = {
        .clockOff = false,
        .mux = kCLOCK_M33_ClockRoot_MuxSysPll1Pfd0Div2, // 500MHz
        .div = 2
    };

    /* clang-format on */
    BOARD_InitBootPins();
    BOARD_BootClockRUN();

    CLOCK_SetRootClock(EXAMPLE_DMA_CLOCK_ROOT, &dmaClkCfg);
    CLOCK_EnableClock(EXAMPLE_DMA_CLOCK_GATE);
    CLOCK_SetRootClock(LPUART_CLOCK_ROOT, &lpuartClkCfg);
    CLOCK_EnableClock(LPUART_CLOCK_GATE);
}
/*${function:end}*/
