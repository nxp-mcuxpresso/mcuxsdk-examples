/*
 * Copyright 2025-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "fsl_common.h"
#include "pin_mux.h"
#include "board.h"
#include "mcmgr.h"
#include "tzm_config.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();

    BOARD_InitDebugConsole();

    CLOCK_EnableClock(kCLOCK_SenseAccessRamArbiter0);
}

/*!
 * @brief Application-specific implementation of the SystemInitHook() weak function.
 */
void SystemInitHook(void)
{
    /* The TrustZone should be configured as early as possible after RESET.
       Therefore it is called from SystemInit() during startup. The SystemInitHook()
       weak function overloading is used for this purpose. */
    BOARD_InitBootTEE();
}
/*${function:end}*/
