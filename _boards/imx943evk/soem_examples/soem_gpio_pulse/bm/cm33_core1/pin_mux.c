/*
 * Copyright 2024 NXP
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
    BOARD_InitETH4Pins();
    BOARD_InitGPIOPins();
}

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitETH4Pins, assigned for the Cortex-M33 core.
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitETH4Pins(void)
{
#if BOARD_IMX943_TYPE == BOARD_IMX943_EMULATOR
#else
    HAL_PinctrlSetPinMux(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_ETH4_MDC_GPIO1__NETC_EMDC, 0U);
    HAL_PinctrlSetPinMux(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_ETH4_MDIO_GPIO2__NETC_EMDIO, 0U);
    HAL_PinctrlSetPinMux(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_ETH4_TXD3__NETC_PINMUX_ETH4_TXD3, 0U);
    HAL_PinctrlSetPinMux(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_ETH4_TXD2__NETC_PINMUX_ETH4_TXD2, 0U);
    HAL_PinctrlSetPinMux(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_ETH4_TXD1__NETC_PINMUX_ETH4_TXD1, 0U);
    HAL_PinctrlSetPinMux(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_ETH4_TXD0__NETC_PINMUX_ETH4_TXD0, 0U);
    HAL_PinctrlSetPinMux(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_ETH4_TX_CTL__NETC_PINMUX_ETH4_TX_CTL, 0U);
    HAL_PinctrlSetPinMux(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_ETH4_TX_CLK__NETC_PINMUX_ETH4_TX_CLK, 0U);
    HAL_PinctrlSetPinMux(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_ETH4_RX_CTL__NETC_PINMUX_ETH4_RX_CTL, 0U);
    HAL_PinctrlSetPinMux(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_ETH4_RX_CLK__NETC_PINMUX_ETH4_RX_CLK, 0U);
    HAL_PinctrlSetPinMux(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_ETH4_RXD0__NETC_PINMUX_ETH4_RXD0, 0U);
    HAL_PinctrlSetPinMux(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_ETH4_RXD1__NETC_PINMUX_ETH4_RXD1, 0U);
    HAL_PinctrlSetPinMux(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_ETH4_RXD2__NETC_PINMUX_ETH4_RXD2, 0U);
    HAL_PinctrlSetPinMux(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_ETH4_RXD3__NETC_PINMUX_ETH4_RXD3, 0U);

    HAL_PinctrlSetPinCfg(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_ETH4_MDC_GPIO1__NETC_EMDC, 0x57eU);
    HAL_PinctrlSetPinCfg(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_ETH4_MDIO_GPIO2__NETC_EMDIO, 0x57eU);
    HAL_PinctrlSetPinCfg(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_ETH4_TXD3__NETC_PINMUX_ETH4_TXD3, 0x57eU);
    HAL_PinctrlSetPinCfg(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_ETH4_TXD2__NETC_PINMUX_ETH4_TXD2, 0x57eU);
    HAL_PinctrlSetPinCfg(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_ETH4_TXD1__NETC_PINMUX_ETH4_TXD1, 0x57eU);
    HAL_PinctrlSetPinCfg(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_ETH4_TXD0__NETC_PINMUX_ETH4_TXD0, 0x57eU);
    HAL_PinctrlSetPinCfg(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_ETH4_TX_CTL__NETC_PINMUX_ETH4_TX_CTL, 0x57eU);
    HAL_PinctrlSetPinCfg(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_ETH4_TX_CLK__NETC_PINMUX_ETH4_TX_CLK, 0x57eU);
    HAL_PinctrlSetPinCfg(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_ETH4_RX_CTL__NETC_PINMUX_ETH4_RX_CTL, 0x57eU);
    HAL_PinctrlSetPinCfg(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_ETH4_RX_CLK__NETC_PINMUX_ETH4_RX_CLK, 0x57eU);
    HAL_PinctrlSetPinCfg(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_ETH4_RXD0__NETC_PINMUX_ETH4_RXD0, 0x57eU);
    HAL_PinctrlSetPinCfg(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_ETH4_RXD1__NETC_PINMUX_ETH4_RXD1, 0x57eU);
    HAL_PinctrlSetPinCfg(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_ETH4_RXD2__NETC_PINMUX_ETH4_RXD2, 0x57eU);
    HAL_PinctrlSetPinCfg(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_ETH4_RXD3__NETC_PINMUX_ETH4_RXD3, 0x57eU);
#endif
}

void BOARD_InitGPIOPins(void)
{
    HAL_PinctrlSetPinMux(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_GPIO_IO28__GPIO2_IO28, 0U);

    HAL_PinctrlSetPinCfg(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_GPIO_IO28__GPIO2_IO28,
                        HAL_PINCTRL_PLATFORM_IOMUXC_PAD_DSE(15U) |
                        HAL_PINCTRL_PLATFORM_IOMUXC_PAD_FSEL1(2U) |
                        HAL_PINCTRL_PLATFORM_IOMUXC_PAD_PD_MASK);
}

/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
