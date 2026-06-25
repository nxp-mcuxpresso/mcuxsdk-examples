/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitLEDsPins();
    BOARD_InitButtonsPins();
    BOARD_InitLINPins();
    /* Set LIN LPUART clock */
    CLOCK_SetIpSrc(kCLOCK_Lpuart0, kCLOCK_IpSrcFro192M);
    CLOCK_SetIpSrcDiv(kCLOCK_Lpuart0, 15U);
#if defined(TIMER_TPM) && TIMER_TPM
    /* Set Timer LPUART clock */
    CLOCK_SetIpSrc(DEMO_TPM_CLOCK, kCLOCK_IpSrcFro192M);
    CLOCK_SetIpSrcDiv(DEMO_TPM_CLOCK, 15U);
#endif
    CLOCK_EnableClock(kCLOCK_Fro_hf_div);

    BOARD_InitDebugConsole();
}
/*${function:end}*/
