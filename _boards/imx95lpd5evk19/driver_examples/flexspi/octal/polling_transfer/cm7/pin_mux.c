/*
 * Copyright 2023 NXP
 * All rights reserved.
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
product: Pins v13.0
processor: MIMX9352xxxxM
package_id: MIMX9352DVUXM
mcu_data: ksdk2_0
processor_version: 0.13.4
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
    BOARD_InitPins();
}

/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitPins:
- options: {callFromInitBoot: 'true', coreID: cm33}
- pin_list:
  - {pin_num: D20, peripheral: LPI2C2, signal: lpi2c_scl, pin_signal: I2C2_SCL, SION: ENABLED, HYS: DISABLED, OD: ENABLED, PD: DISABLED}
  - {pin_num: D21, peripheral: LPI2C2, signal: lpi2c_sda, pin_signal: I2C2_SDA, SION: ENABLED, OD: ENABLED, PD: DISABLED}
  - {pin_num: F21, peripheral: LPUART2, signal: lpuart_tx, pin_signal: UART2_TXD, OD: no_init, PD: DISABLED, PU: DISABLED, FSEL1: SlOW_SLEW_RATE}
  - {pin_num: F20, peripheral: LPUART2, signal: lpuart_rx, pin_signal: UART2_RXD, OD: DISABLED, PD: ENABLED, PU: DISABLED, FSEL1: SlOW_SLEW_RATE, DSE: NO_DRIVE}
  - {pin_num: V16, peripheral: FLEXSPI1, signal: flexspi_a_sclk, pin_signal: SD3_CLK, PD: DISABLED, FSEL1: SlOW_SLEW_RATE, DSE: X6}
  - {pin_num: T16, peripheral: FLEXSPI1, signal: 'flexspi_a_data, 00', pin_signal: SD3_DATA0, PD: DISABLED, FSEL1: SlOW_SLEW_RATE, DSE: X6}
  - {pin_num: V14, peripheral: FLEXSPI1, signal: 'flexspi_a_data, 01', pin_signal: SD3_DATA1, PD: DISABLED, FSEL1: SlOW_SLEW_RATE, DSE: X6}
  - {pin_num: U14, peripheral: FLEXSPI1, signal: 'flexspi_a_data, 02', pin_signal: SD3_DATA2, PD: DISABLED, FSEL1: SlOW_SLEW_RATE, DSE: X6}
  - {pin_num: T14, peripheral: FLEXSPI1, signal: 'flexspi_a_data, 03', pin_signal: SD3_DATA3, PD: DISABLED, FSEL1: SlOW_SLEW_RATE, DSE: X6}
  - {pin_num: U16, peripheral: USDHC3, signal: 'flexspi_a_ss_b, 0', pin_signal: SD3_CMD, PD: DISABLED, FSEL1: SlOW_SLEW_RATE, DSE: X6}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitPins(void) {                                /*!< Function assigned for the core: Cortex-M33[cm33] */
    HAL_PinctrlSetPinMux(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_GPIO_IO15__LPUART3_RX, 0U);
    HAL_PinctrlSetPinMux(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_GPIO_IO14__LPUART3_TX, 0U);

    HAL_PinctrlSetPinCfg(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_GPIO_IO15__LPUART3_RX,
                        HAL_PINCTRL_PLATFORM_IOMUXC_PAD_PD_MASK);
    HAL_PinctrlSetPinCfg(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_GPIO_IO14__LPUART3_TX,
                        HAL_PINCTRL_PLATFORM_IOMUXC_PAD_DSE(15U));

    HAL_PinctrlSetPinMux(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_XSPI1_SCLK__FLEXSPI1_A_SCLK, 0U);
    HAL_PinctrlSetPinCfg(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_XSPI1_SCLK__FLEXSPI1_A_SCLK,
                        HAL_PINCTRL_PLATFORM_IOMUXC_PAD_DSE(63U));
    HAL_PinctrlSetPinMux(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_XSPI1_SS0_B__FLEXSPI1_A_SS0_B, 0U);
    HAL_PinctrlSetPinCfg(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_XSPI1_SS0_B__FLEXSPI1_A_SS0_B,
                        HAL_PINCTRL_PLATFORM_IOMUXC_PAD_DSE(63U));
    HAL_PinctrlSetPinMux(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_XSPI1_SS1_B__FLEXSPI1_A_SS1_B, 0U);
    HAL_PinctrlSetPinCfg(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_XSPI1_SS1_B__FLEXSPI1_A_SS1_B,
                        HAL_PINCTRL_PLATFORM_IOMUXC_PAD_DSE(63U));
    HAL_PinctrlSetPinMux(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_XSPI1_DQS__FLEXSPI1_A_DQS, 0U);
    HAL_PinctrlSetPinCfg(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_XSPI1_DQS__FLEXSPI1_A_DQS,
                        HAL_PINCTRL_PLATFORM_IOMUXC_PAD_DSE(63U));
    HAL_PinctrlSetPinMux(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_XSPI1_DATA0__FLEXSPI1_A_DATA_BIT0, 0U);
    HAL_PinctrlSetPinCfg(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_XSPI1_DATA0__FLEXSPI1_A_DATA_BIT0,
                        HAL_PINCTRL_PLATFORM_IOMUXC_PAD_DSE(63U));
    HAL_PinctrlSetPinMux(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_XSPI1_DATA1__FLEXSPI1_A_DATA_BIT1, 0U);
    HAL_PinctrlSetPinCfg(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_XSPI1_DATA1__FLEXSPI1_A_DATA_BIT1,
                        HAL_PINCTRL_PLATFORM_IOMUXC_PAD_DSE(63U));
    HAL_PinctrlSetPinMux(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_XSPI1_DATA2__FLEXSPI1_A_DATA_BIT2, 0U);
    HAL_PinctrlSetPinCfg(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_XSPI1_DATA2__FLEXSPI1_A_DATA_BIT2,
                        HAL_PINCTRL_PLATFORM_IOMUXC_PAD_DSE(63U));
    HAL_PinctrlSetPinMux(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_XSPI1_DATA3__FLEXSPI1_A_DATA_BIT3, 0U);
    HAL_PinctrlSetPinCfg(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_XSPI1_DATA3__FLEXSPI1_A_DATA_BIT3,
                        HAL_PINCTRL_PLATFORM_IOMUXC_PAD_DSE(63U));
    HAL_PinctrlSetPinMux(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_XSPI1_DATA4__FLEXSPI1_A_DATA_BIT4, 0U);
    HAL_PinctrlSetPinCfg(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_XSPI1_DATA4__FLEXSPI1_A_DATA_BIT4,
                        HAL_PINCTRL_PLATFORM_IOMUXC_PAD_DSE(63U));
    HAL_PinctrlSetPinMux(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_XSPI1_DATA5__FLEXSPI1_A_DATA_BIT5, 0U);
    HAL_PinctrlSetPinCfg(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_XSPI1_DATA5__FLEXSPI1_A_DATA_BIT5,
                        HAL_PINCTRL_PLATFORM_IOMUXC_PAD_DSE(63U));
    HAL_PinctrlSetPinMux(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_XSPI1_DATA6__FLEXSPI1_A_DATA_BIT6, 0U);
    HAL_PinctrlSetPinCfg(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_XSPI1_DATA6__FLEXSPI1_A_DATA_BIT6,
                        HAL_PINCTRL_PLATFORM_IOMUXC_PAD_DSE(63U));
    HAL_PinctrlSetPinMux(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_XSPI1_DATA7__FLEXSPI1_A_DATA_BIT7, 0U);
    HAL_PinctrlSetPinCfg(HAL_PINCTRL_PLATFORM_IOMUXC_PAD_XSPI1_DATA7__FLEXSPI1_A_DATA_BIT7,
                        HAL_PINCTRL_PLATFORM_IOMUXC_PAD_DSE(63U));
}

/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
