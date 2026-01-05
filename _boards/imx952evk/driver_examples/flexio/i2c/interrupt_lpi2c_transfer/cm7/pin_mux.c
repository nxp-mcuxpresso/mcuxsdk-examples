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
    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO15__LPUART3_RX, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO14__LPUART3_TX, 0U);

    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO15__LPUART3_RX,
                        IOMUXC_PAD_PD_MASK);
    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO14__LPUART3_TX,
                        IOMUXC_PAD_DSE(15U)); 	
}

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitPins(void)
{
    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO09__FLEXIO1_FLEXIO_9, 1U);
    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO08__FLEXIO1_FLEXIO_8, 1U);

    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO09__FLEXIO1_FLEXIO_9,
                        IOMUXC_PAD_DSE(15U) | IOMUXC_PAD_FSEL1(2U) |
                        IOMUXC_PAD_OD_MASK);
    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO08__FLEXIO1_FLEXIO_8,
                        IOMUXC_PAD_DSE(15U) | IOMUXC_PAD_FSEL1(2U) |
                        IOMUXC_PAD_OD_MASK);
}

void BOARD_I2C_ConfigurePins(void) 
{
    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO03__LPI2C6_SCL, 1U);
    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO02__LPI2C6_SDA, 1U);

    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO03__LPI2C6_SCL,
                        IOMUXC_PAD_DSE(15U) | IOMUXC_PAD_FSEL1(2U) |
                        IOMUXC_PAD_OD_MASK);
    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO02__LPI2C6_SDA,
                        IOMUXC_PAD_DSE(15U) | IOMUXC_PAD_FSEL1(2U) |
                        IOMUXC_PAD_OD_MASK);
}

