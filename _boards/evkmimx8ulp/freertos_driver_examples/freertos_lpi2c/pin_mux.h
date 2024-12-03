/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _PIN_MUX_H_
#define _PIN_MUX_H_


/***********************************************************************************************************************
 * Definitions
 **********************************************************************************************************************/

/* PTA19, UART1_RX */
#define BOARD_UART1_RX_PERIPHERAL                                        LPUART1   /*!< Device name: LPUART1 */
#define BOARD_UART1_RX_SIGNAL                                          lpuart_rx   /*!< LPUART1 signal: lpuart_rx */
#define BOARD_UART1_RX_PIN_NAME                                            PTA11   /*!< Pin name */
#define BOARD_UART1_RX_PIN_FUNCTION_ID                   IOMUXC_PTA11_LPUART1_RX   /*!< Pin function id */
#define BOARD_UART1_RX_LABEL                                          "UART1_RX"   /*!< Label */
#define BOARD_UART1_RX_NAME                                           "UART1_RX"   /*!< Identifier name */

/* PTA18, UART1_TX */
#define BOARD_UART1_TX_PERIPHERAL                                        LPUART1   /*!< Device name: LPUART1 */
#define BOARD_UART1_TX_SIGNAL                                          lpuart_tx   /*!< LPUART1 signal: lpuart_tx */
#define BOARD_UART1_TX_PIN_NAME                                            PTA10   /*!< Pin name */
#define BOARD_UART1_TX_PIN_FUNCTION_ID                   IOMUXC_PTA10_LPUART1_TX   /*!< Pin function id */
#define BOARD_UART1_TX_LABEL                                          "UART1_TX"   /*!< Label */
#define BOARD_UART1_TX_NAME                                           "UART1_TX"   /*!< Identifier name */
/* PTA16 , I2C0_SCL */
/* Routed pin properties */
#define BOARD_I2C0_SCL_PERIPHERAL                                         LPI2C0   /*!< Peripheral name */
#define BOARD_I2C0_SCL_SIGNAL                                          lpi2c_scl   /*!< Signal name */
#define BOARD_I2C0_SCL_PIN_NAME                                            PTA16   /*!< Routed pin name */
#define BOARD_I2C0_SCL_PIN_FUNCTION_ID                   IOMUXC_PTA16_LPI2C0_SCL   /*!< Pin function id */
#define BOARD_I2C0_SCL_LABEL                                          "I2C0_SCL"   /*!< Label */
#define BOARD_I2C0_SCL_NAME                                           "I2C0_SCL"   /*!< Identifier */

/* PTA17, I2C0_SDA */
/* Routed pin properties */
#define BOARD_I2C0_SDA_PERIPHERAL                                         LPI2C0   /*!< Peripheral name */
#define BOARD_I2C0_SDA_SIGNAL                                          lpi2c_sda   /*!< Signal name */
#define BOARD_I2C0_SDA_PIN_NAME                                            PTA17   /*!< Routed pin name */
#define BOARD_I2C0_SDA_PIN_FUNCTION_ID                   IOMUXC_PTA17_LPI2C0_SDA   /*!< Pin function id */
#define BOARD_I2C0_SDA_LABEL                                          "I2C0_SDA"   /*!< Label */
#define BOARD_I2C0_SDA_NAME                                           "I2C0_SDA"   /*!< Identifier */

/* PTA20, I2C1_SCL */
/* Routed pin properties */
#define BOARD_I2C1_SCL_PERIPHERAL                                         LPI2C1   /*!< Peripheral name */
#define BOARD_I2C1_SCL_SIGNAL                                          lpi2c_scl   /*!< Signal name */
#define BOARD_I2C1_SCL_PIN_NAME                                            PTA20   /*!< Routed pin name */
#define BOARD_I2C1_SCL_PIN_FUNCTION_ID                   IOMUXC_PTA20_LPI2C1_SCL   /*!< Pin function id */
#define BOARD_I2C1_SCL_LABEL                                          "I2C1_SCL"   /*!< Label */
#define BOARD_I2C1_SCL_NAME                                           "I2C1_SCL"   /*!< Identifier */

/* PTA21, I2C1_SDA */
/* Routed pin properties */
#define BOARD_I2C1_SDA_PERIPHERAL                                         LPI2C1   /*!< Peripheral name */
#define BOARD_I2C1_SDA_SIGNAL                                          lpi2c_sda   /*!< Signal name */
#define BOARD_I2C1_SDA_PIN_NAME                                            PTA21   /*!< Routed pin name */
#define BOARD_I2C1_SDA_PIN_FUNCTION_ID                   IOMUXC_PTA21_LPI2C1_SDA   /*!< Pin function id */
#define BOARD_I2C1_SDA_LABEL                                          "I2C1_SDA"   /*!< Label */
#define BOARD_I2C1_SDA_NAME                                           "I2C1_SDA"   /*!< Identifier */

/*!
 * @addtogroup pin_mux
 * @{
 */

/***********************************************************************************************************************
 * API
 **********************************************************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif


/*!
 * @brief Calls initialization functions.
 *
 */
void BOARD_InitBootPins(void);

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitPins(void);                                 /*!< Function assigned for the core: Cortex-M4[cm4] */

#if defined(__cplusplus)
}
#endif

/*!
 * @}
 */
#endif /* _PIN_MUX_H_ */

/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
