/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
#include "hal_pinctrl.h"
#include "pin_mux.h"
#include "board.h"
#include "mmu.h"
#include "hal_clock.h"

/*******************************************************************************
 * Code
 ******************************************************************************/

void BOARD_InitDebugConsolePins(void)
{
    HAL_PinctrlSetPinMux(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_GPIO_IO15__LPUART3_RX, 0U);
    HAL_PinctrlSetPinMux(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_GPIO_IO14__LPUART3_TX, 0U);

    HAL_PinctrlSetPinCfg(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_GPIO_IO15__LPUART3_RX,
                         HAL_PINCTRL_PLATFORM_IOMUXC_PAD_PD_MASK);
    HAL_PinctrlSetPinCfg(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_GPIO_IO14__LPUART3_TX,
                         HAL_PINCTRL_PLATFORM_IOMUXC_PAD_DSE(15U));
}

/* Initialize debug console. */
void BOARD_InitDebugConsole(void)
{
    /* clang-format off */
    hal_clk_t hal_clk = {
        .clk_id        = BOARD_DEBUG_UART_CLOCK_ROOT,
        .clk_round_opt = hal_clk_round_auto,
        .rate          = 24000000UL,
    };
    /* clang-format on */

    HAL_ClockSetRate(&hal_clk);
    HAL_ClockEnable(&hal_clk);

    BOARD_InitDebugConsolePins();

    DbgConsole_Init(BOARD_DEBUG_CONSOLE_PORT, BOARD_DEBUG_CONSOLE_BAUDRATE, BOARD_DEBUG_CONSOLE_TYPE,
                    HAL_ClockGetRate(hal_clk.clk_id));
}
