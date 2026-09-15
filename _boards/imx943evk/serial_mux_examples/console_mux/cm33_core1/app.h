/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "fsl_clock.h"
/*${header:end}*/

/*${macro:start}*/
/* Hub core UART-MUX bridge wiring. Which physical LPUART carries the muxed
 * COBS stream depends on whether the hub keeps a dedicated console:
 *   - CONFIG_MUX_HUB_DEDICATED_CONSOLE=n: the hub has no separate console,
 *     so the mux bridge takes the board console UART (LPUART8) and carries
 *     the hub console channel plus all client channels.
 *   - CONFIG_MUX_HUB_DEDICATED_CONSOLE=y: LPUART8 stays a plain console UART
 *     for the hub (standard DbgConsole/PRINTF) and the mux bridge moves to
 *     LPUART11, carrying only the client virtual channels.
 * The generic demo requires these to be defined here; an undefined macro is
 * a hard build error. */
#if defined(CONFIG_MUX_HUB_DEDICATED_CONSOLE)
#define MUX_LPUART_INSTANCE_IDX (11U)
#define MUX_LPUART            LPUART11
#define MUX_LPUART_IRQn       LPUART11_IRQn
#define MUX_LPUART_IRQHandler LPUART11_IRQHandler
#define MUX_LPUART_CLK_FREQ   CLOCK_GetRate(kCLOCK_Lpuart11)
#else
#define MUX_LPUART_INSTANCE_IDX (8U)
#define MUX_LPUART            LPUART8
#define MUX_LPUART_IRQn       LPUART8_IRQn
#define MUX_LPUART_IRQHandler LPUART8_IRQHandler
#define MUX_LPUART_CLK_FREQ   CLOCK_GetRate(kCLOCK_Lpuart8)
#endif

/* UART-MUX line rate (board-common; every core must agree). */
#define MUX_BAUDRATE          115200U

#if defined(CONFIG_MUX_HUB_DEDICATED_CONSOLE)
/* Hub-dedicated console mode (CONFIG_MUX_HUB_DEDICATED_CONSOLE=y): the hub
 * keeps its OWN physical console UART (LPUART8) driven by standard
 * DbgConsole/PRINTF, while the mux UART (LPUART11 above) carries only the
 * client virtual channels. These macros are referenced only in that mode. */
#define MUX_HUB_CONSOLE_LPUART_INSTANCE_IDX (8U)
#define MUX_HUB_CONSOLE_BAUDRATE            115200U
#define MUX_HUB_CONSOLE_CLK_FREQ            CLOCK_GetRate(kCLOCK_Lpuart8)
#endif

static inline uint64_t CLOCK_GetCoreSysClkFreq(void)
{
    return CLOCK_GetRate(kCLOCK_M33sync);
}
/*${macro:end}*/

/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
