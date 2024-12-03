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

/* PTA19 (number AB19), UART1_RX */
#define BOARD_UART1_RX_PERIPHERAL                                        LPUART1   /*!< Device name: LPUART1 */
#define BOARD_UART1_RX_SIGNAL                                          lpuart_rx   /*!< LPUART1 signal: lpuart_rx */
#define BOARD_UART1_RX_PIN_NAME                                            PTA11   /*!< Pin name */
#define BOARD_UART1_RX_PIN_FUNCTION_ID                   IOMUXC_PTA11_LPUART1_RX   /*!< Pin function id */
#define BOARD_UART1_RX_LABEL                                          "UART1_RX"   /*!< Label */
#define BOARD_UART1_RX_NAME                                           "UART1_RX"   /*!< Identifier name */

/* PTA18 (number AC19), UART1_TX */
#define BOARD_UART1_TX_PERIPHERAL                                        LPUART1   /*!< Device name: LPUART1 */
#define BOARD_UART1_TX_SIGNAL                                          lpuart_tx   /*!< LPUART1 signal: lpuart_tx */
#define BOARD_UART1_TX_PIN_NAME                                            PTA10   /*!< Pin name */
#define BOARD_UART1_TX_PIN_FUNCTION_ID                   IOMUXC_PTA10_LPUART1_TX   /*!< Pin function id */
#define BOARD_UART1_TX_LABEL                                          "UART1_TX"   /*!< Label */
#define BOARD_UART1_TX_NAME                                           "UART1_TX"   /*!< Identifier name */

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
