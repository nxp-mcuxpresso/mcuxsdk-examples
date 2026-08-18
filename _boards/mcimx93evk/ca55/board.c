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

void BOARD_InitClock(void)
{
    g_clockSourceFreq[kCLOCK_Osc24M]          = 24000000U;
    g_clockSourceFreq[kCLOCK_SysPll1]         = 4000000000U;
    g_clockSourceFreq[kCLOCK_SysPll1Pfd0]     = 1000000000U;
    g_clockSourceFreq[kCLOCK_SysPll1Pfd0Div2] = 500000000U;
    g_clockSourceFreq[kCLOCK_SysPll1Pfd1]     = 800000000U;
    g_clockSourceFreq[kCLOCK_SysPll1Pfd1Div2] = 400000000U;
    g_clockSourceFreq[kCLOCK_SysPll1Pfd2]     = 625000000U;
    g_clockSourceFreq[kCLOCK_SysPll1Pfd2Div2] = 312500000U;
    g_clockSourceFreq[kCLOCK_AudioPll1Out]    = 393216000U;
    g_clockSourceFreq[kCLOCK_AudioPll1]       = 393216000U;
}

static void pin_mux_lpuart2(void)
{
    IOMUXC_SetPinMux(IOMUXC_PAD_UART2_RXD__LPUART2_RX, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_UART2_TXD__LPUART2_TX, 0U);
    IOMUXC_SetPinConfig(IOMUXC_PAD_UART2_RXD__LPUART2_RX, IOMUXC_PAD_PD_MASK);
    IOMUXC_SetPinConfig(IOMUXC_PAD_UART2_TXD__LPUART2_TX, IOMUXC_PAD_DSE(15U));
}

static void clock_config_lpuart2(void)
{
    const clock_root_config_t uartClkCfg = {
        .clockOff = false,
        .mux      = 0, /* 24MHz oscillator source */
        .div      = 1,
    };

    CLOCK_SetRootClock(kCLOCK_Root_Lpuart2, &uartClkCfg);
}

/* Initialize debug console. */
void BOARD_InitDebugConsole(void)
{
    pin_mux_lpuart2();
    clock_config_lpuart2();

    DbgConsole_Init(BOARD_DEBUG_UART_INSTANCE, BOARD_DEBUG_UART_BAUDRATE, BOARD_DEBUG_UART_TYPE,
                    BOARD_DEBUG_UART_CLK_FREQ);

}

/* Initialize MMU, configure memory attributes for each region. */
void BOARD_InitMemory(void)
{
    MMU_init();
}

void BOARD_RdcInit(void)
{
}
