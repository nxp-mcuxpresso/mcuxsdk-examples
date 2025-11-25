/*
 * Copyright 2025 NXP
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
}

void BOARD_I2C_ConfigurePins(void) 
{
    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO19__LPI2C4_SCL, 1U);
    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO18__LPI2C4_SDA, 1U);

    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO19__LPI2C4_SCL,
                        IOMUXC_PAD_DSE(7U) | IOMUXC_PAD_FSEL1(2U) |
                        IOMUXC_PAD_PD_MASK);
    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO18__LPI2C4_SDA,
                        IOMUXC_PAD_DSE(7U) | IOMUXC_PAD_FSEL1(2U) |
                        IOMUXC_PAD_PD_MASK);

    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO28__LPI2C6_SCL, 1U);
    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO29__LPI2C6_SDA, 1U);

    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO28__LPI2C6_SCL,
                        IOMUXC_PAD_DSE(7U) | IOMUXC_PAD_FSEL1(2U) |
                        IOMUXC_PAD_PD_MASK);
    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO29__LPI2C6_SDA,
                        IOMUXC_PAD_DSE(7U) | IOMUXC_PAD_FSEL1(2U) |
                        IOMUXC_PAD_PD_MASK);
}

