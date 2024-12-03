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
    BOARD_InitPins();
}

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitPins, assigned for the Cortex-M33 core.
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitPins(void)
{
    HAL_PinctrlSetPinMux(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_GPIO_IO15__LPUART3_RX, 0U);
    HAL_PinctrlSetPinMux(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_GPIO_IO14__LPUART3_TX, 0U);

    HAL_PinctrlSetPinCfg(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_GPIO_IO15__LPUART3_RX,
                        HAL_PINCTRL_PLATFORM_IOMUXC_PAD_PD_MASK);
    HAL_PinctrlSetPinCfg(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_GPIO_IO14__LPUART3_TX,
                        HAL_PINCTRL_PLATFORM_IOMUXC_PAD_DSE(15U));

    HAL_PinctrlSetPinMux(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_ENET1_MDC__NETC_MDC, 0U);
    HAL_PinctrlSetPinMux(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_ENET1_MDIO__NETC_MDIO, 0U);

    HAL_PinctrlSetPinCfg(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_ENET1_MDC__NETC_MDC, 0x57e);
    HAL_PinctrlSetPinCfg(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_ENET1_MDIO__NETC_MDIO, 0x57e);

    HAL_PinctrlSetPinMux(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_ENET2_TD3__ETH1_RGMII_TD3, 0U);
    HAL_PinctrlSetPinMux(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_ENET2_TD2__ETH1_RGMII_TD2, 0U);
    HAL_PinctrlSetPinMux(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_ENET2_TD1__ETH1_RGMII_TD1, 0U);
    HAL_PinctrlSetPinMux(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_ENET2_TD0__ETH1_RGMII_TD0, 0U);
    HAL_PinctrlSetPinMux(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_ENET2_TX_CTL__ETH1_RGMII_TX_CTL, 0U);
    HAL_PinctrlSetPinMux(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_ENET2_TXC__ETH1_RGMII_TX_CLK, 0U);
    HAL_PinctrlSetPinMux(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_ENET2_RX_CTL__ETH1_RGMII_RX_CTL, 0U);
    HAL_PinctrlSetPinMux(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_ENET2_RXC__ETH1_RGMII_RX_CLK, 0U);
    HAL_PinctrlSetPinMux(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_ENET2_RD0__ETH1_RGMII_RD0, 0U);
    HAL_PinctrlSetPinMux(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_ENET2_RD1__ETH1_RGMII_RD1, 0U);
    HAL_PinctrlSetPinMux(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_ENET2_RD2__ETH1_RGMII_RD2, 0U);
    HAL_PinctrlSetPinMux(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_ENET2_RD3__ETH1_RGMII_RD3, 0U);

    HAL_PinctrlSetPinCfg(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_ENET2_TD3__ETH1_RGMII_TD3, 0x57e);
    HAL_PinctrlSetPinCfg(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_ENET2_TD2__ETH1_RGMII_TD2, 0x57e);
    HAL_PinctrlSetPinCfg(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_ENET2_TD1__ETH1_RGMII_TD1, 0x57e);
    HAL_PinctrlSetPinCfg(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_ENET2_TD0__ETH1_RGMII_TD0, 0x57e);
    HAL_PinctrlSetPinCfg(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_ENET2_TX_CTL__ETH1_RGMII_TX_CTL, 0x57e);
    HAL_PinctrlSetPinCfg(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_ENET2_TXC__ETH1_RGMII_TX_CLK, 0x5fe);
    HAL_PinctrlSetPinCfg(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_ENET2_RX_CTL__ETH1_RGMII_RX_CTL, 0x57e);
    HAL_PinctrlSetPinCfg(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_ENET2_RXC__ETH1_RGMII_RX_CLK, 0x5fe);
    HAL_PinctrlSetPinCfg(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_ENET2_RD0__ETH1_RGMII_RD0, 0x57e);
    HAL_PinctrlSetPinCfg(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_ENET2_RD1__ETH1_RGMII_RD1, 0x57e);
    HAL_PinctrlSetPinCfg(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_ENET2_RD2__ETH1_RGMII_RD2, 0x57e);
    HAL_PinctrlSetPinCfg(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_ENET2_RD3__ETH1_RGMII_RD3, 0x57e);

    HAL_PinctrlSetPinMux(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_GPIO_IO23__LPI2C5_SCL, 1U);
    HAL_PinctrlSetPinMux(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_GPIO_IO22__LPI2C5_SDA, 1U);

    HAL_PinctrlSetPinCfg(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_GPIO_IO23__LPI2C5_SCL,
                         HAL_PINCTRL_PLATFORM_IOMUXC_PAD_DSE(15U) | HAL_PINCTRL_PLATFORM_IOMUXC_PAD_FSEL1(2U) |
                         HAL_PINCTRL_PLATFORM_IOMUXC_PAD_OD_MASK);
    HAL_PinctrlSetPinCfg(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_GPIO_IO22__LPI2C5_SDA,
                         HAL_PINCTRL_PLATFORM_IOMUXC_PAD_DSE(15U) | HAL_PINCTRL_PLATFORM_IOMUXC_PAD_FSEL1(2U) |
                         HAL_PINCTRL_PLATFORM_IOMUXC_PAD_OD_MASK);

    HAL_PinctrlSetPinMux(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_GPIO_IO09__LPI2C7_SCL, 1U);
    HAL_PinctrlSetPinMux(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_GPIO_IO08__LPI2C7_SDA, 1U);

    HAL_PinctrlSetPinCfg(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_GPIO_IO09__LPI2C7_SCL,
                         HAL_PINCTRL_PLATFORM_IOMUXC_PAD_DSE(15U) | HAL_PINCTRL_PLATFORM_IOMUXC_PAD_FSEL1(2U) |
                         HAL_PINCTRL_PLATFORM_IOMUXC_PAD_OD_MASK);
    HAL_PinctrlSetPinCfg(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_GPIO_IO08__LPI2C7_SDA,
                         HAL_PINCTRL_PLATFORM_IOMUXC_PAD_DSE(15U) | HAL_PINCTRL_PLATFORM_IOMUXC_PAD_FSEL1(2U) |
                         HAL_PINCTRL_PLATFORM_IOMUXC_PAD_OD_MASK);
}

/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
