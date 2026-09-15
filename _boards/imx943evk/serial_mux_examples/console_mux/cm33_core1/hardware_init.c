/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "board.h"
#include "pin_mux.h"
#include "app.h"
#include "fsl_clock.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* clang-format off */
    clk_t muxUartClk = {
        .clkId = BOARD_GetUartClkId(MUX_LPUART_INSTANCE_IDX),
    };
#if defined(CONFIG_MUX_HUB_DEDICATED_CONSOLE)
    clk_t hubConsoleClk = {
        .clkId = BOARD_GetUartClkId(MUX_HUB_CONSOLE_LPUART_INSTANCE_IDX),
    };
#endif
    /* clang-format on */

    SystemPlatformInit();

    /* Only the UART-MUX bridge (LPUART8) pins are needed: the hub has no local
     * debug console, its output travels out on channel 0 through LPUART8. */
    BOARD_InitLpuartPins(MUX_LPUART_INSTANCE_IDX);

#if defined(CONFIG_MUX_HUB_DEDICATED_CONSOLE)
    /* Dedicated hub console mode: also bring up the hub's own console UART
     * (LPUART8) pins so standard DbgConsole/PRINTF can drive it. */
    BOARD_InitLpuartPins(MUX_HUB_CONSOLE_LPUART_INSTANCE_IDX);
#endif

    BOARD_BootClockRUN();
    BOARD_ConfigMPU();

    /* Bring up the mux-bridge LPUART clock (LPUART8 by default, LPUART11 when
     * the hub keeps a dedicated console). */
    CLOCK_SetParent(&muxUartClk);
    CLOCK_SetRate(&muxUartClk);
    CLOCK_EnableClock(muxUartClk.clkId);

#if defined(CONFIG_MUX_HUB_DEDICATED_CONSOLE)
    /* Bring up the hub's own console UART (LPUART8) clock. */
    CLOCK_SetParent(&hubConsoleClk);
    CLOCK_SetRate(&hubConsoleClk);
    CLOCK_EnableClock(hubConsoleClk.clkId);
#endif
}
/*${function:end}*/
