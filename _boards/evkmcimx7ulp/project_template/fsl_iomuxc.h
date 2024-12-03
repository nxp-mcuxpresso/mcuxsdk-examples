/*
 * Copyright 2015 - 2016 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _FSL_IOMUXC_H_
#define _FSL_IOMUXC_H_

#include "fsl_common.h"

/*!
 * @addtogroup iomuxc_driver
 * @{
 */

/*! @file */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @name Driver version */
/*@{*/
/*! @brief IOMUXC driver version 2.0.0. */
#define FSL_IOMUXC_DRIVER_VERSION (MAKE_VERSION(2, 0, 0))
/*@}*/

/*! @name Pin function ID */
/*@{*/
/*! @brief The pin function ID is a tuple of <muxRegister muxMode inputRegister inputDaisy configRegister> */
#define IOMUXC_PTA0_CMP0_IN1_3V                             0x4103D000, 0x0, 0x00000000, 0x0, 0x4103D000
#define IOMUXC_PTA0_GPIOA0                                  0x4103D000, 0x1, 0x00000000, 0x0, 0x4103D000
#define IOMUXC_PTA0_LPSPI0_PCS1                             0x4103D000, 0x3, 0x4103D104, 0x2, 0x4103D000
#define IOMUXC_PTA0_LPUART0_CTS_b                           0x4103D000, 0x4, 0x4103D1F8, 0x2, 0x4103D000
#define IOMUXC_PTA0_LPI2C0_SCL                              0x4103D000, 0x5, 0x4103D17C, 0x2, 0x4103D000
#define IOMUXC_PTA0_TPM0_CLKIN                              0x4103D000, 0x6, 0x4103D1A8, 0x2, 0x4103D000
#define IOMUXC_PTA0_I2S0_RX_BCLK                            0x4103D000, 0x7, 0x4103D1B8, 0x2, 0x4103D000
#define IOMUXC_PTA0_LLWU0_P0                                0x4103D000, 0xD, 0x00000000, 0x0, 0x4103D000
#define IOMUXC_PTA0_BT0_CFG0                                0x4103D000, 0x0, 0x00000000, 0x0, 0x4103D000
#define IOMUXC_PTA1_CMP0_IN2_3V                             0x4103D004, 0x0, 0x00000000, 0x0, 0x4103D004
#define IOMUXC_PTA1_GPIOA1                                  0x4103D004, 0x1, 0x00000000, 0x0, 0x4103D004
#define IOMUXC_PTA1_LPSPI0_PCS2                             0x4103D004, 0x3, 0x4103D108, 0x1, 0x4103D004
#define IOMUXC_PTA1_LPUART0_RTS_b                           0x4103D004, 0x4, 0x00000000, 0x0, 0x4103D004
#define IOMUXC_PTA1_LPI2C0_SDA                              0x4103D004, 0x5, 0x4103D180, 0x1, 0x4103D004
#define IOMUXC_PTA1_TPM0_CH0                                0x4103D004, 0x6, 0x4103D138, 0x1, 0x4103D004
#define IOMUXC_PTA1_I2S0_RX_FS                              0x4103D004, 0x7, 0x4103D1BC, 0x1, 0x4103D004
#define IOMUXC_PTA1_BT0_CFG1                                0x4103D004, 0x0, 0x00000000, 0x0, 0x4103D004
#define IOMUXC_PTA2_CMP1_IN2_3V                             0x4103D008, 0x0, 0x00000000, 0x0, 0x4103D008
#define IOMUXC_PTA2_GPIOA2                                  0x4103D008, 0x1, 0x00000000, 0x0, 0x4103D008
#define IOMUXC_PTA2_LPSPI0_PCS3                             0x4103D008, 0x3, 0x4103D10C, 0x1, 0x4103D008
#define IOMUXC_PTA2_LPUART0_TX                              0x4103D008, 0x4, 0x4103D200, 0x1, 0x4103D008
#define IOMUXC_PTA2_LPI2C0_HREQ                             0x4103D008, 0x5, 0x4103D178, 0x1, 0x4103D008
#define IOMUXC_PTA2_TPM0_CH1                                0x4103D008, 0x6, 0x4103D13C, 0x1, 0x4103D008
#define IOMUXC_PTA2_I2S0_RXD0                               0x4103D008, 0x7, 0x4103D1DC, 0x1, 0x4103D008
#define IOMUXC_PTA2_BT0_CFG2                                0x4103D008, 0x0, 0x00000000, 0x0, 0x4103D008
#define IOMUXC_PTA3_CMP1_IN4_3V                             0x4103D00C, 0x0, 0x00000000, 0x0, 0x4103D00C
#define IOMUXC_PTA3_GPIOA3                                  0x4103D00C, 0x1, 0x00000000, 0x0, 0x4103D00C
#define IOMUXC_PTA3_LPSPI0_PCS0                             0x4103D00C, 0x3, 0x4103D100, 0x1, 0x4103D00C
#define IOMUXC_PTA3_LPUART0_RX                              0x4103D00C, 0x4, 0x4103D1FC, 0x1, 0x4103D00C
#define IOMUXC_PTA3_TPM0_CH2                                0x4103D00C, 0x6, 0x4103D140, 0x1, 0x4103D00C
#define IOMUXC_PTA3_I2S0_RXD1                               0x4103D00C, 0x7, 0x4103D1E0, 0x1, 0x4103D00C
#define IOMUXC_PTA3_CMP0_OUT                                0x4103D00C, 0xB, 0x00000000, 0x0, 0x4103D00C
#define IOMUXC_PTA3_LLWU0_P1                                0x4103D00C, 0xD, 0x00000000, 0x0, 0x4103D00C
#define IOMUXC_PTA3_BT0_CFG3                                0x4103D00C, 0x0, 0x00000000, 0x0, 0x4103D00C
#define IOMUXC_PTA4_ADC1_CH3A                               0x4103D010, 0x0, 0x00000000, 0x0, 0x4103D010
#define IOMUXC_PTA4_GPIOA4                                  0x4103D010, 0x1, 0x00000000, 0x0, 0x4103D010
#define IOMUXC_PTA4_LPSPI0_SIN                              0x4103D010, 0x3, 0x4103D114, 0x1, 0x4103D010
#define IOMUXC_PTA4_LPUART1_CTS_b                           0x4103D010, 0x4, 0x4103D204, 0x1, 0x4103D010
#define IOMUXC_PTA4_LPI2C1_SCL                              0x4103D010, 0x5, 0x4103D188, 0x1, 0x4103D010
#define IOMUXC_PTA4_TPM0_CH3                                0x4103D010, 0x6, 0x4103D144, 0x1, 0x4103D010
#define IOMUXC_PTA4_I2S0_MCLK                               0x4103D010, 0x7, 0x4103D1B4, 0x1, 0x4103D010
#define IOMUXC_PTA4_BT0_CFG4                                0x4103D010, 0x0, 0x00000000, 0x0, 0x4103D010
#define IOMUXC_PTA5_ADC1_CH3B                               0x4103D014, 0x0, 0x00000000, 0x0, 0x4103D014
#define IOMUXC_PTA5_GPIOA5                                  0x4103D014, 0x1, 0x00000000, 0x0, 0x4103D014
#define IOMUXC_PTA5_LPSPI0_SOUT                             0x4103D014, 0x3, 0x4103D118, 0x1, 0x4103D014
#define IOMUXC_PTA5_LPUART1_RTS_b                           0x4103D014, 0x4, 0x00000000, 0x0, 0x4103D014
#define IOMUXC_PTA5_LPI2C1_SDA                              0x4103D014, 0x5, 0x4103D18C, 0x1, 0x4103D014
#define IOMUXC_PTA5_TPM0_CH4                                0x4103D014, 0x6, 0x4103D148, 0x1, 0x4103D014
#define IOMUXC_PTA5_I2S0_TX_BCLK                            0x4103D014, 0x7, 0x4103D1C0, 0x1, 0x4103D014
#define IOMUXC_PTA5_BT0_CFG5                                0x4103D014, 0x0, 0x00000000, 0x0, 0x4103D014
#define IOMUXC_PTA6_ADC1_CH4A_5A_6A_7A_8A                   0x4103D018, 0x0, 0x00000000, 0x0, 0x4103D018
#define IOMUXC_PTA6_GPIOA6                                  0x4103D018, 0x1, 0x00000000, 0x0, 0x4103D018
#define IOMUXC_PTA6_LPSPI0_SCK                              0x4103D018, 0x3, 0x4103D110, 0x1, 0x4103D018
#define IOMUXC_PTA6_LPUART1_TX                              0x4103D018, 0x4, 0x4103D20C, 0x1, 0x4103D018
#define IOMUXC_PTA6_LPI2C1_HREQ                             0x4103D018, 0x5, 0x4103D184, 0x1, 0x4103D018
#define IOMUXC_PTA6_TPM0_CH5                                0x4103D018, 0x6, 0x4103D14C, 0x1, 0x4103D018
#define IOMUXC_PTA6_I2S0_TX_FS                              0x4103D018, 0x7, 0x4103D1C4, 0x1, 0x4103D018
#define IOMUXC_PTA6_BT0_CFG6                                0x4103D018, 0x0, 0x00000000, 0x0, 0x4103D018
#define IOMUXC_PTA7_ADC1_CH4B_5B_6B_7B_8B                   0x4103D01C, 0x0, 0x00000000, 0x0, 0x4103D01C
#define IOMUXC_PTA7_GPIOA7                                  0x4103D01C, 0x1, 0x00000000, 0x0, 0x4103D01C
#define IOMUXC_PTA7_LPUART1_RX                              0x4103D01C, 0x4, 0x4103D208, 0x1, 0x4103D01C
#define IOMUXC_PTA7_TPM1_CH1                                0x4103D01C, 0x6, 0x4103D154, 0x1, 0x4103D01C
#define IOMUXC_PTA7_I2S0_TXD0                               0x4103D01C, 0x7, 0x00000000, 0x0, 0x4103D01C
#define IOMUXC_PTA7_BT0_CFG7                                0x4103D01C, 0x0, 0x00000000, 0x0, 0x4103D01C
#define IOMUXC_PTA8_ADC1_CH4A_5A_6A_7A_8A                   0x4103D020, 0x0, 0x00000000, 0x0, 0x4103D020
#define IOMUXC_PTA8_GPIOA8                                  0x4103D020, 0x1, 0x00000000, 0x0, 0x4103D020
#define IOMUXC_PTA8_LPSPI1_PCS1                             0x4103D020, 0x3, 0x4103D120, 0x1, 0x4103D020
#define IOMUXC_PTA8_LPUART2_CTS_b                           0x4103D020, 0x4, 0x4103D210, 0x1, 0x4103D020
#define IOMUXC_PTA8_LPI2C2_SCL                              0x4103D020, 0x5, 0x4103D194, 0x1, 0x4103D020
#define IOMUXC_PTA8_TPM1_CLKIN                              0x4103D020, 0x6, 0x4103D1AC, 0x1, 0x4103D020
#define IOMUXC_PTA8_I2S0_TXD1                               0x4103D020, 0x7, 0x00000000, 0x0, 0x4103D020
#define IOMUXC_PTA8_BT0_CFG8                                0x4103D020, 0x0, 0x00000000, 0x0, 0x4103D020
#define IOMUXC_PTA9_ADC1_CH4B_5B_6B_7B_8B                   0x4103D024, 0x0, 0x00000000, 0x0, 0x4103D024
#define IOMUXC_PTA9_GPIOA9                                  0x4103D024, 0x1, 0x00000000, 0x0, 0x4103D024
#define IOMUXC_PTA9_LPSPI1_PCS2                             0x4103D024, 0x3, 0x4103D124, 0x1, 0x4103D024
#define IOMUXC_PTA9_LPUART2_RTS_b                           0x4103D024, 0x4, 0x00000000, 0x0, 0x4103D024
#define IOMUXC_PTA9_LPI2C2_SDA                              0x4103D024, 0x5, 0x4103D198, 0x1, 0x4103D024
#define IOMUXC_PTA9_TPM1_CH0                                0x4103D024, 0x6, 0x4103D150, 0x1, 0x4103D024
#define IOMUXC_PTA9_NMI0_b                                  0x4103D024, 0xB, 0x00000000, 0x0, 0x4103D024
#define IOMUXC_PTA9_BT0_CFG9                                0x4103D024, 0x0, 0x00000000, 0x0, 0x4103D024
#define IOMUXC_PTA10_ADC1_CH4A_5A_6A_7A_8A                  0x4103D028, 0x0, 0x00000000, 0x0, 0x4103D028
#define IOMUXC_PTA10_GPIOA10                                0x4103D028, 0x1, 0x00000000, 0x0, 0x4103D028
#define IOMUXC_PTA10_LPSPI1_PCS3                            0x4103D028, 0x3, 0x4103D128, 0x1, 0x4103D028
#define IOMUXC_PTA10_LPUART2_TX                             0x4103D028, 0x4, 0x4103D218, 0x1, 0x4103D028
#define IOMUXC_PTA10_LPI2C2_HREQ                            0x4103D028, 0x5, 0x4103D190, 0x1, 0x4103D028
#define IOMUXC_PTA10_TPM2_CLKIN                             0x4103D028, 0x6, 0x4103D1F4, 0x1, 0x4103D028
#define IOMUXC_PTA10_I2S0_RX_BCLK                           0x4103D028, 0x7, 0x4103D1B8, 0x1, 0x4103D028
#define IOMUXC_PTA10_BT0_CFG10                              0x4103D028, 0x0, 0x00000000, 0x0, 0x4103D028
#define IOMUXC_PTA11_ADC1_CH4B_5B_6B_7B_8B                  0x4103D02C, 0x0, 0x00000000, 0x0, 0x4103D02C
#define IOMUXC_PTA11_GPIOA11                                0x4103D02C, 0x1, 0x00000000, 0x0, 0x4103D02C
#define IOMUXC_PTA11_LPUART2_RX                             0x4103D02C, 0x4, 0x4103D214, 0x1, 0x4103D02C
#define IOMUXC_PTA11_TPM2_CH0                               0x4103D02C, 0x6, 0x4103D158, 0x1, 0x4103D02C
#define IOMUXC_PTA11_I2S0_RX_FS                             0x4103D02C, 0x7, 0x4103D1BC, 0x2, 0x4103D02C
#define IOMUXC_PTA11_BT0_CFG11                              0x4103D02C, 0x0, 0x00000000, 0x0, 0x4103D02C
#define IOMUXC_PTA12_ADC1_CH4A_5A_6A_7A_8A                  0x4103D030, 0x0, 0x00000000, 0x0, 0x4103D030
#define IOMUXC_PTA12_GPIOA12                                0x4103D030, 0x1, 0x00000000, 0x0, 0x4103D030
#define IOMUXC_PTA12_LPSPI1_SIN                             0x4103D030, 0x3, 0x4103D130, 0x1, 0x4103D030
#define IOMUXC_PTA12_LPUART3_CTS_b                          0x4103D030, 0x4, 0x4103D21C, 0x1, 0x4103D030
#define IOMUXC_PTA12_LPI2C3_SCL                             0x4103D030, 0x5, 0x4103D1A0, 0x1, 0x4103D030
#define IOMUXC_PTA12_TPM2_CH1                               0x4103D030, 0x6, 0x4103D15C, 0x1, 0x4103D030
#define IOMUXC_PTA12_I2S0_RXD0                              0x4103D030, 0x7, 0x4103D1DC, 0x2, 0x4103D030
#define IOMUXC_PTA12_BT0_CFG12                              0x4103D030, 0x0, 0x00000000, 0x0, 0x4103D030
#define IOMUXC_PTA13_ADC1_CH4B_5B_6B_7B_8B                  0x4103D034, 0x0, 0x00000000, 0x0, 0x4103D034
#define IOMUXC_PTA13_GPIOA13                                0x4103D034, 0x1, 0x00000000, 0x0, 0x4103D034
#define IOMUXC_PTA13_LPSPI1_SOUT                            0x4103D034, 0x3, 0x4103D134, 0x2, 0x4103D034
#define IOMUXC_PTA13_LPUART3_RTS_b                          0x4103D034, 0x4, 0x00000000, 0x0, 0x4103D034
#define IOMUXC_PTA13_LPI2C3_SDA                             0x4103D034, 0x5, 0x4103D1A4, 0x2, 0x4103D034
#define IOMUXC_PTA13_TPM3_CLKIN                             0x4103D034, 0x6, 0x4103D1B0, 0x1, 0x4103D034
#define IOMUXC_PTA13_I2S0_RXD1                              0x4103D034, 0x7, 0x4103D1E0, 0x2, 0x4103D034
#define IOMUXC_PTA13_CMP0_OUT                               0x4103D034, 0xB, 0x00000000, 0x0, 0x4103D034
#define IOMUXC_PTA13_LLWU0_P2                               0x4103D034, 0xD, 0x00000000, 0x0, 0x4103D034
#define IOMUXC_PTA13_BT0_CFG13                              0x4103D034, 0x0, 0x00000000, 0x0, 0x4103D034
#define IOMUXC_PTA14_ADC1_CH4A_5A_6A_7A_8A                  0x4103D038, 0x0, 0x00000000, 0x0, 0x4103D038
#define IOMUXC_PTA14_GPIOA14                                0x4103D038, 0x1, 0x00000000, 0x0, 0x4103D038
#define IOMUXC_PTA14_LPSPI1_SCK                             0x4103D038, 0x3, 0x4103D12C, 0x2, 0x4103D038
#define IOMUXC_PTA14_LPUART3_TX                             0x4103D038, 0x4, 0x4103D224, 0x2, 0x4103D038
#define IOMUXC_PTA14_LPI2C3_HREQ                            0x4103D038, 0x5, 0x4103D19C, 0x2, 0x4103D038
#define IOMUXC_PTA14_TPM3_CH0                               0x4103D038, 0x6, 0x4103D160, 0x1, 0x4103D038
#define IOMUXC_PTA14_I2S0_MCLK                              0x4103D038, 0x7, 0x4103D1B4, 0x2, 0x4103D038
#define IOMUXC_PTA14_LLWU0_P3                               0x4103D038, 0xD, 0x00000000, 0x0, 0x4103D038
#define IOMUXC_PTA14_BT0_CFG14                              0x4103D038, 0x0, 0x00000000, 0x0, 0x4103D038
#define IOMUXC_PTA15_ADC1_CH4B_5B_6B_7B_8B                  0x4103D03C, 0x0, 0x00000000, 0x0, 0x4103D03C
#define IOMUXC_PTA15_GPIOA15                                0x4103D03C, 0x1, 0x00000000, 0x0, 0x4103D03C
#define IOMUXC_PTA15_LPSPI1_PCS0                            0x4103D03C, 0x3, 0x4103D11C, 0x1, 0x4103D03C
#define IOMUXC_PTA15_LPUART3_RX                             0x4103D03C, 0x4, 0x4103D220, 0x1, 0x4103D03C
#define IOMUXC_PTA15_TPM3_CH1                               0x4103D03C, 0x6, 0x4103D164, 0x1, 0x4103D03C
#define IOMUXC_PTA15_I2S0_TX_BCLK                           0x4103D03C, 0x7, 0x4103D1C0, 0x2, 0x4103D03C
#define IOMUXC_PTA15_BT0_CFG15                              0x4103D03C, 0x0, 0x00000000, 0x0, 0x4103D03C
#define IOMUXC_PTA16_CMP1_IN5_3V                            0x4103D040, 0x0, 0x00000000, 0x0, 0x4103D040
#define IOMUXC_PTA16_GPIOA16                                0x4103D040, 0x1, 0x00000000, 0x0, 0x4103D040
#define IOMUXC_PTA16_FXIO0_D0                               0x4103D040, 0x2, 0x00000000, 0x0, 0x4103D040
#define IOMUXC_PTA16_LPSPI0_SOUT                            0x4103D040, 0x3, 0x4103D118, 0x2, 0x4103D040
#define IOMUXC_PTA16_LPUART0_CTS_b                          0x4103D040, 0x4, 0x4103D1F8, 0x1, 0x4103D040
#define IOMUXC_PTA16_LPI2C0_SCL                             0x4103D040, 0x5, 0x4103D17C, 0x1, 0x4103D040
#define IOMUXC_PTA16_TPM3_CH2                               0x4103D040, 0x6, 0x4103D168, 0x1, 0x4103D040
#define IOMUXC_PTA16_I2S0_TX_FS                             0x4103D040, 0x7, 0x4103D1C4, 0x2, 0x4103D040
#define IOMUXC_PTA17_CMP1_IN6_3V                            0x4103D044, 0x0, 0x00000000, 0x0, 0x4103D044
#define IOMUXC_PTA17_GPIOA17                                0x4103D044, 0x1, 0x00000000, 0x0, 0x4103D044
#define IOMUXC_PTA17_FXIO0_D1                               0x4103D044, 0x2, 0x00000000, 0x0, 0x4103D044
#define IOMUXC_PTA17_LPSPI0_SCK                             0x4103D044, 0x3, 0x4103D110, 0x2, 0x4103D044
#define IOMUXC_PTA17_LPUART0_RTS_b                          0x4103D044, 0x4, 0x00000000, 0x0, 0x4103D044
#define IOMUXC_PTA17_LPI2C0_SDA                             0x4103D044, 0x5, 0x4103D180, 0x2, 0x4103D044
#define IOMUXC_PTA17_TPM3_CH3                               0x4103D044, 0x6, 0x4103D16C, 0x1, 0x4103D044
#define IOMUXC_PTA17_I2S0_TXD0                              0x4103D044, 0x7, 0x00000000, 0x0, 0x4103D044
#define IOMUXC_PTA18_CMP1_IN1_3V                            0x4103D048, 0x0, 0x00000000, 0x0, 0x4103D048
#define IOMUXC_PTA18_GPIOA18                                0x4103D048, 0x1, 0x00000000, 0x0, 0x4103D048
#define IOMUXC_PTA18_FXIO0_D2                               0x4103D048, 0x2, 0x00000000, 0x0, 0x4103D048
#define IOMUXC_PTA18_LPSPI0_PCS0                            0x4103D048, 0x3, 0x4103D100, 0x2, 0x4103D048
#define IOMUXC_PTA18_LPUART0_TX                             0x4103D048, 0x4, 0x4103D200, 0x2, 0x4103D048
#define IOMUXC_PTA18_LPI2C0_HREQ                            0x4103D048, 0x5, 0x4103D178, 0x2, 0x4103D048
#define IOMUXC_PTA18_TPM3_CH4                               0x4103D048, 0x6, 0x4103D170, 0x1, 0x4103D048
#define IOMUXC_PTA18_I2S0_TXD1                              0x4103D048, 0x7, 0x00000000, 0x0, 0x4103D048
#define IOMUXC_PTA18_LLWU0_P4                               0x4103D048, 0xD, 0x00000000, 0x0, 0x4103D048
#define IOMUXC_PTA19_CMP1_IN3_3V                            0x4103D04C, 0x0, 0x00000000, 0x0, 0x4103D04C
#define IOMUXC_PTA19_GPIOA19                                0x4103D04C, 0x1, 0x00000000, 0x0, 0x4103D04C
#define IOMUXC_PTA19_FXIO0_D3                               0x4103D04C, 0x2, 0x00000000, 0x0, 0x4103D04C
#define IOMUXC_PTA19_LPUART0_RX                             0x4103D04C, 0x4, 0x4103D1FC, 0x2, 0x4103D04C
#define IOMUXC_PTA19_TPM3_CH5                               0x4103D04C, 0x6, 0x4103D174, 0x1, 0x4103D04C
#define IOMUXC_PTA19_I2S1_RX_BCLK                           0x4103D04C, 0x7, 0x4103D1CC, 0x1, 0x4103D04C
#define IOMUXC_PTA19_LPTMR0_ALT3                            0x4103D04C, 0xB, 0x00000000, 0x0, 0x4103D04C
#define IOMUXC_PTA19_LLWU0_P5                               0x4103D04C, 0xD, 0x00000000, 0x0, 0x4103D04C
#define IOMUXC_PTA20_ADC0_CH8A_9A_10A                       0x4103D050, 0x0, 0x00000000, 0x0, 0x4103D050
#define IOMUXC_PTA20_GPIOA20                                0x4103D050, 0x1, 0x00000000, 0x0, 0x4103D050
#define IOMUXC_PTA20_FXIO0_D4                               0x4103D050, 0x2, 0x00000000, 0x0, 0x4103D050
#define IOMUXC_PTA20_LPSPI0_SIN                             0x4103D050, 0x3, 0x4103D114, 0x2, 0x4103D050
#define IOMUXC_PTA20_LPUART1_CTS_b                          0x4103D050, 0x4, 0x4103D204, 0x2, 0x4103D050
#define IOMUXC_PTA20_LPI2C1_SCL                             0x4103D050, 0x5, 0x4103D188, 0x2, 0x4103D050
#define IOMUXC_PTA20_TPM0_CLKIN                             0x4103D050, 0x6, 0x4103D1A8, 0x1, 0x4103D050
#define IOMUXC_PTA20_I2S1_RX_FS                             0x4103D050, 0x7, 0x4103D1D0, 0x1, 0x4103D050
#define IOMUXC_PTA21_ADC0_CH8B_9B_10B                       0x4103D054, 0x0, 0x00000000, 0x0, 0x4103D054
#define IOMUXC_PTA21_GPIOA21                                0x4103D054, 0x1, 0x00000000, 0x0, 0x4103D054
#define IOMUXC_PTA21_FXIO0_D5                               0x4103D054, 0x2, 0x00000000, 0x0, 0x4103D054
#define IOMUXC_PTA21_LPSPI0_PCS1                            0x4103D054, 0x3, 0x4103D104, 0x1, 0x4103D054
#define IOMUXC_PTA21_LPUART1_RTS_b                          0x4103D054, 0x4, 0x00000000, 0x0, 0x4103D054
#define IOMUXC_PTA21_LPI2C1_SDA                             0x4103D054, 0x5, 0x4103D18C, 0x2, 0x4103D054
#define IOMUXC_PTA21_TPM0_CH0                               0x4103D054, 0x6, 0x4103D138, 0x2, 0x4103D054
#define IOMUXC_PTA21_I2S1_RXD0                              0x4103D054, 0x7, 0x4103D1E4, 0x1, 0x4103D054
#define IOMUXC_PTA22_ADC0_CH8A_9A_10A                       0x4103D058, 0x0, 0x00000000, 0x0, 0x4103D058
#define IOMUXC_PTA22_GPIOA22                                0x4103D058, 0x1, 0x00000000, 0x0, 0x4103D058
#define IOMUXC_PTA22_FXIO0_D6                               0x4103D058, 0x2, 0x00000000, 0x0, 0x4103D058
#define IOMUXC_PTA22_LPSPI0_PCS2                            0x4103D058, 0x3, 0x4103D108, 0x2, 0x4103D058
#define IOMUXC_PTA22_LPUART1_TX                             0x4103D058, 0x4, 0x4103D20C, 0x2, 0x4103D058
#define IOMUXC_PTA22_LPI2C1_HREQ                            0x4103D058, 0x5, 0x4103D184, 0x2, 0x4103D058
#define IOMUXC_PTA22_TPM0_CH1                               0x4103D058, 0x6, 0x4103D13C, 0x2, 0x4103D058
#define IOMUXC_PTA22_I2S1_RXD1                              0x4103D058, 0x7, 0x4103D1E8, 0x1, 0x4103D058
#define IOMUXC_PTA22_LPTMR0_ALT2                            0x4103D058, 0xB, 0x00000000, 0x0, 0x4103D058
#define IOMUXC_PTA22_EWM_OUT_b                              0x4103D058, 0xC, 0x00000000, 0x0, 0x4103D058
#define IOMUXC_PTA23_ADC0_CH8B_9B_10B                       0x4103D05C, 0x0, 0x00000000, 0x0, 0x4103D05C
#define IOMUXC_PTA23_GPIOA23                                0x4103D05C, 0x1, 0x00000000, 0x0, 0x4103D05C
#define IOMUXC_PTA23_FXIO0_D7                               0x4103D05C, 0x2, 0x00000000, 0x0, 0x4103D05C
#define IOMUXC_PTA23_LPSPI0_PCS3                            0x4103D05C, 0x3, 0x4103D10C, 0x2, 0x4103D05C
#define IOMUXC_PTA23_LPUART1_RX                             0x4103D05C, 0x4, 0x4103D208, 0x2, 0x4103D05C
#define IOMUXC_PTA23_TPM0_CH2                               0x4103D05C, 0x6, 0x4103D140, 0x2, 0x4103D05C
#define IOMUXC_PTA23_I2S1_MCLK                              0x4103D05C, 0x7, 0x4103D1C8, 0x1, 0x4103D05C
#define IOMUXC_PTA23_LLWU0_P6                               0x4103D05C, 0xD, 0x00000000, 0x0, 0x4103D05C
#define IOMUXC_PTA24_ADC0_CH8A_9A_10A                       0x4103D060, 0x0, 0x00000000, 0x0, 0x4103D060
#define IOMUXC_PTA24_GPIOA24                                0x4103D060, 0x1, 0x00000000, 0x0, 0x4103D060
#define IOMUXC_PTA24_FXIO0_D8                               0x4103D060, 0x2, 0x00000000, 0x0, 0x4103D060
#define IOMUXC_PTA24_LPSPI1_PCS1                            0x4103D060, 0x3, 0x4103D120, 0x2, 0x4103D060
#define IOMUXC_PTA24_LPUART2_CTS_b                          0x4103D060, 0x4, 0x4103D210, 0x2, 0x4103D060
#define IOMUXC_PTA24_LPI2C2_SCL                             0x4103D060, 0x5, 0x4103D194, 0x2, 0x4103D060
#define IOMUXC_PTA24_TPM0_CH3                               0x4103D060, 0x6, 0x4103D144, 0x2, 0x4103D060
#define IOMUXC_PTA24_I2S1_TX_BCLK                           0x4103D060, 0x7, 0x4103D1D4, 0x1, 0x4103D060
#define IOMUXC_PTA25_ADC0_CH8B_9B_10B                       0x4103D064, 0x0, 0x00000000, 0x0, 0x4103D064
#define IOMUXC_PTA25_GPIOA25                                0x4103D064, 0x1, 0x00000000, 0x0, 0x4103D064
#define IOMUXC_PTA25_FXIO0_D9                               0x4103D064, 0x2, 0x00000000, 0x0, 0x4103D064
#define IOMUXC_PTA25_LPSPI1_PCS2                            0x4103D064, 0x3, 0x4103D124, 0x2, 0x4103D064
#define IOMUXC_PTA25_LPUART2_RTS_b                          0x4103D064, 0x4, 0x00000000, 0x0, 0x4103D064
#define IOMUXC_PTA25_LPI2C2_SDA                             0x4103D064, 0x5, 0x4103D198, 0x2, 0x4103D064
#define IOMUXC_PTA25_TPM0_CH4                               0x4103D064, 0x6, 0x4103D148, 0x2, 0x4103D064
#define IOMUXC_PTA25_I2S1_TX_FS                             0x4103D064, 0x7, 0x4103D1D8, 0x1, 0x4103D064
#define IOMUXC_PTA26_GPIOA26                                0x4103D068, 0x1, 0x00000000, 0x0, 0x4103D068
#define IOMUXC_PTA26_JTAG_TMS_SWD_DIO                       0x4103D068, 0xA, 0x00000000, 0x0, 0x4103D068
#define IOMUXC_PTA26_FXIO0_D10                              0x4103D068, 0x2, 0x00000000, 0x0, 0x4103D068
#define IOMUXC_PTA26_LPSPI1_PCS3                            0x4103D068, 0x3, 0x4103D128, 0x2, 0x4103D068
#define IOMUXC_PTA26_LPUART2_TX                             0x4103D068, 0x4, 0x4103D218, 0x2, 0x4103D068
#define IOMUXC_PTA26_LPI2C2_HREQ                            0x4103D068, 0x5, 0x4103D190, 0x2, 0x4103D068
#define IOMUXC_PTA26_TPM0_CH5                               0x4103D068, 0x6, 0x4103D14C, 0x2, 0x4103D068
#define IOMUXC_PTA26_I2S1_RXD2                              0x4103D068, 0x7, 0x4103D1EC, 0x1, 0x4103D068
#define IOMUXC_PTA27_GPIOA27                                0x4103D06C, 0x1, 0x00000000, 0x0, 0x4103D06C
#define IOMUXC_PTA27_JTAG_TDO                               0x4103D06C, 0xA, 0x00000000, 0x0, 0x4103D06C
#define IOMUXC_PTA27_FXIO0_D11                              0x4103D06C, 0x2, 0x00000000, 0x0, 0x4103D06C
#define IOMUXC_PTA27_LPUART2_RX                             0x4103D06C, 0x4, 0x4103D214, 0x2, 0x4103D06C
#define IOMUXC_PTA27_TPM1_CH1                               0x4103D06C, 0x6, 0x4103D154, 0x2, 0x4103D06C
#define IOMUXC_PTA27_I2S1_RXD3                              0x4103D06C, 0x7, 0x4103D1F0, 0x1, 0x4103D06C
#define IOMUXC_PTA28_GPIOA28                                0x4103D070, 0x1, 0x00000000, 0x0, 0x4103D070
#define IOMUXC_PTA28_JTAG_TDI                               0x4103D070, 0xA, 0x00000000, 0x0, 0x4103D070
#define IOMUXC_PTA28_FXIO0_D12                              0x4103D070, 0x2, 0x00000000, 0x0, 0x4103D070
#define IOMUXC_PTA28_LPSPI1_SIN                             0x4103D070, 0x3, 0x4103D130, 0x2, 0x4103D070
#define IOMUXC_PTA28_LPUART3_CTS_b                          0x4103D070, 0x4, 0x4103D21C, 0x2, 0x4103D070
#define IOMUXC_PTA28_LPI2C3_SCL                             0x4103D070, 0x5, 0x4103D1A0, 0x2, 0x4103D070
#define IOMUXC_PTA28_TPM1_CLKIN                             0x4103D070, 0x6, 0x4103D1AC, 0x2, 0x4103D070
#define IOMUXC_PTA28_I2S1_TXD2                              0x4103D070, 0x7, 0x00000000, 0x0, 0x4103D070
#define IOMUXC_PTA29_GPIOA29                                0x4103D074, 0x1, 0x00000000, 0x0, 0x4103D074
#define IOMUXC_PTA29_JTAG_TCLK_SWD_CLK                      0x4103D074, 0xA, 0x00000000, 0x0, 0x4103D074
#define IOMUXC_PTA29_FXIO0_D13                              0x4103D074, 0x2, 0x00000000, 0x0, 0x4103D074
#define IOMUXC_PTA29_LPSPI1_SOUT                            0x4103D074, 0x3, 0x4103D134, 0x1, 0x4103D074
#define IOMUXC_PTA29_LPUART3_RTS_b                          0x4103D074, 0x4, 0x00000000, 0x0, 0x4103D074
#define IOMUXC_PTA29_LPI2C3_SDA                             0x4103D074, 0x5, 0x4103D1A4, 0x1, 0x4103D074
#define IOMUXC_PTA29_TPM1_CH0                               0x4103D074, 0x6, 0x4103D150, 0x2, 0x4103D074
#define IOMUXC_PTA29_I2S1_TXD3                              0x4103D074, 0x7, 0x00000000, 0x0, 0x4103D074
#define IOMUXC_PTA30_ADC0_CH1A                              0x4103D078, 0x0, 0x00000000, 0x0, 0x4103D078
#define IOMUXC_PTA30_GPIOA30                                0x4103D078, 0x1, 0x00000000, 0x0, 0x4103D078
#define IOMUXC_PTA30_FXIO0_D14                              0x4103D078, 0x2, 0x00000000, 0x0, 0x4103D078
#define IOMUXC_PTA30_LPSPI1_SCK                             0x4103D078, 0x3, 0x4103D12C, 0x1, 0x4103D078
#define IOMUXC_PTA30_LPUART3_TX                             0x4103D078, 0x4, 0x4103D224, 0x1, 0x4103D078
#define IOMUXC_PTA30_LPI2C3_HREQ                            0x4103D078, 0x5, 0x4103D19C, 0x1, 0x4103D078
#define IOMUXC_PTA30_TPM2_CLKIN                             0x4103D078, 0x6, 0x4103D1F4, 0x2, 0x4103D078
#define IOMUXC_PTA30_I2S1_TXD0                              0x4103D078, 0x7, 0x00000000, 0x0, 0x4103D078
#define IOMUXC_PTA30_JTAG_TRST_b                            0x4103D078, 0xA, 0x00000000, 0x0, 0x4103D078
#define IOMUXC_PTA31_ADC0_CH1B                              0x4103D07C, 0x0, 0x00000000, 0x0, 0x4103D07C
#define IOMUXC_PTA31_GPIOA31                                0x4103D07C, 0x1, 0x00000000, 0x0, 0x4103D07C
#define IOMUXC_PTA31_FXIO0_D15                              0x4103D07C, 0x2, 0x00000000, 0x0, 0x4103D07C
#define IOMUXC_PTA31_LPSPI1_PCS0                            0x4103D07C, 0x3, 0x4103D11C, 0x2, 0x4103D07C
#define IOMUXC_PTA31_LPUART3_RX                             0x4103D07C, 0x4, 0x4103D220, 0x2, 0x4103D07C
#define IOMUXC_PTA31_TPM2_CH0                               0x4103D07C, 0x6, 0x4103D158, 0x2, 0x4103D07C
#define IOMUXC_PTA31_I2S1_TXD1                              0x4103D07C, 0x7, 0x00000000, 0x0, 0x4103D07C
#define IOMUXC_PTA31_LPTMR0_ALT1                            0x4103D07C, 0xB, 0x00000000, 0x0, 0x4103D07C
#define IOMUXC_PTA31_EWM_IN                                 0x4103D07C, 0xC, 0x4103D228, 0x1, 0x4103D07C
#define IOMUXC_PTA31_LLWU0_P7                               0x4103D07C, 0xD, 0x00000000, 0x0, 0x4103D07C
#define IOMUXC_PTB0_ADC0_CH0A                               0x4103D080, 0x0, 0x00000000, 0x0, 0x4103D080
#define IOMUXC_PTB0_GPIOB0                                  0x4103D080, 0x1, 0x00000000, 0x0, 0x4103D080
#define IOMUXC_PTB0_FXIO0_D16                               0x4103D080, 0x2, 0x00000000, 0x0, 0x4103D080
#define IOMUXC_PTB0_LPSPI0_SIN                              0x4103D080, 0x3, 0x4103D114, 0x3, 0x4103D080
#define IOMUXC_PTB0_LPUART0_TX                              0x4103D080, 0x4, 0x4103D200, 0x3, 0x4103D080
#define IOMUXC_PTB0_TPM2_CH1                                0x4103D080, 0x6, 0x4103D15C, 0x2, 0x4103D080
#define IOMUXC_PTB0_CLKOUT0                                 0x4103D080, 0x9, 0x00000000, 0x0, 0x4103D080
#define IOMUXC_PTB0_CMP1_OUT                                0x4103D080, 0xB, 0x00000000, 0x0, 0x4103D080
#define IOMUXC_PTB0_EWM_OUT_b                               0x4103D080, 0xC, 0x00000000, 0x0, 0x4103D080
#define IOMUXC_PTB1_ADC0_CH0B                               0x4103D084, 0x0, 0x00000000, 0x0, 0x4103D084
#define IOMUXC_PTB1_GPIOB1                                  0x4103D084, 0x1, 0x00000000, 0x0, 0x4103D084
#define IOMUXC_PTB1_FXIO0_D17                               0x4103D084, 0x2, 0x00000000, 0x0, 0x4103D084
#define IOMUXC_PTB1_LPSPI0_SOUT                             0x4103D084, 0x3, 0x4103D118, 0x3, 0x4103D084
#define IOMUXC_PTB1_LPUART0_RX                              0x4103D084, 0x4, 0x4103D1FC, 0x3, 0x4103D084
#define IOMUXC_PTB1_TPM3_CLKIN                              0x4103D084, 0x6, 0x4103D1B0, 0x3, 0x4103D084
#define IOMUXC_PTB1_I2S1_TX_BCLK                            0x4103D084, 0x7, 0x4103D1D4, 0x2, 0x4103D084
#define IOMUXC_PTB1_RTC_CLKOUT                              0x4103D084, 0xB, 0x00000000, 0x0, 0x4103D084
#define IOMUXC_PTB1_EWM_IN                                  0x4103D084, 0xC, 0x4103D228, 0x2, 0x4103D084
#define IOMUXC_PTB1_LLWU0_P8                                0x4103D084, 0xD, 0x00000000, 0x0, 0x4103D084
#define IOMUXC_PTB2_ADC0_CH4A_5A_6A                         0x4103D088, 0x0, 0x00000000, 0x0, 0x4103D088
#define IOMUXC_PTB2_GPIOB2                                  0x4103D088, 0x1, 0x00000000, 0x0, 0x4103D088
#define IOMUXC_PTB2_FXIO0_D18                               0x4103D088, 0x2, 0x00000000, 0x0, 0x4103D088
#define IOMUXC_PTB2_LPSPI0_SCK                              0x4103D088, 0x3, 0x4103D110, 0x3, 0x4103D088
#define IOMUXC_PTB2_LPUART1_TX                              0x4103D088, 0x4, 0x4103D20C, 0x3, 0x4103D088
#define IOMUXC_PTB2_TPM3_CH0                                0x4103D088, 0x6, 0x4103D160, 0x2, 0x4103D088
#define IOMUXC_PTB2_I2S1_TX_FS                              0x4103D088, 0x7, 0x4103D1D8, 0x2, 0x4103D088
#define IOMUXC_PTB2_TRACE_CLKOUT                            0x4103D088, 0xA, 0x00000000, 0x0, 0x4103D088
#define IOMUXC_PTB3_ADC0_CH4B_5B_6B                         0x4103D08C, 0x0, 0x00000000, 0x0, 0x4103D08C
#define IOMUXC_PTB3_GPIOB3                                  0x4103D08C, 0x1, 0x00000000, 0x0, 0x4103D08C
#define IOMUXC_PTB3_FXIO0_D19                               0x4103D08C, 0x2, 0x00000000, 0x0, 0x4103D08C
#define IOMUXC_PTB3_LPSPI0_PCS0                             0x4103D08C, 0x3, 0x4103D100, 0x3, 0x4103D08C
#define IOMUXC_PTB3_LPUART1_RX                              0x4103D08C, 0x4, 0x4103D208, 0x3, 0x4103D08C
#define IOMUXC_PTB3_TPM3_CH1                                0x4103D08C, 0x6, 0x4103D164, 0x2, 0x4103D08C
#define IOMUXC_PTB3_I2S1_TXD0                               0x4103D08C, 0x7, 0x00000000, 0x0, 0x4103D08C
#define IOMUXC_PTB3_TRACE_D0                                0x4103D08C, 0xA, 0x00000000, 0x0, 0x4103D08C
#define IOMUXC_PTB3_LPTMR1_ALT2                             0x4103D08C, 0xB, 0x00000000, 0x0, 0x4103D08C
#define IOMUXC_PTB3_LLWU0_P9                                0x4103D08C, 0xD, 0x00000000, 0x0, 0x4103D08C
#define IOMUXC_PTB4_GPIOB4                                  0x4103D090, 0x1, 0x00000000, 0x0, 0x4103D090
#define IOMUXC_PTB4_FXIO0_D20                               0x4103D090, 0x2, 0x00000000, 0x0, 0x4103D090
#define IOMUXC_PTB4_LPSPI0_PCS1                             0x4103D090, 0x3, 0x4103D104, 0x3, 0x4103D090
#define IOMUXC_PTB4_LPUART2_TX                              0x4103D090, 0x4, 0x4103D218, 0x3, 0x4103D090
#define IOMUXC_PTB4_LPI2C0_HREQ                             0x4103D090, 0x5, 0x4103D178, 0x3, 0x4103D090
#define IOMUXC_PTB4_TPM3_CH2                                0x4103D090, 0x6, 0x4103D168, 0x2, 0x4103D090
#define IOMUXC_PTB4_I2S1_TXD1                               0x4103D090, 0x7, 0x00000000, 0x0, 0x4103D090
#define IOMUXC_PTB4_QSPIA_DATA7                             0x4103D090, 0x8, 0x00000000, 0x0, 0x4103D090
#define IOMUXC_PTB4_TRACE_D1                                0x4103D090, 0xA, 0x00000000, 0x0, 0x4103D090
#define IOMUXC_PTB4_BT_MODE0                                0x4103D090, 0x0, 0x00000000, 0x0, 0x4103D090
#define IOMUXC_PTB5_GPIOB5                                  0x4103D094, 0x1, 0x00000000, 0x0, 0x4103D094
#define IOMUXC_PTB5_FXIO0_D21                               0x4103D094, 0x2, 0x00000000, 0x0, 0x4103D094
#define IOMUXC_PTB5_LPSPI0_PCS2                             0x4103D094, 0x3, 0x4103D108, 0x3, 0x4103D094
#define IOMUXC_PTB5_LPUART2_RX                              0x4103D094, 0x4, 0x4103D214, 0x3, 0x4103D094
#define IOMUXC_PTB5_LPI2C1_HREQ                             0x4103D094, 0x5, 0x4103D184, 0x3, 0x4103D094
#define IOMUXC_PTB5_TPM3_CH3                                0x4103D094, 0x6, 0x4103D16C, 0x2, 0x4103D094
#define IOMUXC_PTB5_I2S1_TXD2                               0x4103D094, 0x7, 0x00000000, 0x0, 0x4103D094
#define IOMUXC_PTB5_QSPIA_DATA6                             0x4103D094, 0x8, 0x00000000, 0x0, 0x4103D094
#define IOMUXC_PTB5_TRACE_D2                                0x4103D094, 0xA, 0x00000000, 0x0, 0x4103D094
#define IOMUXC_PTB5_BT_MODE1                                0x4103D094, 0x0, 0x00000000, 0x0, 0x4103D094
#define IOMUXC_PTB6_ADC1_CH1A                               0x4103D098, 0x0, 0x00000000, 0x0, 0x4103D098
#define IOMUXC_PTB6_GPIOB6                                  0x4103D098, 0x1, 0x00000000, 0x0, 0x4103D098
#define IOMUXC_PTB6_FXIO0_D22                               0x4103D098, 0x2, 0x00000000, 0x0, 0x4103D098
#define IOMUXC_PTB6_LPSPI0_PCS3                             0x4103D098, 0x3, 0x4103D10C, 0x3, 0x4103D098
#define IOMUXC_PTB6_LPUART3_TX                              0x4103D098, 0x4, 0x4103D224, 0x3, 0x4103D098
#define IOMUXC_PTB6_LPI2C0_SCL                              0x4103D098, 0x5, 0x4103D17C, 0x3, 0x4103D098
#define IOMUXC_PTB6_TPM3_CH4                                0x4103D098, 0x6, 0x4103D170, 0x2, 0x4103D098
#define IOMUXC_PTB6_I2S1_TXD3                               0x4103D098, 0x7, 0x00000000, 0x0, 0x4103D098
#define IOMUXC_PTB6_QSPIA_DATA5                             0x4103D098, 0x8, 0x00000000, 0x0, 0x4103D098
#define IOMUXC_PTB6_TRACE_D3                                0x4103D098, 0xA, 0x00000000, 0x0, 0x4103D098
#define IOMUXC_PTB6_LPTMR1_ALT3                             0x4103D098, 0xB, 0x00000000, 0x0, 0x4103D098
#define IOMUXC_PTB6_LLWU0_P10                               0x4103D098, 0xD, 0x00000000, 0x0, 0x4103D098
#define IOMUXC_PTB7_ADC1_CH1B                               0x4103D09C, 0x0, 0x00000000, 0x0, 0x4103D09C
#define IOMUXC_PTB7_GPIOB7                                  0x4103D09C, 0x1, 0x00000000, 0x0, 0x4103D09C
#define IOMUXC_PTB7_FXIO0_D23                               0x4103D09C, 0x2, 0x00000000, 0x0, 0x4103D09C
#define IOMUXC_PTB7_LPSPI1_SIN                              0x4103D09C, 0x3, 0x4103D130, 0x3, 0x4103D09C
#define IOMUXC_PTB7_LPUART3_RX                              0x4103D09C, 0x4, 0x4103D220, 0x3, 0x4103D09C
#define IOMUXC_PTB7_LPI2C0_SDA                              0x4103D09C, 0x5, 0x4103D180, 0x3, 0x4103D09C
#define IOMUXC_PTB7_TPM3_CH5                                0x4103D09C, 0x6, 0x4103D174, 0x2, 0x4103D09C
#define IOMUXC_PTB7_I2S1_MCLK                               0x4103D09C, 0x7, 0x4103D1C8, 0x2, 0x4103D09C
#define IOMUXC_PTB7_QSPIA_SS1_B                             0x4103D09C, 0x8, 0x00000000, 0x0, 0x4103D09C
#define IOMUXC_PTB7_CMP1_OUT                                0x4103D09C, 0xB, 0x00000000, 0x0, 0x4103D09C
#define IOMUXC_PTB7_LLWU0_P11                               0x4103D09C, 0xD, 0x00000000, 0x0, 0x4103D09C
#define IOMUXC_PTB8_ADC0_CH14A_CMP0_IN0                     0x4103D0A0, 0x0, 0x00000000, 0x0, 0x4103D0A0
#define IOMUXC_PTB8_GPIOB8                                  0x4103D0A0, 0x1, 0x00000000, 0x0, 0x4103D0A0
#define IOMUXC_PTB8_FXIO0_D24                               0x4103D0A0, 0x2, 0x00000000, 0x0, 0x4103D0A0
#define IOMUXC_PTB8_LPSPI1_SOUT                             0x4103D0A0, 0x3, 0x4103D134, 0x3, 0x4103D0A0
#define IOMUXC_PTB8_LPI2C1_SCL                              0x4103D0A0, 0x5, 0x4103D188, 0x3, 0x4103D0A0
#define IOMUXC_PTB8_TPM0_CLKIN                              0x4103D0A0, 0x6, 0x4103D1A8, 0x3, 0x4103D0A0
#define IOMUXC_PTB8_I2S1_RX_BCLK                            0x4103D0A0, 0x7, 0x4103D1CC, 0x2, 0x4103D0A0
#define IOMUXC_PTB8_QSPIA_SS0_B                             0x4103D0A0, 0x8, 0x00000000, 0x0, 0x4103D0A0
#define IOMUXC_PTB8_RTC_CLKOUT                              0x4103D0A0, 0xB, 0x00000000, 0x0, 0x4103D0A0
#define IOMUXC_PTB9_ADC0_CH14B_CMP0_IN2                     0x4103D0A4, 0x0, 0x00000000, 0x0, 0x4103D0A4
#define IOMUXC_PTB9_GPIOB9                                  0x4103D0A4, 0x1, 0x00000000, 0x0, 0x4103D0A4
#define IOMUXC_PTB9_FXIO0_D25                               0x4103D0A4, 0x2, 0x00000000, 0x0, 0x4103D0A4
#define IOMUXC_PTB9_LPSPI1_SCK                              0x4103D0A4, 0x3, 0x4103D12C, 0x3, 0x4103D0A4
#define IOMUXC_PTB9_LPI2C1_SDA                              0x4103D0A4, 0x5, 0x4103D18C, 0x3, 0x4103D0A4
#define IOMUXC_PTB9_TPM0_CH0                                0x4103D0A4, 0x6, 0x4103D138, 0x3, 0x4103D0A4
#define IOMUXC_PTB9_I2S1_RX_FS                              0x4103D0A4, 0x7, 0x4103D1D0, 0x2, 0x4103D0A4
#define IOMUXC_PTB9_QSPIA_DQS                               0x4103D0A4, 0x8, 0x00000000, 0x0, 0x4103D0A4
#define IOMUXC_PTB9_LLWU0_P12                               0x4103D0A4, 0xD, 0x00000000, 0x0, 0x4103D0A4
#define IOMUXC_PTB10_CMP0_IN1                               0x4103D0A8, 0x0, 0x00000000, 0x0, 0x4103D0A8
#define IOMUXC_PTB10_GPIOB10                                0x4103D0A8, 0x1, 0x00000000, 0x0, 0x4103D0A8
#define IOMUXC_PTB10_FXIO0_D26                              0x4103D0A8, 0x2, 0x00000000, 0x0, 0x4103D0A8
#define IOMUXC_PTB10_LPSPI1_PCS0                            0x4103D0A8, 0x3, 0x4103D11C, 0x3, 0x4103D0A8
#define IOMUXC_PTB10_LPI2C2_SCL                             0x4103D0A8, 0x5, 0x4103D194, 0x3, 0x4103D0A8
#define IOMUXC_PTB10_TPM0_CH1                               0x4103D0A8, 0x6, 0x4103D13C, 0x3, 0x4103D0A8
#define IOMUXC_PTB10_I2S1_RXD0                              0x4103D0A8, 0x7, 0x4103D1E4, 0x2, 0x4103D0A8
#define IOMUXC_PTB10_TRACE_D4                               0x4103D0A8, 0xA, 0x00000000, 0x0, 0x4103D0A8
#define IOMUXC_PTB11_CMP0_IN3                               0x4103D0AC, 0x0, 0x00000000, 0x0, 0x4103D0AC
#define IOMUXC_PTB11_GPIOB11                                0x4103D0AC, 0x1, 0x00000000, 0x0, 0x4103D0AC
#define IOMUXC_PTB11_FXIO0_D27                              0x4103D0AC, 0x2, 0x00000000, 0x0, 0x4103D0AC
#define IOMUXC_PTB11_LPSPI1_PCS1                            0x4103D0AC, 0x3, 0x4103D120, 0x3, 0x4103D0AC
#define IOMUXC_PTB11_LPI2C2_SDA                             0x4103D0AC, 0x5, 0x4103D198, 0x3, 0x4103D0AC
#define IOMUXC_PTB11_TPM1_CLKIN                             0x4103D0AC, 0x6, 0x4103D1AC, 0x3, 0x4103D0AC
#define IOMUXC_PTB11_I2S1_RXD1                              0x4103D0AC, 0x7, 0x4103D1E8, 0x2, 0x4103D0AC
#define IOMUXC_PTB11_TRACE_D5                               0x4103D0AC, 0xA, 0x00000000, 0x0, 0x4103D0AC
#define IOMUXC_PTB12_ADC1_CH13A_CMP1_IN0                    0x4103D0B0, 0x0, 0x00000000, 0x0, 0x4103D0B0
#define IOMUXC_PTB12_GPIOB12                                0x4103D0B0, 0x1, 0x00000000, 0x0, 0x4103D0B0
#define IOMUXC_PTB12_FXIO0_D28                              0x4103D0B0, 0x2, 0x00000000, 0x0, 0x4103D0B0
#define IOMUXC_PTB12_LPSPI1_PCS2                            0x4103D0B0, 0x3, 0x4103D124, 0x3, 0x4103D0B0
#define IOMUXC_PTB12_LPI2C3_SCL                             0x4103D0B0, 0x5, 0x4103D1A0, 0x3, 0x4103D0B0
#define IOMUXC_PTB12_TPM1_CH0                               0x4103D0B0, 0x6, 0x4103D150, 0x3, 0x4103D0B0
#define IOMUXC_PTB12_I2S1_RXD2                              0x4103D0B0, 0x7, 0x4103D1EC, 0x2, 0x4103D0B0
#define IOMUXC_PTB12_TRACE_D6                               0x4103D0B0, 0xA, 0x00000000, 0x0, 0x4103D0B0
#define IOMUXC_PTB13_ADC1_CH13B_CMP1_IN1                    0x4103D0B4, 0x0, 0x00000000, 0x0, 0x4103D0B4
#define IOMUXC_PTB13_GPIOB13                                0x4103D0B4, 0x1, 0x00000000, 0x0, 0x4103D0B4
#define IOMUXC_PTB13_FXIO0_D29                              0x4103D0B4, 0x2, 0x00000000, 0x0, 0x4103D0B4
#define IOMUXC_PTB13_LPSPI1_PCS3                            0x4103D0B4, 0x3, 0x4103D128, 0x3, 0x4103D0B4
#define IOMUXC_PTB13_LPI2C3_SDA                             0x4103D0B4, 0x5, 0x4103D1A4, 0x3, 0x4103D0B4
#define IOMUXC_PTB13_TPM1_CH1                               0x4103D0B4, 0x6, 0x4103D154, 0x3, 0x4103D0B4
#define IOMUXC_PTB13_I2S1_RXD3                              0x4103D0B4, 0x7, 0x4103D1F0, 0x2, 0x4103D0B4
#define IOMUXC_PTB13_QSPIA_DATA4                            0x4103D0B4, 0x8, 0x00000000, 0x0, 0x4103D0B4
#define IOMUXC_PTB13_TRACE_D7                               0x4103D0B4, 0xA, 0x00000000, 0x0, 0x4103D0B4
#define IOMUXC_PTB14_ADC1_CH2A                              0x4103D0B8, 0x0, 0x00000000, 0x0, 0x4103D0B8
#define IOMUXC_PTB14_GPIOB14                                0x4103D0B8, 0x1, 0x00000000, 0x0, 0x4103D0B8
#define IOMUXC_PTB14_FXIO0_D30                              0x4103D0B8, 0x2, 0x00000000, 0x0, 0x4103D0B8
#define IOMUXC_PTB14_LPI2C2_HREQ                            0x4103D0B8, 0x5, 0x4103D190, 0x3, 0x4103D0B8
#define IOMUXC_PTB14_TPM2_CLKIN                             0x4103D0B8, 0x6, 0x4103D1F4, 0x3, 0x4103D0B8
#define IOMUXC_PTB14_QSPIA_SS1_B                            0x4103D0B8, 0x8, 0x00000000, 0x0, 0x4103D0B8
#define IOMUXC_PTB14_QSPIA_SCLK_b                           0x4103D0B8, 0x9, 0x00000000, 0x0, 0x4103D0B8
#define IOMUXC_PTB14_LLWU0_P13                              0x4103D0B8, 0xD, 0x00000000, 0x0, 0x4103D0B8
#define IOMUXC_PTB15_ADC1_CH2B                              0x4103D0BC, 0x0, 0x00000000, 0x0, 0x4103D0BC
#define IOMUXC_PTB15_GPIOB15                                0x4103D0BC, 0x1, 0x00000000, 0x0, 0x4103D0BC
#define IOMUXC_PTB15_FXIO0_D31                              0x4103D0BC, 0x2, 0x00000000, 0x0, 0x4103D0BC
#define IOMUXC_PTB15_LPI2C3_HREQ                            0x4103D0BC, 0x5, 0x4103D19C, 0x3, 0x4103D0BC
#define IOMUXC_PTB15_TPM2_CH0                               0x4103D0BC, 0x6, 0x4103D158, 0x3, 0x4103D0BC
#define IOMUXC_PTB15_QSPIA_SCLK                             0x4103D0BC, 0x8, 0x00000000, 0x0, 0x4103D0BC
#define IOMUXC_PTB16_ADC0_CH4A_5A_6A                        0x4103D0C0, 0x0, 0x00000000, 0x0, 0x4103D0C0
#define IOMUXC_PTB16_GPIOB16                                0x4103D0C0, 0x1, 0x00000000, 0x0, 0x4103D0C0
#define IOMUXC_PTB16_TPM2_CH1                               0x4103D0C0, 0x6, 0x4103D15C, 0x3, 0x4103D0C0
#define IOMUXC_PTB16_QSPIA_DATA3                            0x4103D0C0, 0x8, 0x00000000, 0x0, 0x4103D0C0
#define IOMUXC_PTB16_LLWU0_P14                              0x4103D0C0, 0xD, 0x00000000, 0x0, 0x4103D0C0
#define IOMUXC_PTB17_ADC0_CH4B_5B_6B                        0x4103D0C4, 0x0, 0x00000000, 0x0, 0x4103D0C4
#define IOMUXC_PTB17_GPIOB17                                0x4103D0C4, 0x1, 0x00000000, 0x0, 0x4103D0C4
#define IOMUXC_PTB17_TPM3_CLKIN                             0x4103D0C4, 0x6, 0x4103D1B0, 0x2, 0x4103D0C4
#define IOMUXC_PTB17_QSPIA_DATA2                            0x4103D0C4, 0x8, 0x00000000, 0x0, 0x4103D0C4
#define IOMUXC_PTB18_ADC0_CH4A_5A_6A                        0x4103D0C8, 0x0, 0x00000000, 0x0, 0x4103D0C8
#define IOMUXC_PTB18_GPIOB18                                0x4103D0C8, 0x1, 0x00000000, 0x0, 0x4103D0C8
#define IOMUXC_PTB18_TPM3_CH0                               0x4103D0C8, 0x6, 0x4103D160, 0x3, 0x4103D0C8
#define IOMUXC_PTB18_QSPIA_DATA1                            0x4103D0C8, 0x8, 0x00000000, 0x0, 0x4103D0C8
#define IOMUXC_PTB19_ADC0_CH4B_5B_6B                        0x4103D0CC, 0x0, 0x00000000, 0x0, 0x4103D0CC
#define IOMUXC_PTB19_GPIOB19                                0x4103D0CC, 0x1, 0x00000000, 0x0, 0x4103D0CC
#define IOMUXC_PTB19_TPM3_CH1                               0x4103D0CC, 0x6, 0x4103D164, 0x3, 0x4103D0CC
#define IOMUXC_PTB19_QSPIA_DATA0                            0x4103D0CC, 0x8, 0x00000000, 0x0, 0x4103D0CC
#define IOMUXC_PTB19_USB0_ID                                0x4103D0CC, 0xA, 0x00000000, 0x0, 0x4103D0CC
#define IOMUXC_PTB19_LLWU0_P15                              0x4103D0CC, 0xD, 0x00000000, 0x0, 0x4103D0CC
#define IOMUXC_PTC0_GPIOC0                                  0x40AC0000, 0x1, 0x00000000, 0x0, 0x40AC0000
#define IOMUXC_PTC0_LPUART4_CTS_b                           0x40AC0000, 0x4, 0x40AC0244, 0x1, 0x40AC0000
#define IOMUXC_PTC0_LPI2C4_SCL                              0x40AC0000, 0x5, 0x40AC0278, 0x1, 0x40AC0000
#define IOMUXC_PTC0_TPM4_CLKIN                              0x40AC0000, 0x6, 0x40AC0298, 0x1, 0x40AC0000
#define IOMUXC_PTC0_FB_AD0                                  0x40AC0000, 0x9, 0x00000000, 0x0, 0x40AC0000
#define IOMUXC_PTC0_TRACE_D15                               0x40AC0000, 0xA, 0x00000000, 0x0, 0x40AC0000
#define IOMUXC_PTC0_DEBUG_MUX0                              0x40AC0000, 0xE, 0x00000000, 0x0, 0x40AC0000
#define IOMUXC_PTC1_GPIOC1                                  0x40AC0004, 0x1, 0x00000000, 0x0, 0x40AC0004
#define IOMUXC_PTC1_LPUART4_RTS_b                           0x40AC0004, 0x4, 0x00000000, 0x0, 0x40AC0004
#define IOMUXC_PTC1_LPI2C4_SDA                              0x40AC0004, 0x5, 0x40AC027C, 0x1, 0x40AC0004
#define IOMUXC_PTC1_TPM4_CH0                                0x40AC0004, 0x6, 0x40AC0280, 0x1, 0x40AC0004
#define IOMUXC_PTC1_FB_AD1                                  0x40AC0004, 0x9, 0x00000000, 0x0, 0x40AC0004
#define IOMUXC_PTC1_TRACE_D14                               0x40AC0004, 0xA, 0x00000000, 0x0, 0x40AC0004
#define IOMUXC_PTC1_DEBUG_MUX1                              0x40AC0004, 0xE, 0x00000000, 0x0, 0x40AC0004
#define IOMUXC_PTC2_GPIOC2                                  0x40AC0008, 0x1, 0x00000000, 0x0, 0x40AC0008
#define IOMUXC_PTC2_LPUART4_TX                              0x40AC0008, 0x4, 0x40AC024C, 0x1, 0x40AC0008
#define IOMUXC_PTC2_LPI2C4_HREQ                             0x40AC0008, 0x5, 0x40AC0274, 0x1, 0x40AC0008
#define IOMUXC_PTC2_TPM4_CH1                                0x40AC0008, 0x6, 0x40AC0284, 0x1, 0x40AC0008
#define IOMUXC_PTC2_FB_AD2                                  0x40AC0008, 0x9, 0x00000000, 0x0, 0x40AC0008
#define IOMUXC_PTC2_TRACE_D13                               0x40AC0008, 0xA, 0x00000000, 0x0, 0x40AC0008
#define IOMUXC_PTC2_DEBUG_MUX2                              0x40AC0008, 0xE, 0x00000000, 0x0, 0x40AC0008
#define IOMUXC_PTC3_GPIOC3                                  0x40AC000C, 0x1, 0x00000000, 0x0, 0x40AC000C
#define IOMUXC_PTC3_LPUART4_RX                              0x40AC000C, 0x4, 0x40AC0248, 0x1, 0x40AC000C
#define IOMUXC_PTC3_TPM4_CH2                                0x40AC000C, 0x6, 0x40AC0288, 0x1, 0x40AC000C
#define IOMUXC_PTC3_FB_AD3                                  0x40AC000C, 0x9, 0x00000000, 0x0, 0x40AC000C
#define IOMUXC_PTC3_TRACE_D12                               0x40AC000C, 0xA, 0x00000000, 0x0, 0x40AC000C
#define IOMUXC_PTC3_DEBUG_MUX3                              0x40AC000C, 0xE, 0x00000000, 0x0, 0x40AC000C
#define IOMUXC_PTC4_GPIOC4                                  0x40AC0010, 0x1, 0x00000000, 0x0, 0x40AC0010
#define IOMUXC_PTC4_FXIO1_D0                                0x40AC0010, 0x2, 0x40AC0204, 0x1, 0x40AC0010
#define IOMUXC_PTC4_LPSPI2_PCS1                             0x40AC0010, 0x3, 0x40AC02A0, 0x1, 0x40AC0010
#define IOMUXC_PTC4_LPUART5_CTS_b                           0x40AC0010, 0x4, 0x40AC0250, 0x1, 0x40AC0010
#define IOMUXC_PTC4_LPI2C5_SCL                              0x40AC0010, 0x5, 0x40AC02BC, 0x1, 0x40AC0010
#define IOMUXC_PTC4_TPM4_CH3                                0x40AC0010, 0x6, 0x40AC028C, 0x1, 0x40AC0010
#define IOMUXC_PTC4_FB_AD4                                  0x40AC0010, 0x9, 0x00000000, 0x0, 0x40AC0010
#define IOMUXC_PTC4_TRACE_D11                               0x40AC0010, 0xA, 0x00000000, 0x0, 0x40AC0010
#define IOMUXC_PTC4_DEBUG_MUX4                              0x40AC0010, 0xE, 0x00000000, 0x0, 0x40AC0010
#define IOMUXC_PTC5_GPIOC5                                  0x40AC0014, 0x1, 0x00000000, 0x0, 0x40AC0014
#define IOMUXC_PTC5_FXIO1_D1                                0x40AC0014, 0x2, 0x40AC0208, 0x1, 0x40AC0014
#define IOMUXC_PTC5_LPSPI2_PCS2                             0x40AC0014, 0x3, 0x40AC02A4, 0x1, 0x40AC0014
#define IOMUXC_PTC5_LPUART5_RTS_b                           0x40AC0014, 0x4, 0x00000000, 0x0, 0x40AC0014
#define IOMUXC_PTC5_LPI2C5_SDA                              0x40AC0014, 0x5, 0x40AC02C0, 0x1, 0x40AC0014
#define IOMUXC_PTC5_TPM4_CH4                                0x40AC0014, 0x6, 0x40AC0290, 0x1, 0x40AC0014
#define IOMUXC_PTC5_FB_AD5                                  0x40AC0014, 0x9, 0x00000000, 0x0, 0x40AC0014
#define IOMUXC_PTC5_TRACE_D10                               0x40AC0014, 0xA, 0x00000000, 0x0, 0x40AC0014
#define IOMUXC_PTC5_DEBUG_MUX5                              0x40AC0014, 0xE, 0x00000000, 0x0, 0x40AC0014
#define IOMUXC_PTC6_GPIOC6                                  0x40AC0018, 0x1, 0x00000000, 0x0, 0x40AC0018
#define IOMUXC_PTC6_FXIO1_D2                                0x40AC0018, 0x2, 0x40AC020C, 0x1, 0x40AC0018
#define IOMUXC_PTC6_LPSPI2_PCS3                             0x40AC0018, 0x3, 0x40AC02A8, 0x1, 0x40AC0018
#define IOMUXC_PTC6_LPUART5_TX                              0x40AC0018, 0x4, 0x40AC0258, 0x1, 0x40AC0018
#define IOMUXC_PTC6_LPI2C5_HREQ                             0x40AC0018, 0x5, 0x40AC02B8, 0x1, 0x40AC0018
#define IOMUXC_PTC6_TPM4_CH5                                0x40AC0018, 0x6, 0x40AC0294, 0x1, 0x40AC0018
#define IOMUXC_PTC6_FB_AD6                                  0x40AC0018, 0x9, 0x00000000, 0x0, 0x40AC0018
#define IOMUXC_PTC6_TRACE_D9                                0x40AC0018, 0xA, 0x00000000, 0x0, 0x40AC0018
#define IOMUXC_PTC6_DEBUG_MUX6                              0x40AC0018, 0xE, 0x00000000, 0x0, 0x40AC0018
#define IOMUXC_PTC7_GPIOC7                                  0x40AC001C, 0x1, 0x00000000, 0x0, 0x40AC001C
#define IOMUXC_PTC7_FXIO1_D3                                0x40AC001C, 0x2, 0x40AC0210, 0x1, 0x40AC001C
#define IOMUXC_PTC7_LPUART5_RX                              0x40AC001C, 0x4, 0x40AC0254, 0x1, 0x40AC001C
#define IOMUXC_PTC7_TPM5_CH1                                0x40AC001C, 0x6, 0x40AC02C8, 0x1, 0x40AC001C
#define IOMUXC_PTC7_FB_AD7                                  0x40AC001C, 0x9, 0x00000000, 0x0, 0x40AC001C
#define IOMUXC_PTC7_TRACE_D8                                0x40AC001C, 0xA, 0x00000000, 0x0, 0x40AC001C
#define IOMUXC_PTC7_DEBUG_MUX7                              0x40AC001C, 0xE, 0x00000000, 0x0, 0x40AC001C
#define IOMUXC_PTC8_GPIOC8                                  0x40AC0020, 0x1, 0x00000000, 0x0, 0x40AC0020
#define IOMUXC_PTC8_FXIO1_D4                                0x40AC0020, 0x2, 0x40AC0214, 0x1, 0x40AC0020
#define IOMUXC_PTC8_LPSPI2_SIN                              0x40AC0020, 0x3, 0x40AC02B0, 0x1, 0x40AC0020
#define IOMUXC_PTC8_LPUART6_CTS_b                           0x40AC0020, 0x4, 0x40AC025C, 0x1, 0x40AC0020
#define IOMUXC_PTC8_LPI2C6_SCL                              0x40AC0020, 0x5, 0x40AC02FC, 0x1, 0x40AC0020
#define IOMUXC_PTC8_TPM5_CLKIN                              0x40AC0020, 0x6, 0x40AC02CC, 0x1, 0x40AC0020
#define IOMUXC_PTC8_FB_AD8                                  0x40AC0020, 0x9, 0x00000000, 0x0, 0x40AC0020
#define IOMUXC_PTC8_TRACE_D7                                0x40AC0020, 0xA, 0x00000000, 0x0, 0x40AC0020
#define IOMUXC_PTC8_DEBUG_MUX8                              0x40AC0020, 0xE, 0x00000000, 0x0, 0x40AC0020
#define IOMUXC_PTC9_GPIOC9                                  0x40AC0024, 0x1, 0x00000000, 0x0, 0x40AC0024
#define IOMUXC_PTC9_FXIO1_D5                                0x40AC0024, 0x2, 0x40AC0218, 0x1, 0x40AC0024
#define IOMUXC_PTC9_LPSPI2_SOUT                             0x40AC0024, 0x3, 0x40AC02B4, 0x1, 0x40AC0024
#define IOMUXC_PTC9_LPUART6_RTS_b                           0x40AC0024, 0x4, 0x00000000, 0x0, 0x40AC0024
#define IOMUXC_PTC9_LPI2C6_SDA                              0x40AC0024, 0x5, 0x40AC0300, 0x1, 0x40AC0024
#define IOMUXC_PTC9_TPM5_CH0                                0x40AC0024, 0x6, 0x40AC02C4, 0x1, 0x40AC0024
#define IOMUXC_PTC9_FB_AD9                                  0x40AC0024, 0x9, 0x00000000, 0x0, 0x40AC0024
#define IOMUXC_PTC9_TRACE_D6                                0x40AC0024, 0xA, 0x00000000, 0x0, 0x40AC0024
#define IOMUXC_PTC9_DEBUG_MUX9                              0x40AC0024, 0xE, 0x00000000, 0x0, 0x40AC0024
#define IOMUXC_PTC10_GPIOC10                                0x40AC0028, 0x1, 0x00000000, 0x0, 0x40AC0028
#define IOMUXC_PTC10_FXIO1_D6                               0x40AC0028, 0x2, 0x40AC021C, 0x1, 0x40AC0028
#define IOMUXC_PTC10_LPSPI2_SCK                             0x40AC0028, 0x3, 0x40AC02AC, 0x1, 0x40AC0028
#define IOMUXC_PTC10_LPUART6_TX                             0x40AC0028, 0x4, 0x40AC0264, 0x1, 0x40AC0028
#define IOMUXC_PTC10_LPI2C6_HREQ                            0x40AC0028, 0x5, 0x40AC02F8, 0x1, 0x40AC0028
#define IOMUXC_PTC10_TPM7_CH3                               0x40AC0028, 0x6, 0x40AC02E8, 0x1, 0x40AC0028
#define IOMUXC_PTC10_FB_AD10                                0x40AC0028, 0x9, 0x00000000, 0x0, 0x40AC0028
#define IOMUXC_PTC10_TRACE_D5                               0x40AC0028, 0xA, 0x00000000, 0x0, 0x40AC0028
#define IOMUXC_PTC10_DEBUG_MUX10                            0x40AC0028, 0xE, 0x00000000, 0x0, 0x40AC0028
#define IOMUXC_PTC11_GPIOC11                                0x40AC002C, 0x1, 0x00000000, 0x0, 0x40AC002C
#define IOMUXC_PTC11_FXIO1_D7                               0x40AC002C, 0x2, 0x40AC0220, 0x1, 0x40AC002C
#define IOMUXC_PTC11_LPSPI2_PCS0                            0x40AC002C, 0x3, 0x40AC029C, 0x1, 0x40AC002C
#define IOMUXC_PTC11_LPUART6_RX                             0x40AC002C, 0x4, 0x40AC0260, 0x1, 0x40AC002C
#define IOMUXC_PTC11_TPM7_CH4                               0x40AC002C, 0x6, 0x40AC02EC, 0x1, 0x40AC002C
#define IOMUXC_PTC11_FB_AD11                                0x40AC002C, 0x9, 0x00000000, 0x0, 0x40AC002C
#define IOMUXC_PTC11_TRACE_D4                               0x40AC002C, 0xA, 0x00000000, 0x0, 0x40AC002C
#define IOMUXC_PTC11_DEBUG_MUX11                            0x40AC002C, 0xE, 0x00000000, 0x0, 0x40AC002C
#define IOMUXC_PTC12_GPIOC12                                0x40AC0030, 0x1, 0x00000000, 0x0, 0x40AC0030
#define IOMUXC_PTC12_FXIO1_D8                               0x40AC0030, 0x2, 0x40AC0224, 0x1, 0x40AC0030
#define IOMUXC_PTC12_LPSPI3_PCS1                            0x40AC0030, 0x3, 0x40AC0314, 0x1, 0x40AC0030
#define IOMUXC_PTC12_LPUART7_CTS_b                          0x40AC0030, 0x4, 0x40AC0268, 0x1, 0x40AC0030
#define IOMUXC_PTC12_LPI2C7_SCL                             0x40AC0030, 0x5, 0x40AC0308, 0x1, 0x40AC0030
#define IOMUXC_PTC12_TPM7_CH5                               0x40AC0030, 0x6, 0x40AC02F0, 0x1, 0x40AC0030
#define IOMUXC_PTC12_FB_AD12                                0x40AC0030, 0x9, 0x00000000, 0x0, 0x40AC0030
#define IOMUXC_PTC12_TRACE_D3                               0x40AC0030, 0xA, 0x00000000, 0x0, 0x40AC0030
#define IOMUXC_PTC12_DEBUG_MUX12                            0x40AC0030, 0xE, 0x00000000, 0x0, 0x40AC0030
#define IOMUXC_PTC13_GPIOC13                                0x40AC0034, 0x1, 0x00000000, 0x0, 0x40AC0034
#define IOMUXC_PTC13_FXIO1_D9                               0x40AC0034, 0x2, 0x40AC0228, 0x1, 0x40AC0034
#define IOMUXC_PTC13_LPSPI3_PCS2                            0x40AC0034, 0x3, 0x40AC0318, 0x1, 0x40AC0034
#define IOMUXC_PTC13_LPUART7_RTS_b                          0x40AC0034, 0x4, 0x00000000, 0x0, 0x40AC0034
#define IOMUXC_PTC13_LPI2C7_SDA                             0x40AC0034, 0x5, 0x40AC030C, 0x1, 0x40AC0034
#define IOMUXC_PTC13_TPM7_CLKIN                             0x40AC0034, 0x6, 0x40AC02F4, 0x1, 0x40AC0034
#define IOMUXC_PTC13_FB_AD13                                0x40AC0034, 0x9, 0x00000000, 0x0, 0x40AC0034
#define IOMUXC_PTC13_TRACE_D2                               0x40AC0034, 0xA, 0x00000000, 0x0, 0x40AC0034
#define IOMUXC_PTC13_DEBUG_MUX13                            0x40AC0034, 0xE, 0x00000000, 0x0, 0x40AC0034
#define IOMUXC_PTC14_GPIOC14                                0x40AC0038, 0x1, 0x00000000, 0x0, 0x40AC0038
#define IOMUXC_PTC14_FXIO1_D10                              0x40AC0038, 0x2, 0x40AC022C, 0x1, 0x40AC0038
#define IOMUXC_PTC14_LPSPI3_PCS3                            0x40AC0038, 0x3, 0x40AC031C, 0x1, 0x40AC0038
#define IOMUXC_PTC14_LPUART7_TX                             0x40AC0038, 0x4, 0x40AC0270, 0x1, 0x40AC0038
#define IOMUXC_PTC14_LPI2C7_HREQ                            0x40AC0038, 0x5, 0x40AC0304, 0x1, 0x40AC0038
#define IOMUXC_PTC14_TPM7_CH0                               0x40AC0038, 0x6, 0x40AC02DC, 0x1, 0x40AC0038
#define IOMUXC_PTC14_FB_AD14                                0x40AC0038, 0x9, 0x00000000, 0x0, 0x40AC0038
#define IOMUXC_PTC14_TRACE_D1                               0x40AC0038, 0xA, 0x00000000, 0x0, 0x40AC0038
#define IOMUXC_PTC14_DEBUG_MUX14                            0x40AC0038, 0xE, 0x00000000, 0x0, 0x40AC0038
#define IOMUXC_PTC15_GPIOC15                                0x40AC003C, 0x1, 0x00000000, 0x0, 0x40AC003C
#define IOMUXC_PTC15_FXIO1_D11                              0x40AC003C, 0x2, 0x40AC0230, 0x1, 0x40AC003C
#define IOMUXC_PTC15_LPUART7_RX                             0x40AC003C, 0x4, 0x40AC026C, 0x1, 0x40AC003C
#define IOMUXC_PTC15_TPM7_CH1                               0x40AC003C, 0x6, 0x40AC02E0, 0x1, 0x40AC003C
#define IOMUXC_PTC15_FB_AD15                                0x40AC003C, 0x9, 0x00000000, 0x0, 0x40AC003C
#define IOMUXC_PTC15_TRACE_D0                               0x40AC003C, 0xA, 0x00000000, 0x0, 0x40AC003C
#define IOMUXC_PTC15_DEBUG_MUX15                            0x40AC003C, 0xE, 0x00000000, 0x0, 0x40AC003C
#define IOMUXC_PTC16_GPIOC16                                0x40AC0040, 0x1, 0x00000000, 0x0, 0x40AC0040
#define IOMUXC_PTC16_FXIO1_D12                              0x40AC0040, 0x2, 0x40AC0234, 0x1, 0x40AC0040
#define IOMUXC_PTC16_LPSPI3_SIN                             0x40AC0040, 0x3, 0x40AC0324, 0x1, 0x40AC0040
#define IOMUXC_PTC16_TPM7_CH2                               0x40AC0040, 0x6, 0x40AC02E4, 0x1, 0x40AC0040
#define IOMUXC_PTC16_FB_ALE_FB_CS1_b_FB_TS_b                0x40AC0040, 0x9, 0x00000000, 0x0, 0x40AC0040
#define IOMUXC_PTC16_TRACE_CLKOUT                           0x40AC0040, 0xA, 0x00000000, 0x0, 0x40AC0040
#define IOMUXC_PTC16_USB1_OC2                               0x40AC0040, 0xB, 0x40AC0334, 0x1, 0x40AC0040
#define IOMUXC_PTC17_GPIOC17                                0x40AC0044, 0x1, 0x00000000, 0x0, 0x40AC0044
#define IOMUXC_PTC17_FXIO1_D13                              0x40AC0044, 0x2, 0x40AC0238, 0x1, 0x40AC0044
#define IOMUXC_PTC17_LPSPI3_SOUT                            0x40AC0044, 0x3, 0x40AC0328, 0x1, 0x40AC0044
#define IOMUXC_PTC17_TPM6_CLKIN                             0x40AC0044, 0x6, 0x40AC02D8, 0x1, 0x40AC0044
#define IOMUXC_PTC17_FB_CS0_b                               0x40AC0044, 0x9, 0x00000000, 0x0, 0x40AC0044
#define IOMUXC_PTC17_DEBUG_MUX16                            0x40AC0044, 0xE, 0x00000000, 0x0, 0x40AC0044
#define IOMUXC_PTC18_GPIOC18                                0x40AC0048, 0x1, 0x00000000, 0x0, 0x40AC0048
#define IOMUXC_PTC18_FXIO1_D14                              0x40AC0048, 0x2, 0x40AC023C, 0x1, 0x40AC0048
#define IOMUXC_PTC18_LPSPI3_SCK                             0x40AC0048, 0x3, 0x40AC0320, 0x1, 0x40AC0048
#define IOMUXC_PTC18_TPM6_CH0                               0x40AC0048, 0x6, 0x40AC02D0, 0x1, 0x40AC0048
#define IOMUXC_PTC18_FB_OE_b                                0x40AC0048, 0x9, 0x00000000, 0x0, 0x40AC0048
#define IOMUXC_PTC18_DEBUG_MUX17                            0x40AC0048, 0xE, 0x00000000, 0x0, 0x40AC0048
#define IOMUXC_PTC19_GPIOC19                                0x40AC004C, 0x1, 0x00000000, 0x0, 0x40AC004C
#define IOMUXC_PTC19_FXIO1_D15                              0x40AC004C, 0x2, 0x40AC0240, 0x1, 0x40AC004C
#define IOMUXC_PTC19_LPSPI3_PCS0                            0x40AC004C, 0x3, 0x40AC0310, 0x1, 0x40AC004C
#define IOMUXC_PTC19_TPM6_CH1                               0x40AC004C, 0x6, 0x40AC02D4, 0x1, 0x40AC004C
#define IOMUXC_PTC19_FB_A16                                 0x40AC004C, 0x9, 0x00000000, 0x0, 0x40AC004C
#define IOMUXC_PTC19_USB1_PWR2                              0x40AC004C, 0xB, 0x00000000, 0x0, 0x40AC004C
#define IOMUXC_PTD0_GPIOD0                                  0x40AC0080, 0x1, 0x00000000, 0x0, 0x40AC0080
#define IOMUXC_PTD0_SDHC0_RESET_b                           0x40AC0080, 0x8, 0x00000000, 0x0, 0x40AC0080
#define IOMUXC_PTD0_DEBUG_MUX18                             0x40AC0080, 0xE, 0x00000000, 0x0, 0x40AC0080
#define IOMUXC_PTD1_GPIOD1                                  0x40AC0084, 0x1, 0x00000000, 0x0, 0x40AC0084
#define IOMUXC_PTD1_SDHC0_CMD                               0x40AC0084, 0x8, 0x00000000, 0x0, 0x40AC0084
#define IOMUXC_PTD1_DEBUG_MUX19                             0x40AC0084, 0xE, 0x00000000, 0x0, 0x40AC0084
#define IOMUXC_PTD2_GPIOD2                                  0x40AC0088, 0x1, 0x00000000, 0x0, 0x40AC0088
#define IOMUXC_PTD2_SDHC0_CLK                               0x40AC0088, 0x8, 0x00000000, 0x0, 0x40AC0088
#define IOMUXC_PTD2_DEBUG_MUX20                             0x40AC0088, 0xE, 0x00000000, 0x0, 0x40AC0088
#define IOMUXC_PTD3_GPIOD3                                  0x40AC008C, 0x1, 0x00000000, 0x0, 0x40AC008C
#define IOMUXC_PTD3_SDHC0_D7                                0x40AC008C, 0x8, 0x00000000, 0x0, 0x40AC008C
#define IOMUXC_PTD3_DEBUG_MUX21                             0x40AC008C, 0xE, 0x00000000, 0x0, 0x40AC008C
#define IOMUXC_PTD4_GPIOD4                                  0x40AC0090, 0x1, 0x00000000, 0x0, 0x40AC0090
#define IOMUXC_PTD4_SDHC0_D6                                0x40AC0090, 0x8, 0x00000000, 0x0, 0x40AC0090
#define IOMUXC_PTD4_DEBUG_MUX22                             0x40AC0090, 0xE, 0x00000000, 0x0, 0x40AC0090
#define IOMUXC_PTD5_GPIOD5                                  0x40AC0094, 0x1, 0x00000000, 0x0, 0x40AC0094
#define IOMUXC_PTD5_SDHC0_D5                                0x40AC0094, 0x8, 0x00000000, 0x0, 0x40AC0094
#define IOMUXC_PTD5_DEBUG_MUX23                             0x40AC0094, 0xE, 0x00000000, 0x0, 0x40AC0094
#define IOMUXC_PTD6_GPIOD6                                  0x40AC0098, 0x1, 0x00000000, 0x0, 0x40AC0098
#define IOMUXC_PTD6_SDHC0_D4                                0x40AC0098, 0x8, 0x00000000, 0x0, 0x40AC0098
#define IOMUXC_PTD6_DEBUG_MUX24                             0x40AC0098, 0xE, 0x00000000, 0x0, 0x40AC0098
#define IOMUXC_PTD7_GPIOD7                                  0x40AC009C, 0x1, 0x00000000, 0x0, 0x40AC009C
#define IOMUXC_PTD7_SDHC0_D3                                0x40AC009C, 0x8, 0x00000000, 0x0, 0x40AC009C
#define IOMUXC_PTD7_DEBUG_MUX25                             0x40AC009C, 0xE, 0x00000000, 0x0, 0x40AC009C
#define IOMUXC_PTD8_GPIOD8                                  0x40AC00A0, 0x1, 0x00000000, 0x0, 0x40AC00A0
#define IOMUXC_PTD8_TPM4_CLKIN                              0x40AC00A0, 0x6, 0x40AC0298, 0x2, 0x40AC00A0
#define IOMUXC_PTD8_SDHC0_D2                                0x40AC00A0, 0x8, 0x00000000, 0x0, 0x40AC00A0
#define IOMUXC_PTD8_DEBUG_MUX26                             0x40AC00A0, 0xE, 0x00000000, 0x0, 0x40AC00A0
#define IOMUXC_PTD9_GPIOD9                                  0x40AC00A4, 0x1, 0x00000000, 0x0, 0x40AC00A4
#define IOMUXC_PTD9_TPM4_CH0                                0x40AC00A4, 0x6, 0x40AC0280, 0x2, 0x40AC00A4
#define IOMUXC_PTD9_SDHC0_D1                                0x40AC00A4, 0x8, 0x00000000, 0x0, 0x40AC00A4
#define IOMUXC_PTD9_DEBUG_MUX27                             0x40AC00A4, 0xE, 0x00000000, 0x0, 0x40AC00A4
#define IOMUXC_PTD10_GPIOD10                                0x40AC00A8, 0x1, 0x00000000, 0x0, 0x40AC00A8
#define IOMUXC_PTD10_TPM4_CH1                               0x40AC00A8, 0x6, 0x40AC0284, 0x2, 0x40AC00A8
#define IOMUXC_PTD10_SDHC0_D0                               0x40AC00A8, 0x8, 0x00000000, 0x0, 0x40AC00A8
#define IOMUXC_PTD10_DEBUG_MUX28                            0x40AC00A8, 0xE, 0x00000000, 0x0, 0x40AC00A8
#define IOMUXC_PTD11_GPIOD11                                0x40AC00AC, 0x1, 0x00000000, 0x0, 0x40AC00AC
#define IOMUXC_PTD11_TPM4_CH2                               0x40AC00AC, 0x6, 0x40AC0288, 0x2, 0x40AC00AC
#define IOMUXC_PTD11_SDHC0_DQS                              0x40AC00AC, 0x8, 0x00000000, 0x0, 0x40AC00AC
#define IOMUXC_PTD11_DEBUG_MUX29                            0x40AC00AC, 0xE, 0x00000000, 0x0, 0x40AC00AC
#define IOMUXC_PTE0_GPIOE0                                  0x40AC0100, 0x1, 0x00000000, 0x0, 0x40AC0100
#define IOMUXC_PTE0_FXIO1_D31                               0x40AC0100, 0x2, 0x00000000, 0x0, 0x40AC0100
#define IOMUXC_PTE0_LPSPI2_PCS1                             0x40AC0100, 0x3, 0x40AC02A0, 0x2, 0x40AC0100
#define IOMUXC_PTE0_LPUART4_CTS_b                           0x40AC0100, 0x4, 0x40AC0244, 0x2, 0x40AC0100
#define IOMUXC_PTE0_LPI2C4_SCL                              0x40AC0100, 0x5, 0x40AC0278, 0x2, 0x40AC0100
#define IOMUXC_PTE0_SDHC1_D1                                0x40AC0100, 0x8, 0x00000000, 0x0, 0x40AC0100
#define IOMUXC_PTE0_FB_A25                                  0x40AC0100, 0x9, 0x00000000, 0x0, 0x40AC0100
#define IOMUXC_PTE0_DEBUG_MUX30                             0x40AC0100, 0xE, 0x00000000, 0x0, 0x40AC0100
#define IOMUXC_PTE1_GPIOE1                                  0x40AC0104, 0x1, 0x00000000, 0x0, 0x40AC0104
#define IOMUXC_PTE1_FXIO1_D30                               0x40AC0104, 0x2, 0x00000000, 0x0, 0x40AC0104
#define IOMUXC_PTE1_LPSPI2_PCS2                             0x40AC0104, 0x3, 0x40AC02A4, 0x2, 0x40AC0104
#define IOMUXC_PTE1_LPUART4_RTS_b                           0x40AC0104, 0x4, 0x00000000, 0x0, 0x40AC0104
#define IOMUXC_PTE1_LPI2C4_SDA                              0x40AC0104, 0x5, 0x40AC027C, 0x2, 0x40AC0104
#define IOMUXC_PTE1_SDHC1_D0                                0x40AC0104, 0x8, 0x00000000, 0x0, 0x40AC0104
#define IOMUXC_PTE1_FB_A26                                  0x40AC0104, 0x9, 0x00000000, 0x0, 0x40AC0104
#define IOMUXC_PTE1_DEBUG_MUX31                             0x40AC0104, 0xE, 0x00000000, 0x0, 0x40AC0104
#define IOMUXC_PTE2_GPIOE2                                  0x40AC0108, 0x1, 0x00000000, 0x0, 0x40AC0108
#define IOMUXC_PTE2_FXIO1_D29                               0x40AC0108, 0x2, 0x00000000, 0x0, 0x40AC0108
#define IOMUXC_PTE2_LPSPI2_PCS3                             0x40AC0108, 0x3, 0x40AC02A8, 0x2, 0x40AC0108
#define IOMUXC_PTE2_LPUART4_TX                              0x40AC0108, 0x4, 0x40AC024C, 0x2, 0x40AC0108
#define IOMUXC_PTE2_LPI2C4_HREQ                             0x40AC0108, 0x5, 0x40AC0274, 0x2, 0x40AC0108
#define IOMUXC_PTE2_SDHC1_CLK                               0x40AC0108, 0x8, 0x00000000, 0x0, 0x40AC0108
#define IOMUXC_PTE2_DEBUG_MUX32                             0x40AC0108, 0xE, 0x00000000, 0x0, 0x40AC0108
#define IOMUXC_PTE3_GPIOE3                                  0x40AC010C, 0x1, 0x00000000, 0x0, 0x40AC010C
#define IOMUXC_PTE3_FXIO1_D28                               0x40AC010C, 0x2, 0x00000000, 0x0, 0x40AC010C
#define IOMUXC_PTE3_LPUART4_RX                              0x40AC010C, 0x4, 0x40AC0248, 0x2, 0x40AC010C
#define IOMUXC_PTE3_TPM5_CH1                                0x40AC010C, 0x6, 0x40AC02C8, 0x2, 0x40AC010C
#define IOMUXC_PTE3_SDHC1_CMD                               0x40AC010C, 0x8, 0x00000000, 0x0, 0x40AC010C
#define IOMUXC_PTE3_DEBUG_MUX33                             0x40AC010C, 0xE, 0x00000000, 0x0, 0x40AC010C
#define IOMUXC_PTE4_GPIOE4                                  0x40AC0110, 0x1, 0x00000000, 0x0, 0x40AC0110
#define IOMUXC_PTE4_FXIO1_D27                               0x40AC0110, 0x2, 0x00000000, 0x0, 0x40AC0110
#define IOMUXC_PTE4_LPSPI2_SIN                              0x40AC0110, 0x3, 0x40AC02B0, 0x2, 0x40AC0110
#define IOMUXC_PTE4_LPUART5_CTS_b                           0x40AC0110, 0x4, 0x40AC0250, 0x2, 0x40AC0110
#define IOMUXC_PTE4_LPI2C5_SCL                              0x40AC0110, 0x5, 0x40AC02BC, 0x2, 0x40AC0110
#define IOMUXC_PTE4_TPM5_CLKIN                              0x40AC0110, 0x6, 0x40AC02CC, 0x2, 0x40AC0110
#define IOMUXC_PTE4_SDHC1_D3                                0x40AC0110, 0x8, 0x00000000, 0x0, 0x40AC0110
#define IOMUXC_PTE5_GPIOE5                                  0x40AC0114, 0x1, 0x00000000, 0x0, 0x40AC0114
#define IOMUXC_PTE5_FXIO1_D26                               0x40AC0114, 0x2, 0x00000000, 0x0, 0x40AC0114
#define IOMUXC_PTE5_LPSPI2_SOUT                             0x40AC0114, 0x3, 0x40AC02B4, 0x2, 0x40AC0114
#define IOMUXC_PTE5_LPUART5_RTS_b                           0x40AC0114, 0x4, 0x00000000, 0x0, 0x40AC0114
#define IOMUXC_PTE5_LPI2C5_SDA                              0x40AC0114, 0x5, 0x40AC02C0, 0x2, 0x40AC0114
#define IOMUXC_PTE5_TPM5_CH0                                0x40AC0114, 0x6, 0x40AC02C4, 0x2, 0x40AC0114
#define IOMUXC_PTE5_SDHC1_D2                                0x40AC0114, 0x8, 0x00000000, 0x0, 0x40AC0114
#define IOMUXC_PTE6_GPIOE6                                  0x40AC0118, 0x1, 0x00000000, 0x0, 0x40AC0118
#define IOMUXC_PTE6_FXIO1_D25                               0x40AC0118, 0x2, 0x00000000, 0x0, 0x40AC0118
#define IOMUXC_PTE6_LPSPI2_SCK                              0x40AC0118, 0x3, 0x40AC02AC, 0x2, 0x40AC0118
#define IOMUXC_PTE6_LPUART5_TX                              0x40AC0118, 0x4, 0x40AC0258, 0x2, 0x40AC0118
#define IOMUXC_PTE6_LPI2C5_HREQ                             0x40AC0118, 0x5, 0x40AC02B8, 0x2, 0x40AC0118
#define IOMUXC_PTE6_TPM7_CH3                                0x40AC0118, 0x6, 0x40AC02E8, 0x2, 0x40AC0118
#define IOMUXC_PTE6_SDHC1_D4                                0x40AC0118, 0x8, 0x00000000, 0x0, 0x40AC0118
#define IOMUXC_PTE6_FB_A17                                  0x40AC0118, 0x9, 0x00000000, 0x0, 0x40AC0118
#define IOMUXC_PTE6_USB0_OC                                 0x40AC0118, 0xB, 0x40AC0330, 0x1, 0x40AC0118
#define IOMUXC_PTE7_GPIOE7                                  0x40AC011C, 0x1, 0x00000000, 0x0, 0x40AC011C
#define IOMUXC_PTE7_FXIO1_D24                               0x40AC011C, 0x2, 0x00000000, 0x0, 0x40AC011C
#define IOMUXC_PTE7_LPSPI2_PCS0                             0x40AC011C, 0x3, 0x40AC029C, 0x2, 0x40AC011C
#define IOMUXC_PTE7_LPUART5_RX                              0x40AC011C, 0x4, 0x40AC0254, 0x2, 0x40AC011C
#define IOMUXC_PTE7_TPM7_CH4                                0x40AC011C, 0x6, 0x40AC02EC, 0x2, 0x40AC011C
#define IOMUXC_PTE7_SDHC1_D5                                0x40AC011C, 0x8, 0x00000000, 0x0, 0x40AC011C
#define IOMUXC_PTE7_FB_A18                                  0x40AC011C, 0x9, 0x00000000, 0x0, 0x40AC011C
#define IOMUXC_PTE7_TRACE_D7                                0x40AC011C, 0xA, 0x00000000, 0x0, 0x40AC011C
#define IOMUXC_PTE7_USB0_PWR                                0x40AC011C, 0xB, 0x00000000, 0x0, 0x40AC011C
#define IOMUXC_PTE7_VIU_FID                                 0x40AC011C, 0xC, 0x00000000, 0x0, 0x40AC011C
#define IOMUXC_PTE8_GPIOE8                                  0x40AC0120, 0x1, 0x00000000, 0x0, 0x40AC0120
#define IOMUXC_PTE8_TRACE_D6                                0x40AC0120, 0xA, 0x00000000, 0x0, 0x40AC0120
#define IOMUXC_PTE8_VIU_D16                                 0x40AC0120, 0xC, 0x00000000, 0x0, 0x40AC0120
#define IOMUXC_PTE8_FXIO1_D23                               0x40AC0120, 0x2, 0x00000000, 0x0, 0x40AC0120
#define IOMUXC_PTE8_LPSPI3_PCS1                             0x40AC0120, 0x3, 0x40AC0314, 0x2, 0x40AC0120
#define IOMUXC_PTE8_LPUART6_CTS_b                           0x40AC0120, 0x4, 0x40AC025C, 0x2, 0x40AC0120
#define IOMUXC_PTE8_LPI2C6_SCL                              0x40AC0120, 0x5, 0x40AC02FC, 0x2, 0x40AC0120
#define IOMUXC_PTE8_TPM7_CH5                                0x40AC0120, 0x6, 0x40AC02F0, 0x2, 0x40AC0120
#define IOMUXC_PTE8_SDHC1_WP                                0x40AC0120, 0x7, 0x40AC0200, 0x1, 0x40AC0120
#define IOMUXC_PTE8_SDHC1_D6                                0x40AC0120, 0x8, 0x00000000, 0x0, 0x40AC0120
#define IOMUXC_PTE8_FB_CS3_b_FB_BE7_0_BLS31_24_b            0x40AC0120, 0x9, 0x00000000, 0x0, 0x40AC0120
#define IOMUXC_PTE9_GPIOE9                                  0x40AC0124, 0x1, 0x00000000, 0x0, 0x40AC0124
#define IOMUXC_PTE9_TRACE_D5                                0x40AC0124, 0xA, 0x00000000, 0x0, 0x40AC0124
#define IOMUXC_PTE9_VIU_D17                                 0x40AC0124, 0xC, 0x00000000, 0x0, 0x40AC0124
#define IOMUXC_PTE9_FXIO1_D22                               0x40AC0124, 0x2, 0x00000000, 0x0, 0x40AC0124
#define IOMUXC_PTE9_LPSPI3_PCS2                             0x40AC0124, 0x3, 0x40AC0318, 0x2, 0x40AC0124
#define IOMUXC_PTE9_LPUART6_RTS_b                           0x40AC0124, 0x4, 0x00000000, 0x0, 0x40AC0124
#define IOMUXC_PTE9_LPI2C6_SDA                              0x40AC0124, 0x5, 0x40AC0300, 0x2, 0x40AC0124
#define IOMUXC_PTE9_TPM7_CLKIN                              0x40AC0124, 0x6, 0x40AC02F4, 0x2, 0x40AC0124
#define IOMUXC_PTE9_SDHC1_CD                                0x40AC0124, 0x7, 0x40AC032C, 0x1, 0x40AC0124
#define IOMUXC_PTE9_SDHC1_D7                                0x40AC0124, 0x8, 0x00000000, 0x0, 0x40AC0124
#define IOMUXC_PTE9_FB_TBST_b_FB_CS2_b_FB_BE15_8_BLS23_16_b 0x40AC0124, 0x9, 0x00000000, 0x0, 0x40AC0124
#define IOMUXC_PTE10_GPIOE10                                0x40AC0128, 0x1, 0x00000000, 0x0, 0x40AC0128
#define IOMUXC_PTE10_TRACE_D4                               0x40AC0128, 0xA, 0x00000000, 0x0, 0x40AC0128
#define IOMUXC_PTE10_VIU_D18                                0x40AC0128, 0xC, 0x00000000, 0x0, 0x40AC0128
#define IOMUXC_PTE10_FXIO1_D21                              0x40AC0128, 0x2, 0x00000000, 0x0, 0x40AC0128
#define IOMUXC_PTE10_LPSPI3_PCS3                            0x40AC0128, 0x3, 0x40AC031C, 0x2, 0x40AC0128
#define IOMUXC_PTE10_LPUART6_TX                             0x40AC0128, 0x4, 0x40AC0264, 0x2, 0x40AC0128
#define IOMUXC_PTE10_LPI2C6_HREQ                            0x40AC0128, 0x5, 0x40AC02F8, 0x2, 0x40AC0128
#define IOMUXC_PTE10_TPM7_CH0                               0x40AC0128, 0x6, 0x40AC02DC, 0x2, 0x40AC0128
#define IOMUXC_PTE10_SDHC1_VS                               0x40AC0128, 0x7, 0x00000000, 0x0, 0x40AC0128
#define IOMUXC_PTE10_SDHC1_DQS                              0x40AC0128, 0x8, 0x00000000, 0x0, 0x40AC0128
#define IOMUXC_PTE10_FB_A19                                 0x40AC0128, 0x9, 0x00000000, 0x0, 0x40AC0128
#define IOMUXC_PTE11_GPIOE11                                0x40AC012C, 0x1, 0x00000000, 0x0, 0x40AC012C
#define IOMUXC_PTE11_TRACE_D3                               0x40AC012C, 0xA, 0x00000000, 0x0, 0x40AC012C
#define IOMUXC_PTE11_VIU_D19                                0x40AC012C, 0xC, 0x00000000, 0x0, 0x40AC012C
#define IOMUXC_PTE11_FXIO1_D20                              0x40AC012C, 0x2, 0x00000000, 0x0, 0x40AC012C
#define IOMUXC_PTE11_LPUART6_RX                             0x40AC012C, 0x4, 0x40AC0260, 0x2, 0x40AC012C
#define IOMUXC_PTE11_TPM7_CH1                               0x40AC012C, 0x6, 0x40AC02E0, 0x2, 0x40AC012C
#define IOMUXC_PTE11_SDHC1_RESET_b                          0x40AC012C, 0x8, 0x00000000, 0x0, 0x40AC012C
#define IOMUXC_PTE11_FB_A20                                 0x40AC012C, 0x9, 0x00000000, 0x0, 0x40AC012C
#define IOMUXC_PTE12_GPIOE12                                0x40AC0130, 0x1, 0x00000000, 0x0, 0x40AC0130
#define IOMUXC_PTE12_FXIO1_D19                              0x40AC0130, 0x2, 0x00000000, 0x0, 0x40AC0130
#define IOMUXC_PTE12_LPSPI3_SIN                             0x40AC0130, 0x3, 0x40AC0324, 0x2, 0x40AC0130
#define IOMUXC_PTE12_LPUART7_CTS_b                          0x40AC0130, 0x4, 0x40AC0268, 0x2, 0x40AC0130
#define IOMUXC_PTE12_LPI2C7_SCL                             0x40AC0130, 0x5, 0x40AC0308, 0x2, 0x40AC0130
#define IOMUXC_PTE12_TPM7_CH2                               0x40AC0130, 0x6, 0x40AC02E4, 0x2, 0x40AC0130
#define IOMUXC_PTE12_SDHC1_WP                               0x40AC0130, 0x8, 0x40AC0200, 0x2, 0x40AC0130
#define IOMUXC_PTE12_FB_A21                                 0x40AC0130, 0x9, 0x00000000, 0x0, 0x40AC0130
#define IOMUXC_PTE12_TRACE_D2                               0x40AC0130, 0xA, 0x00000000, 0x0, 0x40AC0130
#define IOMUXC_PTE12_USB1_OC2                               0x40AC0130, 0xB, 0x40AC0334, 0x2, 0x40AC0130
#define IOMUXC_PTE12_VIU_D20                                0x40AC0130, 0xC, 0x00000000, 0x0, 0x40AC0130
#define IOMUXC_PTE13_GPIOE13                                0x40AC0134, 0x1, 0x00000000, 0x0, 0x40AC0134
#define IOMUXC_PTE13_FXIO1_D18                              0x40AC0134, 0x2, 0x00000000, 0x0, 0x40AC0134
#define IOMUXC_PTE13_LPSPI3_SOUT                            0x40AC0134, 0x3, 0x40AC0328, 0x2, 0x40AC0134
#define IOMUXC_PTE13_LPUART7_RTS_b                          0x40AC0134, 0x4, 0x00000000, 0x0, 0x40AC0134
#define IOMUXC_PTE13_LPI2C7_SDA                             0x40AC0134, 0x5, 0x40AC030C, 0x2, 0x40AC0134
#define IOMUXC_PTE13_TPM6_CLKIN                             0x40AC0134, 0x6, 0x40AC02D8, 0x2, 0x40AC0134
#define IOMUXC_PTE13_SDHC1_CD                               0x40AC0134, 0x8, 0x40AC032C, 0x2, 0x40AC0134
#define IOMUXC_PTE13_FB_A22                                 0x40AC0134, 0x9, 0x00000000, 0x0, 0x40AC0134
#define IOMUXC_PTE13_TRACE_D1                               0x40AC0134, 0xA, 0x00000000, 0x0, 0x40AC0134
#define IOMUXC_PTE13_USB1_PWR2                              0x40AC0134, 0xB, 0x00000000, 0x0, 0x40AC0134
#define IOMUXC_PTE13_VIU_D21                                0x40AC0134, 0xC, 0x00000000, 0x0, 0x40AC0134
#define IOMUXC_PTE14_GPIOE14                                0x40AC0138, 0x1, 0x00000000, 0x0, 0x40AC0138
#define IOMUXC_PTE14_FXIO1_D17                              0x40AC0138, 0x2, 0x00000000, 0x0, 0x40AC0138
#define IOMUXC_PTE14_LPSPI3_SCK                             0x40AC0138, 0x3, 0x40AC0320, 0x2, 0x40AC0138
#define IOMUXC_PTE14_LPUART7_TX                             0x40AC0138, 0x4, 0x40AC0270, 0x2, 0x40AC0138
#define IOMUXC_PTE14_LPI2C7_HREQ                            0x40AC0138, 0x5, 0x40AC0304, 0x2, 0x40AC0138
#define IOMUXC_PTE14_TPM6_CH0                               0x40AC0138, 0x6, 0x40AC02D0, 0x2, 0x40AC0138
#define IOMUXC_PTE14_SDHC1_VS                               0x40AC0138, 0x8, 0x00000000, 0x0, 0x40AC0138
#define IOMUXC_PTE14_FB_A23                                 0x40AC0138, 0x9, 0x00000000, 0x0, 0x40AC0138
#define IOMUXC_PTE14_TRACE_D0                               0x40AC0138, 0xA, 0x00000000, 0x0, 0x40AC0138
#define IOMUXC_PTE14_USB0_OC                                0x40AC0138, 0xB, 0x40AC0330, 0x2, 0x40AC0138
#define IOMUXC_PTE14_VIU_D22                                0x40AC0138, 0xC, 0x00000000, 0x0, 0x40AC0138
#define IOMUXC_PTE15_GPIOE15                                0x40AC013C, 0x1, 0x00000000, 0x0, 0x40AC013C
#define IOMUXC_PTE15_FXIO1_D16                              0x40AC013C, 0x2, 0x00000000, 0x0, 0x40AC013C
#define IOMUXC_PTE15_LPSPI3_PCS0                            0x40AC013C, 0x3, 0x40AC0310, 0x2, 0x40AC013C
#define IOMUXC_PTE15_LPUART7_RX                             0x40AC013C, 0x4, 0x40AC026C, 0x2, 0x40AC013C
#define IOMUXC_PTE15_TPM6_CH1                               0x40AC013C, 0x6, 0x40AC02D4, 0x2, 0x40AC013C
#define IOMUXC_PTE15_FB_A24                                 0x40AC013C, 0x9, 0x00000000, 0x0, 0x40AC013C
#define IOMUXC_PTE15_TRACE_CLKOUT                           0x40AC013C, 0xA, 0x00000000, 0x0, 0x40AC013C
#define IOMUXC_PTE15_USB0_PWR                               0x40AC013C, 0xB, 0x00000000, 0x0, 0x40AC013C
#define IOMUXC_PTE15_VIU_D23                                0x40AC013C, 0xC, 0x00000000, 0x0, 0x40AC013C
#define IOMUXC_PTF0_GPIOF0                                  0x40AC0180, 0x1, 0x00000000, 0x0, 0x40AC0180
#define IOMUXC_PTF0_LPUART4_CTS_b                           0x40AC0180, 0x4, 0x40AC0244, 0x3, 0x40AC0180
#define IOMUXC_PTF0_LPI2C4_SCL                              0x40AC0180, 0x5, 0x40AC0278, 0x3, 0x40AC0180
#define IOMUXC_PTF0_TPM4_CLKIN                              0x40AC0180, 0x6, 0x40AC0298, 0x3, 0x40AC0180
#define IOMUXC_PTF0_FB_RW_b                                 0x40AC0180, 0x9, 0x00000000, 0x0, 0x40AC0180
#define IOMUXC_PTF0_VIU_DE                                  0x40AC0180, 0xC, 0x00000000, 0x0, 0x40AC0180
#define IOMUXC_PTF0_BT1_CFG0                                0x40AC0180, 0x0, 0x00000000, 0x0, 0x40AC0180
#define IOMUXC_PTF1_GPIOF1                                  0x40AC0184, 0x1, 0x00000000, 0x0, 0x40AC0184
#define IOMUXC_PTF1_LPUART4_RTS_b                           0x40AC0184, 0x4, 0x00000000, 0x0, 0x40AC0184
#define IOMUXC_PTF1_LPI2C4_SDA                              0x40AC0184, 0x5, 0x40AC027C, 0x3, 0x40AC0184
#define IOMUXC_PTF1_TPM4_CH0                                0x40AC0184, 0x6, 0x40AC0280, 0x3, 0x40AC0184
#define IOMUXC_PTF1_CLKOUT                                  0x40AC0184, 0x9, 0x00000000, 0x0, 0x40AC0184
#define IOMUXC_PTF1_VIU_HSYNC                               0x40AC0184, 0xC, 0x00000000, 0x0, 0x40AC0184
#define IOMUXC_PTF1_BT1_CFG1                                0x40AC0184, 0x0, 0x00000000, 0x0, 0x40AC0184
#define IOMUXC_PTF2_GPIOF2                                  0x40AC0188, 0x1, 0x00000000, 0x0, 0x40AC0188
#define IOMUXC_PTF2_LPUART4_TX                              0x40AC0188, 0x4, 0x40AC024C, 0x3, 0x40AC0188
#define IOMUXC_PTF2_LPI2C4_HREQ                             0x40AC0188, 0x5, 0x40AC0274, 0x3, 0x40AC0188
#define IOMUXC_PTF2_TPM4_CH1                                0x40AC0188, 0x6, 0x40AC0284, 0x3, 0x40AC0188
#define IOMUXC_PTF2_FB_TSIZ1_FB_CS5_b_FB_BE23_16_BLS15_8_b  0x40AC0188, 0x9, 0x00000000, 0x0, 0x40AC0188
#define IOMUXC_PTF2_VIU_VSYNC                               0x40AC0188, 0xC, 0x00000000, 0x0, 0x40AC0188
#define IOMUXC_PTF2_BT1_CFG2                                0x40AC0188, 0x0, 0x00000000, 0x0, 0x40AC0188
#define IOMUXC_PTF3_GPIOF3                                  0x40AC018C, 0x1, 0x00000000, 0x0, 0x40AC018C
#define IOMUXC_PTF3_LPUART4_RX                              0x40AC018C, 0x4, 0x40AC0248, 0x3, 0x40AC018C
#define IOMUXC_PTF3_TPM4_CH2                                0x40AC018C, 0x6, 0x40AC0288, 0x3, 0x40AC018C
#define IOMUXC_PTF3_FB_AD16                                 0x40AC018C, 0x9, 0x00000000, 0x0, 0x40AC018C
#define IOMUXC_PTF3_VIU_PCLK                                0x40AC018C, 0xC, 0x00000000, 0x0, 0x40AC018C
#define IOMUXC_PTF3_BT1_CFG3                                0x40AC018C, 0x0, 0x00000000, 0x0, 0x40AC018C
#define IOMUXC_PTF4_GPIOF4                                  0x40AC0190, 0x1, 0x00000000, 0x0, 0x40AC0190
#define IOMUXC_PTF4_FXIO1_D0                                0x40AC0190, 0x2, 0x40AC0204, 0x2, 0x40AC0190
#define IOMUXC_PTF4_LPSPI2_PCS1                             0x40AC0190, 0x3, 0x40AC02A0, 0x3, 0x40AC0190
#define IOMUXC_PTF4_LPUART5_CTS_b                           0x40AC0190, 0x4, 0x40AC0250, 0x3, 0x40AC0190
#define IOMUXC_PTF4_LPI2C5_SCL                              0x40AC0190, 0x5, 0x40AC02BC, 0x3, 0x40AC0190
#define IOMUXC_PTF4_TPM4_CH3                                0x40AC0190, 0x6, 0x40AC028C, 0x2, 0x40AC0190
#define IOMUXC_PTF4_FB_AD17                                 0x40AC0190, 0x9, 0x00000000, 0x0, 0x40AC0190
#define IOMUXC_PTF4_VIU_D0                                  0x40AC0190, 0xC, 0x00000000, 0x0, 0x40AC0190
#define IOMUXC_PTF4_BT1_CFG4                                0x40AC0190, 0x0, 0x00000000, 0x0, 0x40AC0190
#define IOMUXC_PTF5_GPIOF5                                  0x40AC0194, 0x1, 0x00000000, 0x0, 0x40AC0194
#define IOMUXC_PTF5_FXIO1_D1                                0x40AC0194, 0x2, 0x40AC0208, 0x2, 0x40AC0194
#define IOMUXC_PTF5_LPSPI2_PCS2                             0x40AC0194, 0x3, 0x40AC02A4, 0x3, 0x40AC0194
#define IOMUXC_PTF5_LPUART5_RTS_b                           0x40AC0194, 0x4, 0x00000000, 0x0, 0x40AC0194
#define IOMUXC_PTF5_LPI2C5_SDA                              0x40AC0194, 0x5, 0x40AC02C0, 0x3, 0x40AC0194
#define IOMUXC_PTF5_TPM4_CH4                                0x40AC0194, 0x6, 0x40AC0290, 0x2, 0x40AC0194
#define IOMUXC_PTF5_FB_AD18                                 0x40AC0194, 0x9, 0x00000000, 0x0, 0x40AC0194
#define IOMUXC_PTF5_VIU_D1                                  0x40AC0194, 0xC, 0x00000000, 0x0, 0x40AC0194
#define IOMUXC_PTF5_BT1_CFG5                                0x40AC0194, 0x0, 0x00000000, 0x0, 0x40AC0194
#define IOMUXC_PTF6_GPIOF6                                  0x40AC0198, 0x1, 0x00000000, 0x0, 0x40AC0198
#define IOMUXC_PTF6_FXIO1_D2                                0x40AC0198, 0x2, 0x40AC020C, 0x2, 0x40AC0198
#define IOMUXC_PTF6_LPSPI2_PCS3                             0x40AC0198, 0x3, 0x40AC02A8, 0x3, 0x40AC0198
#define IOMUXC_PTF6_LPUART5_TX                              0x40AC0198, 0x4, 0x40AC0258, 0x3, 0x40AC0198
#define IOMUXC_PTF6_LPI2C5_HREQ                             0x40AC0198, 0x5, 0x40AC02B8, 0x3, 0x40AC0198
#define IOMUXC_PTF6_TPM4_CH5                                0x40AC0198, 0x6, 0x40AC0294, 0x2, 0x40AC0198
#define IOMUXC_PTF6_FB_AD19                                 0x40AC0198, 0x9, 0x00000000, 0x0, 0x40AC0198
#define IOMUXC_PTF6_VIU_D2                                  0x40AC0198, 0xC, 0x00000000, 0x0, 0x40AC0198
#define IOMUXC_PTF6_BT1_CFG6                                0x40AC0198, 0x0, 0x00000000, 0x0, 0x40AC0198
#define IOMUXC_PTF7_GPIOF7                                  0x40AC019C, 0x1, 0x00000000, 0x0, 0x40AC019C
#define IOMUXC_PTF7_FXIO1_D3                                0x40AC019C, 0x2, 0x40AC0210, 0x2, 0x40AC019C
#define IOMUXC_PTF7_LPUART5_RX                              0x40AC019C, 0x4, 0x40AC0254, 0x3, 0x40AC019C
#define IOMUXC_PTF7_TPM5_CH1                                0x40AC019C, 0x6, 0x40AC02C8, 0x3, 0x40AC019C
#define IOMUXC_PTF7_FB_AD20                                 0x40AC019C, 0x9, 0x00000000, 0x0, 0x40AC019C
#define IOMUXC_PTF7_VIU_D3                                  0x40AC019C, 0xC, 0x00000000, 0x0, 0x40AC019C
#define IOMUXC_PTF7_BT1_CFG7                                0x40AC019C, 0x0, 0x00000000, 0x0, 0x40AC019C
#define IOMUXC_PTF8_GPIOF8                                  0x40AC01A0, 0x1, 0x00000000, 0x0, 0x40AC01A0
#define IOMUXC_PTF8_FXIO1_D4                                0x40AC01A0, 0x2, 0x40AC0214, 0x2, 0x40AC01A0
#define IOMUXC_PTF8_LPSPI2_SIN                              0x40AC01A0, 0x3, 0x40AC02B0, 0x3, 0x40AC01A0
#define IOMUXC_PTF8_LPUART6_CTS_b                           0x40AC01A0, 0x4, 0x40AC025C, 0x3, 0x40AC01A0
#define IOMUXC_PTF8_LPI2C6_SCL                              0x40AC01A0, 0x5, 0x40AC02FC, 0x3, 0x40AC01A0
#define IOMUXC_PTF8_TPM5_CLKIN                              0x40AC01A0, 0x6, 0x40AC02CC, 0x3, 0x40AC01A0
#define IOMUXC_PTF8_FB_AD21                                 0x40AC01A0, 0x9, 0x00000000, 0x0, 0x40AC01A0
#define IOMUXC_PTF8_USB1_CLK                                0x40AC01A0, 0xB, 0x00000000, 0x0, 0x40AC01A0
#define IOMUXC_PTF8_VIU_D4                                  0x40AC01A0, 0xC, 0x00000000, 0x0, 0x40AC01A0
#define IOMUXC_PTF8_BT1_CFG8                                0x40AC01A0, 0x0, 0x00000000, 0x0, 0x40AC01A0
#define IOMUXC_PTF9_GPIOF9                                  0x40AC01A4, 0x1, 0x00000000, 0x0, 0x40AC01A4
#define IOMUXC_PTF9_FXIO1_D5                                0x40AC01A4, 0x2, 0x40AC0218, 0x2, 0x40AC01A4
#define IOMUXC_PTF9_LPSPI2_SOUT                             0x40AC01A4, 0x3, 0x40AC02B4, 0x3, 0x40AC01A4
#define IOMUXC_PTF9_LPUART6_RTS_b                           0x40AC01A4, 0x4, 0x00000000, 0x0, 0x40AC01A4
#define IOMUXC_PTF9_LPI2C6_SDA                              0x40AC01A4, 0x5, 0x40AC0300, 0x3, 0x40AC01A4
#define IOMUXC_PTF9_TPM5_CH0                                0x40AC01A4, 0x6, 0x40AC02C4, 0x3, 0x40AC01A4
#define IOMUXC_PTF9_FB_AD22                                 0x40AC01A4, 0x9, 0x00000000, 0x0, 0x40AC01A4
#define IOMUXC_PTF9_USB1_NXT                                0x40AC01A4, 0xB, 0x00000000, 0x0, 0x40AC01A4
#define IOMUXC_PTF9_VIU_D5                                  0x40AC01A4, 0xC, 0x00000000, 0x0, 0x40AC01A4
#define IOMUXC_PTF9_BT1_CFG9                                0x40AC01A4, 0x0, 0x00000000, 0x0, 0x40AC01A4
#define IOMUXC_PTF10_GPIOF10                                0x40AC01A8, 0x1, 0x00000000, 0x0, 0x40AC01A8
#define IOMUXC_PTF10_FXIO1_D6                               0x40AC01A8, 0x2, 0x40AC021C, 0x2, 0x40AC01A8
#define IOMUXC_PTF10_LPSPI2_SCK                             0x40AC01A8, 0x3, 0x40AC02AC, 0x3, 0x40AC01A8
#define IOMUXC_PTF10_LPUART6_TX                             0x40AC01A8, 0x4, 0x40AC0264, 0x3, 0x40AC01A8
#define IOMUXC_PTF10_LPI2C6_HREQ                            0x40AC01A8, 0x5, 0x40AC02F8, 0x3, 0x40AC01A8
#define IOMUXC_PTF10_TPM7_CH3                               0x40AC01A8, 0x6, 0x40AC02E8, 0x3, 0x40AC01A8
#define IOMUXC_PTF10_FB_AD23                                0x40AC01A8, 0x9, 0x00000000, 0x0, 0x40AC01A8
#define IOMUXC_PTF10_USB1_STP                               0x40AC01A8, 0xB, 0x00000000, 0x0, 0x40AC01A8
#define IOMUXC_PTF10_VIU_D6                                 0x40AC01A8, 0xC, 0x00000000, 0x0, 0x40AC01A8
#define IOMUXC_PTF10_BT1_CFG10                              0x40AC01A8, 0x0, 0x00000000, 0x0, 0x40AC01A8
#define IOMUXC_PTF11_GPIOF11                                0x40AC01AC, 0x1, 0x00000000, 0x0, 0x40AC01AC
#define IOMUXC_PTF11_FXIO1_D7                               0x40AC01AC, 0x2, 0x40AC0220, 0x2, 0x40AC01AC
#define IOMUXC_PTF11_LPSPI2_PCS0                            0x40AC01AC, 0x3, 0x40AC029C, 0x3, 0x40AC01AC
#define IOMUXC_PTF11_LPUART6_RX                             0x40AC01AC, 0x4, 0x40AC0260, 0x3, 0x40AC01AC
#define IOMUXC_PTF11_TPM7_CH4                               0x40AC01AC, 0x6, 0x40AC02EC, 0x3, 0x40AC01AC
#define IOMUXC_PTF11_FB_CS4_b_FB_TSIZ0_FB_BE31_24_BLS7_0_b  0x40AC01AC, 0x9, 0x00000000, 0x0, 0x40AC01AC
#define IOMUXC_PTF11_USB1_DIR                               0x40AC01AC, 0xB, 0x00000000, 0x0, 0x40AC01AC
#define IOMUXC_PTF11_VIU_D7                                 0x40AC01AC, 0xC, 0x00000000, 0x0, 0x40AC01AC
#define IOMUXC_PTF11_BT1_CFG11                              0x40AC01AC, 0x0, 0x00000000, 0x0, 0x40AC01AC
#define IOMUXC_PTF12_GPIOF12                                0x40AC01B0, 0x1, 0x00000000, 0x0, 0x40AC01B0
#define IOMUXC_PTF12_FXIO1_D8                               0x40AC01B0, 0x2, 0x40AC0224, 0x2, 0x40AC01B0
#define IOMUXC_PTF12_LPSPI3_PCS1                            0x40AC01B0, 0x3, 0x40AC0314, 0x3, 0x40AC01B0
#define IOMUXC_PTF12_LPUART7_CTS_b                          0x40AC01B0, 0x4, 0x40AC0268, 0x3, 0x40AC01B0
#define IOMUXC_PTF12_LPI2C7_SCL                             0x40AC01B0, 0x5, 0x40AC0308, 0x3, 0x40AC01B0
#define IOMUXC_PTF12_TPM7_CH5                               0x40AC01B0, 0x6, 0x40AC02F0, 0x3, 0x40AC01B0
#define IOMUXC_PTF12_FB_AD24                                0x40AC01B0, 0x9, 0x00000000, 0x0, 0x40AC01B0
#define IOMUXC_PTF12_USB1_DATA0                             0x40AC01B0, 0xB, 0x00000000, 0x0, 0x40AC01B0
#define IOMUXC_PTF12_VIU_D8                                 0x40AC01B0, 0xC, 0x00000000, 0x0, 0x40AC01B0
#define IOMUXC_PTF12_BT1_CFG12                              0x40AC01B0, 0x0, 0x00000000, 0x0, 0x40AC01B0
#define IOMUXC_PTF13_GPIOF13                                0x40AC01B4, 0x1, 0x00000000, 0x0, 0x40AC01B4
#define IOMUXC_PTF13_FXIO1_D9                               0x40AC01B4, 0x2, 0x40AC0228, 0x2, 0x40AC01B4
#define IOMUXC_PTF13_LPSPI3_PCS2                            0x40AC01B4, 0x3, 0x40AC0318, 0x3, 0x40AC01B4
#define IOMUXC_PTF13_LPUART7_RTS_b                          0x40AC01B4, 0x4, 0x00000000, 0x0, 0x40AC01B4
#define IOMUXC_PTF13_LPI2C7_SDA                             0x40AC01B4, 0x5, 0x40AC030C, 0x3, 0x40AC01B4
#define IOMUXC_PTF13_TPM7_CLKIN                             0x40AC01B4, 0x6, 0x40AC02F4, 0x3, 0x40AC01B4
#define IOMUXC_PTF13_FB_AD25                                0x40AC01B4, 0x9, 0x00000000, 0x0, 0x40AC01B4
#define IOMUXC_PTF13_USB1_DATA1                             0x40AC01B4, 0xB, 0x00000000, 0x0, 0x40AC01B4
#define IOMUXC_PTF13_VIU_D9                                 0x40AC01B4, 0xC, 0x00000000, 0x0, 0x40AC01B4
#define IOMUXC_PTF13_BT1_CFG13                              0x40AC01B4, 0x0, 0x00000000, 0x0, 0x40AC01B4
#define IOMUXC_PTF14_GPIOF14                                0x40AC01B8, 0x1, 0x00000000, 0x0, 0x40AC01B8
#define IOMUXC_PTF14_FXIO1_D10                              0x40AC01B8, 0x2, 0x40AC022C, 0x2, 0x40AC01B8
#define IOMUXC_PTF14_LPSPI3_PCS3                            0x40AC01B8, 0x3, 0x40AC031C, 0x3, 0x40AC01B8
#define IOMUXC_PTF14_LPUART7_TX                             0x40AC01B8, 0x4, 0x40AC0270, 0x3, 0x40AC01B8
#define IOMUXC_PTF14_LPI2C7_HREQ                            0x40AC01B8, 0x5, 0x40AC0304, 0x3, 0x40AC01B8
#define IOMUXC_PTF14_TPM7_CH0                               0x40AC01B8, 0x6, 0x40AC02DC, 0x3, 0x40AC01B8
#define IOMUXC_PTF14_FB_AD26                                0x40AC01B8, 0x9, 0x00000000, 0x0, 0x40AC01B8
#define IOMUXC_PTF14_USB1_DATA2                             0x40AC01B8, 0xB, 0x00000000, 0x0, 0x40AC01B8
#define IOMUXC_PTF14_VIU_D10                                0x40AC01B8, 0xC, 0x00000000, 0x0, 0x40AC01B8
#define IOMUXC_PTF14_BT1_CFG14                              0x40AC01B8, 0x0, 0x00000000, 0x0, 0x40AC01B8
#define IOMUXC_PTF15_GPIOF15                                0x40AC01BC, 0x1, 0x00000000, 0x0, 0x40AC01BC
#define IOMUXC_PTF15_FXIO1_D11                              0x40AC01BC, 0x2, 0x40AC0230, 0x2, 0x40AC01BC
#define IOMUXC_PTF15_LPUART7_RX                             0x40AC01BC, 0x4, 0x40AC026C, 0x3, 0x40AC01BC
#define IOMUXC_PTF15_TPM7_CH1                               0x40AC01BC, 0x6, 0x40AC02E0, 0x3, 0x40AC01BC
#define IOMUXC_PTF15_FB_AD27                                0x40AC01BC, 0x9, 0x00000000, 0x0, 0x40AC01BC
#define IOMUXC_PTF15_USB1_DATA3                             0x40AC01BC, 0xB, 0x00000000, 0x0, 0x40AC01BC
#define IOMUXC_PTF15_VIU_D11                                0x40AC01BC, 0xC, 0x00000000, 0x0, 0x40AC01BC
#define IOMUXC_PTF15_BT1_CFG15                              0x40AC01BC, 0x0, 0x00000000, 0x0, 0x40AC01BC
#define IOMUXC_PTF16_GPIOF16                                0x40AC01C0, 0x1, 0x00000000, 0x0, 0x40AC01C0
#define IOMUXC_PTF16_USB1_DATA4                             0x40AC01C0, 0xB, 0x00000000, 0x0, 0x40AC01C0
#define IOMUXC_PTF16_VIU_D12                                0x40AC01C0, 0xC, 0x00000000, 0x0, 0x40AC01C0
#define IOMUXC_PTF16_FXIO1_D12                              0x40AC01C0, 0x2, 0x40AC0234, 0x2, 0x40AC01C0
#define IOMUXC_PTF16_LPSPI3_SIN                             0x40AC01C0, 0x3, 0x40AC0324, 0x3, 0x40AC01C0
#define IOMUXC_PTF16_TPM7_CH2                               0x40AC01C0, 0x6, 0x40AC02E4, 0x3, 0x40AC01C0
#define IOMUXC_PTF16_FB_AD28                                0x40AC01C0, 0x9, 0x00000000, 0x0, 0x40AC01C0
#define IOMUXC_PTF17_GPIOF17                                0x40AC01C4, 0x1, 0x00000000, 0x0, 0x40AC01C4
#define IOMUXC_PTF17_USB1_DATA5                             0x40AC01C4, 0xB, 0x00000000, 0x0, 0x40AC01C4
#define IOMUXC_PTF17_VIU_D13                                0x40AC01C4, 0xC, 0x00000000, 0x0, 0x40AC01C4
#define IOMUXC_PTF17_FXIO1_D13                              0x40AC01C4, 0x2, 0x40AC0238, 0x2, 0x40AC01C4
#define IOMUXC_PTF17_LPSPI3_SOUT                            0x40AC01C4, 0x3, 0x40AC0328, 0x3, 0x40AC01C4
#define IOMUXC_PTF17_TPM6_CLKIN                             0x40AC01C4, 0x6, 0x40AC02D8, 0x3, 0x40AC01C4
#define IOMUXC_PTF17_FB_AD29                                0x40AC01C4, 0x9, 0x00000000, 0x0, 0x40AC01C4
#define IOMUXC_PTF18_GPIOF18                                0x40AC01C8, 0x1, 0x00000000, 0x0, 0x40AC01C8
#define IOMUXC_PTF18_USB1_DATA6                             0x40AC01C8, 0xB, 0x00000000, 0x0, 0x40AC01C8
#define IOMUXC_PTF18_VIU_D14                                0x40AC01C8, 0xC, 0x00000000, 0x0, 0x40AC01C8
#define IOMUXC_PTF18_FXIO1_D14                              0x40AC01C8, 0x2, 0x40AC023C, 0x2, 0x40AC01C8
#define IOMUXC_PTF18_LPSPI3_SCK                             0x40AC01C8, 0x3, 0x40AC0320, 0x3, 0x40AC01C8
#define IOMUXC_PTF18_TPM6_CH0                               0x40AC01C8, 0x6, 0x40AC02D0, 0x3, 0x40AC01C8
#define IOMUXC_PTF18_FB_AD30                                0x40AC01C8, 0x9, 0x00000000, 0x0, 0x40AC01C8
#define IOMUXC_PTF19_GPIOF19                                0x40AC01CC, 0x1, 0x00000000, 0x0, 0x40AC01CC
#define IOMUXC_PTF19_USB1_DATA7                             0x40AC01CC, 0xB, 0x00000000, 0x0, 0x40AC01CC
#define IOMUXC_PTF19_VIU_D15                                0x40AC01CC, 0xC, 0x00000000, 0x0, 0x40AC01CC
#define IOMUXC_PTF19_FXIO1_D15                              0x40AC01CC, 0x2, 0x40AC0240, 0x2, 0x40AC01CC
#define IOMUXC_PTF19_LPSPI3_PCS0                            0x40AC01CC, 0x3, 0x40AC0310, 0x3, 0x40AC01CC
#define IOMUXC_PTF19_TPM6_CH1                               0x40AC01CC, 0x6, 0x40AC02D4, 0x3, 0x40AC01CC
#define IOMUXC_PTF19_FB_AD31                                0x40AC01CC, 0x9, 0x00000000, 0x0, 0x40AC01CC
#define IOMUXC_RESET0_b_RESET0_b                            0x4103D294, 0x0, 0x00000000, 0x0, 0x4103D294
#define IOMUXC_RESET1_b_RESET1_b                            0x40AC039C, 0x0, 0x00000000, 0x0, 0x40AC039C
/*@}*/

