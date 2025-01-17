/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/***********************************************************************************************************************
 * This file was generated by the MCUXpresso Config Tools. Any manual edits made to this file
 * will be overwritten if the respective MCUXpresso Config Tools is used to update this file.
 **********************************************************************************************************************/

/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!GlobalInfo
product: Pins v12.0
processor: MIMX9352xxxxM
package_id: MIMX9352DVUXM
mcu_data: ksdk2_0
processor_version: 0.12.3
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

#include "pin_mux.h"

void LPUART1_InitPins(void) {                              /*!< Function assigned for the core: undefined[cm33] */
    HAL_PinctrlSetPinMux(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_UART1_RXD__LPUART1_RX, 0U);
    HAL_PinctrlSetPinMux(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_UART1_TXD__LPUART1_TX, 0U);

    HAL_PinctrlSetPinCfg(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_UART1_RXD__LPUART1_RX,
                        HAL_PINCTRL_PLATFORM_IOMUXC_PAD_PD_MASK);
    HAL_PinctrlSetPinCfg(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_UART1_TXD__LPUART1_TX,
                        HAL_PINCTRL_PLATFORM_IOMUXC_PAD_DSE(15U));
}

void LPUART2_InitPins(void) {                              /*!< Function assigned for the core: undefined[cm33] */
    HAL_PinctrlSetPinMux(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_UART2_RXD__LPUART2_RX, 0U);
    HAL_PinctrlSetPinMux(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_UART2_TXD__LPUART2_TX, 0U);

    HAL_PinctrlSetPinCfg(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_UART2_RXD__LPUART2_RX,
                        HAL_PINCTRL_PLATFORM_IOMUXC_PAD_PD_MASK);
    HAL_PinctrlSetPinCfg(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_UART2_TXD__LPUART2_TX,
                        HAL_PINCTRL_PLATFORM_IOMUXC_PAD_DSE(15U));
}

void LPUART3_InitPins(void) {                              /*!< Function assigned for the core: undefined[cm33] */
    HAL_PinctrlSetPinMux(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_GPIO_IO15__LPUART3_RX, 0U);
    HAL_PinctrlSetPinMux(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_GPIO_IO14__LPUART3_TX, 0U);

    HAL_PinctrlSetPinCfg(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_GPIO_IO15__LPUART3_RX,
                        HAL_PINCTRL_PLATFORM_IOMUXC_PAD_PD_MASK);
    HAL_PinctrlSetPinCfg(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_GPIO_IO14__LPUART3_TX,
                        HAL_PINCTRL_PLATFORM_IOMUXC_PAD_DSE(15U));
}

void LPUART1_DeinitPins(void) {                            /*!< Function assigned for the core: undefined[cm33] */
    HAL_PinctrlSetPinMux(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_UART1_RXD__GPIO1_IO_BIT4, 0U);
    HAL_PinctrlSetPinMux(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_UART1_TXD__GPIO1_IO_BIT5, 0U);
}

void LPUART2_DeinitPins(void) {                            /*!< Function assigned for the core: undefined[cm33] */
    HAL_PinctrlSetPinMux(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_UART2_RXD__GPIO1_IO_BIT6, 0U);
    HAL_PinctrlSetPinMux(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_UART2_TXD__GPIO1_IO_BIT7, 0U);
}

void LPUART3_DeinitPins(void) {                            /*!< Function assigned for the core: undefined[cm33] */
    HAL_PinctrlSetPinMux(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_GPIO_IO15__GPIO2_IO_BIT15, 0U);
    HAL_PinctrlSetPinMux(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_GPIO_IO14__GPIO2_IO_BIT14, 0U);
}
/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitBootPins
 * Description   : Calls initialization functions.
 *
 * END ****************************************************************************************************************/
void BOARD_InitBootPins(void)
{
}

/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitPins:
- options: {callFromInitBoot: 'true', coreID: cm33}
- pin_list: []
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitPins(void) {                                /*!< Function assigned for the core: undefined[cm33] */
}

/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
