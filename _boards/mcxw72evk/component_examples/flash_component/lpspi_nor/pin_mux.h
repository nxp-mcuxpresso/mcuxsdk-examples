/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
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

/*! @name PORTC1 (number 38), LPSPI1_PCS3
  @{ */

/* Symbols to be used with GPIO driver */
#define BOARD_INITPINS_LPSPI1_PCS3_GPIO GPIOC               /*!<@brief GPIO peripheral base pointer */
#define BOARD_INITPINS_LPSPI1_PCS3_GPIO_PIN_MASK (1U << 1U) /*!<@brief GPIO pin mask */

/* Symbols to be used with PORT driver */
#define BOARD_INITPINS_LPSPI1_PCS3_PORT PORTC               /*!<@brief PORT peripheral base pointer */
#define BOARD_INITPINS_LPSPI1_PCS3_PIN 1U                   /*!<@brief PORT pin number */
#define BOARD_INITPINS_LPSPI1_PCS3_PIN_MASK (1U << 1U)      /*!<@brief PORT pin mask */
                                                            /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitPins(void);

/*! @name PORTB0 (number 46), LPSPI1_PCS0
  @{ */

/* Symbols to be used with GPIO driver */
#define BOARD_INITEXTFLASHPINS_LPSPI1_PCS0_GPIO GPIOB               /*!<@brief GPIO peripheral base pointer */
#define BOARD_INITEXTFLASHPINS_LPSPI1_PCS0_GPIO_PIN_MASK (1U << 0U) /*!<@brief GPIO pin mask */

/* Symbols to be used with PORT driver */
#define BOARD_INITEXTFLASHPINS_LPSPI1_PCS0_PORT PORTB               /*!<@brief PORT peripheral base pointer */
#define BOARD_INITEXTFLASHPINS_LPSPI1_PCS0_PIN 0U                   /*!<@brief PORT pin number */
#define BOARD_INITEXTFLASHPINS_LPSPI1_PCS0_PIN_MASK (1U << 0U)      /*!<@brief PORT pin mask */
                                                                    /* @} */

/*! @name PORTC1 (number 38), LPSPI1_PCS3
  @{ */

/* Symbols to be used with GPIO driver */
#define BOARD_INITEXTFLASHPINS_LPSPI1_PCS3_GPIO GPIOC               /*!<@brief GPIO peripheral base pointer */
#define BOARD_INITEXTFLASHPINS_LPSPI1_PCS3_GPIO_PIN_MASK (1U << 1U) /*!<@brief GPIO pin mask */

/* Symbols to be used with PORT driver */
#define BOARD_INITEXTFLASHPINS_LPSPI1_PCS3_PORT PORTC               /*!<@brief PORT peripheral base pointer */
#define BOARD_INITEXTFLASHPINS_LPSPI1_PCS3_PIN 1U                   /*!<@brief PORT pin number */
#define BOARD_INITEXTFLASHPINS_LPSPI1_PCS3_PIN_MASK (1U << 1U)      /*!<@brief PORT pin mask */
                                                                    /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitExtFlashPins(void);

/*! @name PORTB0 (number 46), LPSPI1_PCS0
  @{ */

/* Symbols to be used with GPIO driver */
#define BOARD_DEINITEXTFLASHPINS_LPSPI1_PCS0_GPIO GPIOB               /*!<@brief GPIO peripheral base pointer */
#define BOARD_DEINITEXTFLASHPINS_LPSPI1_PCS0_GPIO_PIN_MASK (1U << 0U) /*!<@brief GPIO pin mask */

/* Symbols to be used with PORT driver */
#define BOARD_DEINITEXTFLASHPINS_LPSPI1_PCS0_PORT PORTB               /*!<@brief PORT peripheral base pointer */
#define BOARD_DEINITEXTFLASHPINS_LPSPI1_PCS0_PIN 0U                   /*!<@brief PORT pin number */
#define BOARD_DEINITEXTFLASHPINS_LPSPI1_PCS0_PIN_MASK (1U << 0U)      /*!<@brief PORT pin mask */
                                                                      /* @} */

/*! @name PORTC1 (number 38), LPSPI1_PCS3
  @{ */

/* Symbols to be used with GPIO driver */
#define BOARD_DEINITEXTFLASHPINS_LPSPI1_PCS3_GPIO GPIOC               /*!<@brief GPIO peripheral base pointer */
#define BOARD_DEINITEXTFLASHPINS_LPSPI1_PCS3_GPIO_PIN_MASK (1U << 1U) /*!<@brief GPIO pin mask */

/* Symbols to be used with PORT driver */
#define BOARD_DEINITEXTFLASHPINS_LPSPI1_PCS3_PORT PORTC               /*!<@brief PORT peripheral base pointer */
#define BOARD_DEINITEXTFLASHPINS_LPSPI1_PCS3_PIN 1U                   /*!<@brief PORT pin number */
#define BOARD_DEINITEXTFLASHPINS_LPSPI1_PCS3_PIN_MASK (1U << 1U)      /*!<@brief PORT pin mask */
                                                                      /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_DeinitExtFlashPins(void);

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
