/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "pin_mux.h"

void BOARD_InitLpuart1Pins(void) {                                /*!< Function assigned for the core: undefined[cm33] */
    IOMUXC_SetPinMux(IOMUXC_PAD_UART1_RXD__LPUART1_RX, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_UART1_TXD__LPUART1_TX, 0U);

    IOMUXC_SetPinConfig(IOMUXC_PAD_UART1_RXD__LPUART1_RX,
                        IOMUXC_PAD_PD_MASK);
    IOMUXC_SetPinConfig(IOMUXC_PAD_UART1_TXD__LPUART1_TX,
                        IOMUXC_PAD_DSE(15U));
}

void BOARD_InitLpuart2Pins(void) {                                /*!< Function assigned for the core: undefined[cm33] */
    IOMUXC_SetPinMux(IOMUXC_PAD_UART2_RXD__LPUART2_RX, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_UART2_TXD__LPUART2_TX, 0U);

    IOMUXC_SetPinConfig(IOMUXC_PAD_UART2_RXD__LPUART2_RX,
                        IOMUXC_PAD_PD_MASK);
    IOMUXC_SetPinConfig(IOMUXC_PAD_UART2_TXD__LPUART2_TX,
                        IOMUXC_PAD_DSE(15U));
}

void BOARD_InitLpuart3Pins(void) {                                /*!< Function assigned for the core: undefined[cm33] */
    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO15__LPUART3_RX, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO14__LPUART3_TX, 0U);

    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO15__LPUART3_RX,
                        IOMUXC_PAD_PD_MASK);
    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO14__LPUART3_TX,
                        IOMUXC_PAD_DSE(15U));
}

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitBootPins
 * Description   : Calls initialization functions.
 *
 * END ****************************************************************************************************************/
void BOARD_InitBootPins(void)
{
    BOARD_InitPins();
#if BOARD_DEBUG_UART_INSTANCE == 1
    BOARD_InitLpuart1Pins();
#elif BOARD_DEBUG_UART_INSTANCE == 2
    BOARD_InitLpuart2Pins();
#elif BOARD_DEBUG_UART_INSTANCE == 3
    BOARD_InitLpuart3Pins();
#endif
}

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitPins, assigned for the Cortex-M33 core.
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitPins(void)
{
    IOMUXC_SetPinMux(IOMUXC_PAD_ENET1_MDC__NETC_MDC, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_ENET1_MDIO__NETC_MDIO, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_ENET1_TD3__ETH0_RGMII_TD3, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_ENET1_TD2__ETH0_RGMII_TD2, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_ENET1_TD1__ETH0_RGMII_TD1, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_ENET1_TD0__ETH0_RGMII_TD0, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_ENET1_TX_CTL__ETH0_RGMII_TX_CTL, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_ENET1_TXC__ETH0_RGMII_TX_CLK, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_ENET1_RX_CTL__ETH0_RGMII_RX_CTL, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_ENET1_RXC__ETH0_RGMII_RX_CLK, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_ENET1_RD0__ETH0_RGMII_RD0, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_ENET1_RD1__ETH0_RGMII_RD1, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_ENET1_RD2__ETH0_RGMII_RD2, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_ENET1_RD3__ETH0_RGMII_RD3, 0U);

    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO08__LPI2C7_SDA, 1U);
    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO09__LPI2C7_SCL, 1U);

    IOMUXC_SetPinConfig(IOMUXC_PAD_ENET1_MDC__NETC_MDC, 0x57e);
    IOMUXC_SetPinConfig(IOMUXC_PAD_ENET1_MDIO__NETC_MDIO, 0x57e);
    IOMUXC_SetPinConfig(IOMUXC_PAD_ENET1_TD3__ETH0_RGMII_TD3, 0x57e);
    IOMUXC_SetPinConfig(IOMUXC_PAD_ENET1_TD2__ETH0_RGMII_TD2, 0x57e);
    IOMUXC_SetPinConfig(IOMUXC_PAD_ENET1_TD1__ETH0_RGMII_TD1, 0x57e);
    IOMUXC_SetPinConfig(IOMUXC_PAD_ENET1_TD0__ETH0_RGMII_TD0, 0x57e);
    IOMUXC_SetPinConfig(IOMUXC_PAD_ENET1_TX_CTL__ETH0_RGMII_TX_CTL, 0x57e);
    IOMUXC_SetPinConfig(IOMUXC_PAD_ENET1_TXC__ETH0_RGMII_TX_CLK, 0x5fe);
    IOMUXC_SetPinConfig(IOMUXC_PAD_ENET1_RX_CTL__ETH0_RGMII_RX_CTL, 0x57e);
    IOMUXC_SetPinConfig(IOMUXC_PAD_ENET1_RXC__ETH0_RGMII_RX_CLK, 0x5fe);
    IOMUXC_SetPinConfig(IOMUXC_PAD_ENET1_RD0__ETH0_RGMII_RD0, 0x57e);
    IOMUXC_SetPinConfig(IOMUXC_PAD_ENET1_RD1__ETH0_RGMII_RD1, 0x57e);
    IOMUXC_SetPinConfig(IOMUXC_PAD_ENET1_RD2__ETH0_RGMII_RD2, 0x57e);
    IOMUXC_SetPinConfig(IOMUXC_PAD_ENET1_RD3__ETH0_RGMII_RD3, 0x57e);

    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO08__LPI2C7_SDA,
                        IOMUXC_PAD_DSE(15U) |
                        IOMUXC_PAD_FSEL1(2U) |
                        IOMUXC_PAD_OD_MASK);
    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO09__LPI2C7_SCL,
                        IOMUXC_PAD_DSE(15U) |
                        IOMUXC_PAD_FSEL1(2U) |
                        IOMUXC_PAD_OD_MASK);
}

/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
