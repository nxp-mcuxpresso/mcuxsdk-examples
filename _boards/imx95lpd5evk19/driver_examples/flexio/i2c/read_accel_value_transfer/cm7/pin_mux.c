/*
 * Copyright 2023 NXP
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
    BOARD_I2C_ConfigurePins();
    HAL_PinctrlSetPinMux(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_GPIO_IO15__LPUART3_RX, 0U);
    HAL_PinctrlSetPinMux(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_GPIO_IO14__LPUART3_TX, 0U);

    HAL_PinctrlSetPinCfg(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_GPIO_IO15__LPUART3_RX,
                        HAL_PINCTRL_PLATFORM_IOMUXC_PAD_PD_MASK);
    HAL_PinctrlSetPinCfg(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_GPIO_IO14__LPUART3_TX,
                        HAL_PINCTRL_PLATFORM_IOMUXC_PAD_DSE(15U));
}

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitPins(void)
{
    HAL_PinctrlSetPinMux(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_GPIO_IO02__FLEXIO1_FLEXIO_BIT2, 1U);
    HAL_PinctrlSetPinMux(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_GPIO_IO03__FLEXIO1_FLEXIO_BIT3, 1U);

    HAL_PinctrlSetPinCfg(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_GPIO_IO02__FLEXIO1_FLEXIO_BIT2,
                        HAL_PINCTRL_PLATFORM_IOMUXC_PAD_DSE(15U) | HAL_PINCTRL_PLATFORM_IOMUXC_PAD_FSEL1(2U) |
                        HAL_PINCTRL_PLATFORM_IOMUXC_PAD_OD_MASK);
    HAL_PinctrlSetPinCfg(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_GPIO_IO03__FLEXIO1_FLEXIO_BIT3,
                        HAL_PINCTRL_PLATFORM_IOMUXC_PAD_DSE(15U) | HAL_PINCTRL_PLATFORM_IOMUXC_PAD_FSEL1(2U) |
                        HAL_PINCTRL_PLATFORM_IOMUXC_PAD_OD_MASK);
}

void BOARD_I2C_ConfigurePins(void) 
{
    HAL_PinctrlSetPinMux(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_I2C1_SCL__LPI2C1_SCL, 1U);
    HAL_PinctrlSetPinMux(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_I2C1_SDA__LPI2C1_SDA, 1U);

    HAL_PinctrlSetPinCfg(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_I2C1_SCL__LPI2C1_SCL,
                        HAL_PINCTRL_PLATFORM_IOMUXC_PAD_DSE(15U) | HAL_PINCTRL_PLATFORM_IOMUXC_PAD_FSEL1(2U) |
                        HAL_PINCTRL_PLATFORM_IOMUXC_PAD_OD_MASK);
    HAL_PinctrlSetPinCfg(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_I2C1_SDA__LPI2C1_SDA,
                        HAL_PINCTRL_PLATFORM_IOMUXC_PAD_DSE(15U) | HAL_PINCTRL_PLATFORM_IOMUXC_PAD_FSEL1(2U) |
                        HAL_PINCTRL_PLATFORM_IOMUXC_PAD_OD_MASK);
}

