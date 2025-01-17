/*
 * Copyright 2022-2023 NXP
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

/*! @name FC4_P0 (coord A1), P1_8
  @{ */
/* @} */

/*! @name FC4_P1 (coord B1), P1_9
  @{ */
/* @} */

/*! @name PIO3_4 (coord F14), P3_4/LED_RED/J5/Q8[2]
  @{ */

/* Symbols to be used with GPIO driver */
#define BOARD_INITPINS_LED_RED_GPIO GPIO3 /*!<@brief GPIO peripheral base pointer */
#define BOARD_INITPINS_LED_RED_GPIO_PIN 4U /*!<@brief GPIO pin number */
#define BOARD_INITPINS_LED_RED_GPIO_PIN_MASK (1U << 4U) /*!<@brief GPIO pin mask */
/* @} */

/*! @name PIO3_3 (coord D16), P3_3/LED_BLUE/Q9[2]
  @{ */

/* Symbols to be used with GPIO driver */
#define BOARD_INITPINS_LED_BLUE_GPIO GPIO3 /*!<@brief GPIO peripheral base pointer */
#define BOARD_INITPINS_LED_BLUE_GPIO_PIN 3U /*!<@brief GPIO pin number */
#define BOARD_INITPINS_LED_BLUE_GPIO_PIN_MASK (1U << 3U) /*!<@brief GPIO pin mask */
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
