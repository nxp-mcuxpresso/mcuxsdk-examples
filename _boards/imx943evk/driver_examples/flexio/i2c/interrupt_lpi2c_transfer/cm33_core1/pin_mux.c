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
    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO30__FLEXIO1_3_1_FLEXIO4, 1U);
    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO30__FLEXIO1_3_1_FLEXIO4,
                        IOMUXC_PAD_DSE(7U) |
                        IOMUXC_PAD_FSEL1(2U));
    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO31__FLEXIO1_3_1_FLEXIO5, 1U);
    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO31__FLEXIO1_3_1_FLEXIO5,
                        IOMUXC_PAD_DSE(7U) |
                        IOMUXC_PAD_FSEL1(2U));
}

void BOARD_I2C_ConfigurePins(void) 
{
    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO19__LPI2C4_SCL, 1U);
    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO18__LPI2C4_SDA, 1U);

    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO19__LPI2C4_SCL,
                        IOMUXC_PAD_DSE(7U) | IOMUXC_PAD_FSEL1(2U) |
                        IOMUXC_PAD_OD_MASK);
    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO18__LPI2C4_SDA,
                        IOMUXC_PAD_DSE(7U) | IOMUXC_PAD_FSEL1(2U) |
                        IOMUXC_PAD_OD_MASK);
}

