/*
 * Copyright 2021-2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/***********************************************************************************************************************
 * This file was generated by the MCUXpresso Config Tools. Any manual edits made to this file
 * will be overwritten if the respective MCUXpresso Config Tools is used to update this file.
 **********************************************************************************************************************/

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
 * @brief Calls initialization functions.
 *
 */
void BOARD_InitBootPins(void);

/*! @name PORTD3 (number 26), LED2_BLUE
  @{ */

/* Symbols to be used with GPIO driver */
#define BOARD_INITPINS_LED2_BLUE_GPIO GPIOD               /*!<@brief GPIO peripheral base pointer */
#define BOARD_INITPINS_LED2_BLUE_GPIO_PIN_MASK (1U << 3U) /*!<@brief GPIO pin mask */

/* Symbols to be used with PORT driver */
#define BOARD_INITPINS_LED2_BLUE_PORT PORTD               /*!<@brief PORT peripheral base pointer */
#define BOARD_INITPINS_LED2_BLUE_PIN 3U                   /*!<@brief PORT pin number */
#define BOARD_INITPINS_LED2_BLUE_PIN_MASK (1U << 3U)      /*!<@brief PORT pin mask */
                                                          /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitPins(void);

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
