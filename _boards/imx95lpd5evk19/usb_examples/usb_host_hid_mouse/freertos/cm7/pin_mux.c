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
