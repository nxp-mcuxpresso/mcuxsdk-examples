/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "fsl_clock.h"
#include "fsl_reset.h"
#include "board.h"
#include "fsl_debug_console.h"
/*${header:end}*/

/******************************************************************************
 * IMPORTANT: COMPILE IN RELEASE MODE
 *
 * CoreMark benchmark must be compiled in RELEASE mode to obtain accurate
 * performance measurements. Debug builds include additional overhead from
 * debugging symbols, assertions, and unoptimized code that will significantly
 * reduce the benchmark score and provide misleading results.
 *****************************************************************************/

/* Detect debug build and issue warning */
#if defined(DEBUG) || defined(_DEBUG) || !defined(NDEBUG)
    #define BUILD_MODE_WARNING_MSG "\n*** WARNING: Compiled in DEBUG mode ***\n" \
                                   "*** CoreMark scores will be inaccurate and significantly lower ***\n" \
                                   "*** Please recompile in RELEASE mode for valid benchmark results ***\n\n"
#endif


/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_InitDEBUG_UARTPins();
    BOARD_InitDebugConsole();

#if defined(DEBUG) || defined(_DEBUG) || !defined(NDEBUG)
    PRINTF(BUILD_MODE_WARNING_MSG);
    assert(false);
#endif
}
/*${function:end}*/