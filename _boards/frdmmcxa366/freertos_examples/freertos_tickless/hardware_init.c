/*
 * Copyright 2025-2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "FreeRTOSConfig.h"
#include "fsl_lptmr.h"
#include "pin_mux.h"
#include "fsl_clock.h"
#include "fsl_reset.h"
#include "board.h"
#include <stdbool.h>
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitDEBUG_UARTPins();
    BOARD_InitBUTTONsPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

#if configUSE_TICKLESS_IDLE == 2
    CLOCK_AttachClk(kFRO_LF_DIV_to_LPTMR0);
    CLOCK_SetClockDiv(kCLOCK_DivLPTMR0, 1u);

    CLOCK_SetupFRO16KClocking(kCLKE_16K_SYSTEM | kCLKE_16K_COREMAIN);
#endif
}
/*${function:end}*/