#if defined(__cplusplus)
extern "C" {
#endif /*__cplusplus */

/*! @name Configuration */
/*@{*/

/*!
 * @brief Sets the IOMUXC pin mux mode.
 * @note The first five parameters can be filled with the pin function ID macros.
 *
 * This is an example to set the PTA2 as the lpuart0_tx:
 * @code
 * IOMUXC_SetPinMux(IOMUXC_PTA2_LPUART0_TX, 0);
 * @endcode
 *
 * This is an example to set the PTA0 as GPIOA0:
 * @code
 * IOMUXC_SetPinMux(IOMUXC_PTA0_GPIOA0, 0);
 * @endcode
 *
 * @param muxRegister    The pin mux register.
 * @param muxMode        The pin mux mode.
 * @param inputRegister  The select input register.
 * @param inputDaisy     The input daisy.
 * @param configRegister The config register.
 * @param inputInv       The pad->module input inversion.
 */
static inline void IOMUXC_SetPinMux(uint32_t muxRegister,
                                    uint32_t muxMode,
                                    uint32_t inputRegister,
                                    uint32_t inputDaisy,
                                    uint32_t configRegister,
                                    uint32_t inputInv)
{
    if (muxRegister)
    {
        *((volatile uint32_t *)muxRegister) =
            (*((volatile uint32_t *)muxRegister) & ~IOMUXC0_SW_MUX_CTL_PAD_MUX_MODE_MASK) |
            IOMUXC0_SW_MUX_CTL_PAD_MUX_MODE(muxMode);
    }

    if (inputRegister)
    {
        *((volatile uint32_t *)inputRegister) =
            IOMUXC0_SELECT_INPUT_DAISY(inputDaisy) | IOMUXC0_SELECT_INPUT_INVERSION(inputInv);
    }
}

/*!
 * @brief Sets the IOMUXC pin configuration.
 * @note The previous five parameters can be filled with the pin function ID macros.
 *
 * This is an example to set pin configuration for IOMUXC_PTA0_LPI2C0_SCL:
 * @code
 * IOMUXC_SetPinConfig(IOMUXC_PTA0_LPI2C0_SCL, IOMUXC0_SW_PAD_CTL_PAD_PUS_MASK | IOMUXC0_SW_PAD_CTL_PAD_PUS(2U))
 * @endcode
 *
 * @param muxRegister    The pin mux register.
 * @param muxMode        The pin mux mode.
 * @param inputRegister  The select input register.
 * @param inputDaisy     The input daisy.
 * @param configRegister The config register.
 * @param configValue    The pin config value.
 */
static inline void IOMUXC_SetPinConfig(uint32_t muxRegister,
                                       uint32_t muxMode,
                                       uint32_t inputRegister,
                                       uint32_t inputDaisy,
                                       uint32_t configRegister,
                                       uint32_t configValue)
{
    if (configRegister)
    {
        *((volatile uint32_t *)configRegister) =
            (*((volatile uint32_t *)configRegister) & IOMUXC0_SW_MUX_CTL_PAD_MUX_MODE_MASK) | configValue;
    }
}
/*@}*/

#if defined(__cplusplus)
}
#endif /*__cplusplus */

/*! @}*/

#endif /* _FSL_IOMUXC_H_ */
