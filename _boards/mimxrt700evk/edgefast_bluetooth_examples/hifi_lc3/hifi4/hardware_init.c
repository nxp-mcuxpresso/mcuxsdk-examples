/*
 * Copyright 2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include <xtensa/config/core.h>
#include <xtensa/xos.h>
#include "app.h"
#include "fsl_common.h"
#include "fsl_gpio.h"
#include "fsl_inputmux.h"
#include "pin_mux.h"
#include "fsl_sema42.h"
#include "board.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
/*${macro:end}*/

/*******************************************************************************
 * Variables
 ******************************************************************************/
/*${variable:start}*/
extern int NonCacheable_start, NonCacheable_end;
extern int NonCacheable_init_start, NonCacheable_init_end;
/*${variable:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${function:start}*/
static void BOARD_InitClock(void)
{
    INPUTMUX_Init(INPUTMUX0);
    INPUTMUX_AttachSignal(INPUTMUX0, 1U, kINPUTMUX_Mu4BToDspInterrupt);
}

static void XOS_Init(void)
{
    xos_set_clock_freq(XOS_CLOCK_FREQ);
    xos_start_system_timer(-1, 0);
}

void BOARD_InitHardware(void)
{
    /* Note: need tell clock driver the frequency of OSC. */
    CLOCK_SetXtalFreq(BOARD_XTAL_SYS_CLK_HZ);

    BOARD_InitBootPins();
    // BOARD_InitDebugConsole();
    BOARD_InitClock();
    XOS_Init();

    /* SEMA42 init */
    SEMA42_Init(APP_SEMA42);
}

/*${function:end}*/
