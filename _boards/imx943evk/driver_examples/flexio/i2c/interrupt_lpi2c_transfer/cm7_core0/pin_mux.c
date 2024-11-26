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
}

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitPins(void)
{
    HAL_PinctrlSetPinMux(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_GPIO_IO28__FLEXIO1_3_1_FLEXIO2, 1U);
    HAL_PinctrlSetPinMux(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_GPIO_IO29__FLEXIO1_3_1_FLEXIO3, 1U);

    HAL_PinctrlSetPinCfg(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_GPIO_IO28__FLEXIO1_3_1_FLEXIO2,
                        HAL_PINCTRL_PLATFORM_IOMUXC_PAD_DSE(15U) | HAL_PINCTRL_PLATFORM_IOMUXC_PAD_FSEL1(2U) |
                        HAL_PINCTRL_PLATFORM_IOMUXC_PAD_OD_MASK);
    HAL_PinctrlSetPinCfg(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_GPIO_IO29__FLEXIO1_3_1_FLEXIO3,
                        HAL_PINCTRL_PLATFORM_IOMUXC_PAD_DSE(15U) | HAL_PINCTRL_PLATFORM_IOMUXC_PAD_FSEL1(2U) |
                        HAL_PINCTRL_PLATFORM_IOMUXC_PAD_OD_MASK);
}

void BOARD_I2C_ConfigurePins(void) 
{
    HAL_PinctrlSetPinMux(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_GPIO_IO19__LPI2C4_SCL, 1U);
    HAL_PinctrlSetPinMux(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_GPIO_IO18__LPI2C4_SDA, 1U);

    HAL_PinctrlSetPinCfg(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_GPIO_IO19__LPI2C4_SCL,
                        HAL_PINCTRL_PLATFORM_IOMUXC_PAD_DSE(15U) | HAL_PINCTRL_PLATFORM_IOMUXC_PAD_FSEL1(2U) |
                        HAL_PINCTRL_PLATFORM_IOMUXC_PAD_OD_MASK);
    HAL_PinctrlSetPinCfg(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_GPIO_IO18__LPI2C4_SDA,
                        HAL_PINCTRL_PLATFORM_IOMUXC_PAD_DSE(15U) | HAL_PINCTRL_PLATFORM_IOMUXC_PAD_FSEL1(2U) |
                        HAL_PINCTRL_PLATFORM_IOMUXC_PAD_OD_MASK);
}

