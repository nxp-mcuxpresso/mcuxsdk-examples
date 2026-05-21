/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "pin_mux.h"

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitBootPins
 * Description   : Calls initialization functions.
 *
 * END ****************************************************************************************************************/
void BOARD_InitBootPins(void)
{
    BOARD_InitPins();
}

void BOARD_InitPins(void)
{
    IOMUXC_SetPinMux(IOMUXC_PAD_UART1_RXD__LPUART1_RX, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_UART1_TXD__LPUART1_TX, 0U);
    IOMUXC_SetPinConfig(IOMUXC_PAD_UART1_RXD__LPUART1_RX,
                        IOMUXC_PAD_PD_MASK);
    IOMUXC_SetPinConfig(IOMUXC_PAD_UART1_TXD__LPUART1_TX,
                        IOMUXC_PAD_DSE(15U));
}

/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
