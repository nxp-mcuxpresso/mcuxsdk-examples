/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _PIN_MUX_H_
#define _PIN_MUX_H_

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
 * @brief Configures pins.
 *
 */
#define BOARD_INITLEDSPINS_LED_RED_GPIO GPIO1                /*!<@brief GPIO peripheral base pointer */
#define BOARD_INITLEDSPINS_LED_RED_INIT_GPIO_VALUE 1U        /*!<@brief GPIO output initial state */
#define BOARD_INITLEDSPINS_LED_RED_GPIO_PIN 15U              /*!<@brief GPIO pin number */
#define BOARD_INITLEDSPINS_LED_RED_GPIO_PIN_MASK (1U << 15U) /*!<@brief GPIO pin mask */

/* Symbols to be used with PORT driver */
#define BOARD_INITLEDSPINS_LED_RED_PORT PORT1                /*!<@brief PORT peripheral base pointer */
#define BOARD_INITLEDSPINS_LED_RED_PIN 15U                   /*!<@brief PORT pin number */
#define BOARD_INITLEDSPINS_LED_RED_PIN_MASK (1U << 15U)      /*!<@brief PORT pin mask */
                                                             /* @} */                                                             /* @} */
void BOARD_InitDEBUG_UARTPins(void);
void BOARD_InitBUTTONsPins(void);
void BOARD_InitLEDsPins(void);

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