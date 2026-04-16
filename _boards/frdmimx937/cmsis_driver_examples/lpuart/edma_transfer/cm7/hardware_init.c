/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "app.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_common.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* clang-format off */
    clk_t clk = {
        .clkId = BOARD_DEBUG_UART_CLOCK_ROOT_FOR_CM7_WITH_SM,
        .pclkId = kCLOCK_osc24m,
        .rate = 24000000UL,
        //.enable_clk = true,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
    };

    /* clang-format on */
    SystemPlatformInit();
    BOARD_InitBootPins();
    BOARD_DeInitDEBUG_UARTPins();
    BOARD_BootClockRUN();

    CLOCK_SetParent(&clk);
    CLOCK_SetRate(&clk);
    CLOCK_EnableClock(clk.clkId);
}

#if BOARD_DEBUG_UART_INSTANCE == 1
uint32_t LPUART1_GetFreq(void)
#elif BOARD_DEBUG_UART_INSTANCE == 2
uint32_t LPUART2_GetFreq(void)
#elif BOARD_DEBUG_UART_INSTANCE == 3
uint32_t LPUART3_GetFreq(void)
#endif
{
    return CLOCK_GetRate(BOARD_DEBUG_UART_CLOCK_ROOT_FOR_CM7_WITH_SM);
}

/*${function:end}*/
