/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_clock.h"
#include "fsl_common.h"
#include "fsl_iomuxc.h"
#include "pin_mux.h"
#include "board.h"
#include "mmu.h"

/*******************************************************************************
 * Code
 ******************************************************************************/

/* Get UART Clock Id. */
clock_ip_name_t BOARD_GetUartClkId(uint32_t uartInstIdx)
{
    clock_ip_name_t uart_clk_id[] = LPUART_CLOCKS;

    return uart_clk_id[uartInstIdx];
}

void BOARD_InitDebugConsolePins(void)
{
    IOMUXC_SetPinMux(IOMUXC_PAD_DAP_TCLK_SWCLK__LPUART8_RX, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_DAP_TMS_SWDIO__LPUART8_TX, 0U);

    IOMUXC_SetPinConfig(IOMUXC_PAD_DAP_TCLK_SWCLK__LPUART8_RX, IOMUXC_PAD_PD_MASK);
    IOMUXC_SetPinConfig(IOMUXC_PAD_DAP_TMS_SWDIO__LPUART8_TX, IOMUXC_PAD_DSE(15U));
}

/* Initialize debug console. */
void BOARD_InitDebugConsole(void)
{
    /* clang-format off */
    clk_t clk = {
        .clkId = kCLOCK_IpInvalid,
        .clkRoundOpt = SCMI_CLOCK_ROUND_AUTO,
        .rate = 24000000UL,
    };

    clk.clkId = BOARD_GetUartClkId(BOARD_DEBUG_CONSOLE_PORT);

    CLOCK_SetRate(&clk);
    CLOCK_EnableClock(clk.clkId);

    BOARD_InitDebugConsolePins();

    /* clang-format on */
    DbgConsole_Init(BOARD_DEBUG_CONSOLE_PORT, BOARD_DEBUG_CONSOLE_BAUDRATE, BOARD_DEBUG_CONSOLE_TYPE,
                    CLOCK_GetRate(clk.clkId));
}
