/*
 * Copyright 2022 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "app.h"
#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_debug_console.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* clang-format off */

    const clock_root_config_t rgpioClkCfg = {
        .clockOff = false,
        .mux = 0, // 24Mhz Mcore root buswake clock
        .div = 1
    };
    /* clang-format on */
    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    CLOCK_SetRootClock(EXAMPLE_RGPIO_CLOCK_ROOT, &rgpioClkCfg);
    CLOCK_EnableClock(EXAMPLE_RGPIO_CLOCK_GATE);
    CLOCK_EnableClock(kCLOCK_Gpio2);
    /* Set PCNS register value to 0x0 to prepare the RGPIO initialization */
    BOARD_LED_RGPIO->PCNS = 0x0;
}
/*${function:end}*/
