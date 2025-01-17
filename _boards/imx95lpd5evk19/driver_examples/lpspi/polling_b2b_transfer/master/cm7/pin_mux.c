/*
 * Copyright 2023 NXP
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

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitBootPins
 * Description   : Calls initialization functions.
 *
 * END ****************************************************************************************************************/
void BOARD_InitBootPins(void)
{
    HAL_PinctrlSetPinMux(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_GPIO_IO15__LPUART3_RX, 0U);
    HAL_PinctrlSetPinMux(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_GPIO_IO14__LPUART3_TX, 0U);

    HAL_PinctrlSetPinCfg(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_GPIO_IO15__LPUART3_RX,
                        HAL_PINCTRL_PLATFORM_IOMUXC_PAD_PD_MASK);
    HAL_PinctrlSetPinCfg(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_GPIO_IO14__LPUART3_TX,
                        HAL_PINCTRL_PLATFORM_IOMUXC_PAD_DSE(15U));
    BOARD_InitPins();
}

/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitPins:
- options: {callFromInitBoot: 'true', coreID: cm33}
- pin_list:
  - {pin_num: F21, peripheral: LPUART2, signal: lpuart_tx, pin_signal: UART2_TXD, HYS: DISABLED, PD: DISABLED, FSEL1: SlOW_SLEW_RATE}
  - {pin_num: F20, peripheral: LPUART2, signal: lpuart_rx, pin_signal: UART2_RXD, HYS: DISABLED, PD: ENABLED, FSEL1: SlOW_SLEW_RATE, DSE: NO_DRIVE}
  - {pin_num: D20, peripheral: LPI2C2, signal: lpi2c_scl, pin_signal: I2C2_SCL, SION: ENABLED, OD: ENABLED, PD: DISABLED}
  - {pin_num: D21, peripheral: LPI2C2, signal: lpi2c_sda, pin_signal: I2C2_SDA, SION: ENABLED, OD: ENABLED, PD: DISABLED}
  - {pin_num: M20, peripheral: LPSPI3, signal: 'lpspi_pcs, 0', pin_signal: GPIO_IO08, PD: ENABLED}
  - {pin_num: N18, peripheral: LPSPI3, signal: lpspi_sck, pin_signal: GPIO_IO11, PD: ENABLED}
  - {pin_num: M21, peripheral: LPSPI3, signal: lpspi_sin, pin_signal: GPIO_IO09, PD: ENABLED}
  - {pin_num: N17, peripheral: LPSPI3, signal: lpspi_sout, pin_signal: GPIO_IO10, PD: ENABLED}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitPins(void) {                                /*!< Function assigned for the core: undefined[cm33] */
    HAL_PinctrlSetPinMux(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_SAI1_TXFS__LPSPI1_PCS0, 0U);
    HAL_PinctrlSetPinMux(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_SAI1_TXC__LPSPI1_SIN, 0U);
    HAL_PinctrlSetPinMux(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_SAI1_RXD0__LPSPI1_SOUT, 0U);
    HAL_PinctrlSetPinMux(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_SAI1_TXD0__LPSPI1_SCK, 0U);

    HAL_PinctrlSetPinCfg(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_SAI1_TXFS__LPSPI1_PCS0, 
                        HAL_PINCTRL_PLATFORM_IOMUXC_PAD_DSE(15U) |
                        HAL_PINCTRL_PLATFORM_IOMUXC_PAD_FSEL1(2U) |
                        HAL_PINCTRL_PLATFORM_IOMUXC_PAD_PD_MASK);
    HAL_PinctrlSetPinCfg(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_SAI1_TXC__LPSPI1_SIN, 
                        HAL_PINCTRL_PLATFORM_IOMUXC_PAD_DSE(15U) |
                        HAL_PINCTRL_PLATFORM_IOMUXC_PAD_FSEL1(2U) |
                        HAL_PINCTRL_PLATFORM_IOMUXC_PAD_PD_MASK);
    HAL_PinctrlSetPinCfg(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_SAI1_RXD0__LPSPI1_SOUT, 
                        HAL_PINCTRL_PLATFORM_IOMUXC_PAD_DSE(15U) |
                        HAL_PINCTRL_PLATFORM_IOMUXC_PAD_FSEL1(2U) |
                        HAL_PINCTRL_PLATFORM_IOMUXC_PAD_PD_MASK);
    HAL_PinctrlSetPinCfg(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_SAI1_TXD0__LPSPI1_SCK, 
                        HAL_PINCTRL_PLATFORM_IOMUXC_PAD_DSE(15U) |
                        HAL_PINCTRL_PLATFORM_IOMUXC_PAD_FSEL1(2U) |
                        HAL_PINCTRL_PLATFORM_IOMUXC_PAD_PD_MASK);
}

/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
