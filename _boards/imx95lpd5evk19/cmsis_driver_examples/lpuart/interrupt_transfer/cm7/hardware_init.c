/*
 * Copyright 2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "app.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_common.h"
#include "sm_platform.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* clang-format off */
    hal_clk_t hal_clk = {
        .clk_id = BOARD_DEBUG_UART_CLOCK_ROOT_FOR_CM7_WITH_SM,
        .pclk_id = hal_clock_osc24m,
        .div = 1,
        .enable_clk = true,
        .clk_round_opt = hal_clk_round_auto,
    };

    /* clang-format on */
    SM_Platform_Init();
    BOARD_InitBootPins();
    BOARD_BootClockRUN();

    HAL_ClockSetRootClk(&hal_clk);
}

#if BOARD_DEBUG_UART_INSTANCE == 1
uint32_t LPUART1_GetFreq(void)
#elif BOARD_DEBUG_UART_INSTANCE == 2
uint32_t LPUART2_GetFreq(void)
#elif BOARD_DEBUG_UART_INSTANCE == 3
uint32_t LPUART3_GetFreq(void)
#endif
{
    return HAL_ClockGetIpFreq(BOARD_DEBUG_UART_CLOCK_ROOT_FOR_CM7_WITH_SM);
}

/*${function:end}*/
