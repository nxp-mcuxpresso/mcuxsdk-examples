/*
 * Copyright 2019-2023, 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
#include "fsl_debug_console.h"
#include "fsl_iomuxc.h"
#include "fsl_clock.h"
#include "board.h"
#include "mmu.h"

/*******************************************************************************
 * Code
 ******************************************************************************/
void BOARD_InitDebugConsolePins(void)
{
    IOMUXC_SetPinMux(IOMUXC_UART4_RXD_UART4_RX, 0U);
    IOMUXC_SetPinConfig(IOMUXC_UART4_RXD_UART4_RX,
                        IOMUXC_SW_PAD_CTL_PAD_DSE(6U) | IOMUXC_SW_PAD_CTL_PAD_FSEL(2U));
    IOMUXC_SetPinMux(IOMUXC_UART4_TXD_UART4_TX, 0U);
    IOMUXC_SetPinConfig(IOMUXC_UART4_TXD_UART4_TX,
                        IOMUXC_SW_PAD_CTL_PAD_DSE(6U) | IOMUXC_SW_PAD_CTL_PAD_FSEL(2U));
}

/* Initialize debug console. */
void BOARD_InitDebugConsole(void)
{
    /* Set UART source clock to 24MHz OSC. */
    CLOCK_DisableClock(kCLOCK_Uart4);
    CLOCK_SetRootMux(kCLOCK_RootUart4, kCLOCK_UartRootmuxOsc24M);
    CLOCK_SetRootDivider(kCLOCK_RootUart4, 1U, 1U);
    CLOCK_EnableClock(kCLOCK_Uart4);

    BOARD_InitDebugConsolePins();

    DbgConsole_Init(BOARD_DEBUG_UART_INSTANCE, BOARD_DEBUG_UART_BAUDRATE, BOARD_DEBUG_UART_TYPE,
                    CLOCK_GetClockRootFreq(kCLOCK_Uart4ClkRoot));

}

/* Initialize MMU, configure memory attributes for each region. */
void BOARD_InitMemory(void)
{
    MMU_init();
}
