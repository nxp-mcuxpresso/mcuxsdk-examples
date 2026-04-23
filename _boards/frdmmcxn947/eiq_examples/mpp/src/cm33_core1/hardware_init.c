/*
 * Copyright 2022-2023, 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "fsl_common.h"
#include "pin_mux.h"
#include "board.h"
#include "mcmgr.h"
#include "fsl_debug_console.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* enable clock for GPIO */
    CLOCK_EnableClock(kCLOCK_Gpio3);
    BOARD_InitBootPins();

#ifdef ENABLE_CORE1_CONSOLE
    BOARD_InitDebugConsole();
#endif

    SystemCoreClock = CLOCK_GetCoreSysClkFreq();
}

/*!
 * @brief Application-specific implementation of the SystemInitHook() weak function.
 */
void SystemInitHook(void)
{
    /* Initialize MCMGR - low level multicore management library. Call this
       function as close to the reset entry as possible to allow CoreUp event
       triggering. The SystemInitHook() weak function overloading is used in this
       application. */
    (void)MCMGR_EarlyInit();
}
/*${function:end}*/
