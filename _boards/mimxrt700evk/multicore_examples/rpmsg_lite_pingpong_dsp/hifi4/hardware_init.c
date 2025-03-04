/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include <xtensa/config/core.h>
#include <xtensa/xos.h>
#include "app.h"
#include "mcmgr.h"
#include "pin_mux.h"
#include "board.h"
#include "fsl_inputmux.h"
/*${header:end}*/

/*${macro:start}*/
/*${macro:end}*/
extern void MU4_B_IRQHandler2(void *arg);
/*${function:start}*/
static void XOS_Init(void)
{
    xos_set_clock_freq(XOS_CLOCK_FREQ);
    xos_start_system_timer(-1, 0);
}

void BOARD_InitHardware(void)
{
    CLOCK_SetXtalFreq(BOARD_XTAL_SYS_CLK_HZ); /* Note: need tell clock driver the frequency of OSC. */

    BOARD_InitBootPins();
    BOARD_InitDebugConsole();

    XOS_Init();
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
