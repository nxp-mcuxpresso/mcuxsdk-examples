/*
 * Copyright 2020-2021 NXP
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

/*! @name GPIOC0 (number 3), Y1[1]/EXTAL
  @{ */
#define BOARD_EXTAL_GPIO GPIOC          /*!<@brief GPIO device name: GPIOC */
#define BOARD_EXTAL_PIN 0U              /*!<@brief GPIOC pin index: 0 */
#define BOARD_EXTAL_PIN_MASK kGPIO_Pin0 /*!<@brief PORT pin mask */
                                        /* @} */

/*! @name GPIOC1 (number 4), Y1[3]/XTAL
  @{ */
#define BOARD_XTAL_GPIO GPIOC          /*!<@brief GPIO device name: GPIOC */
#define BOARD_XTAL_PIN 1U              /*!<@brief GPIOC pin index: 1 */
#define BOARD_XTAL_PIN_MASK kGPIO_Pin1 /*!<@brief PORT pin mask */
                                       /* @} */

/*! @name GPIOD6 (number 9), U14C[9]/TXD_BDM
  @{ */
#define BOARD_TXD_BDM_GPIO GPIOD          /*!<@brief GPIO device name: GPIOD */
#define BOARD_TXD_BDM_PIN 6U              /*!<@brief GPIOD pin index: 6 */
#define BOARD_TXD_BDM_PIN_MASK kGPIO_Pin6 /*!<@brief PORT pin mask */
                                          /* @} */

/*! @name GPIOD5 (number 10), U13[3]/RXD_BDM
  @{ */
#define BOARD_RXD_BDM_GPIO GPIOD          /*!<@brief GPIO device name: GPIOD */
#define BOARD_RXD_BDM_PIN 5U              /*!<@brief GPIOD pin index: 5 */
#define BOARD_RXD_BDM_PIN_MASK kGPIO_Pin5 /*!<@brief PORT pin mask */
                                          /* @} */

/*! @name GPIOA6 (number 19), THER_A
  @{ */
#define BOARD_THER_A_GPIO GPIOA          /*!<@brief GPIO device name: GPIOA */
#define BOARD_THER_A_PIN 6U              /*!<@brief GPIOA pin index: 6 */
#define BOARD_THER_A_PIN_MASK kGPIO_Pin6 /*!<@brief PORT pin mask */
                                         /* @} */

/*! @name GPIOA7 (number 17), THER_B
  @{ */
#define BOARD_THER_B_GPIO GPIOA          /*!<@brief GPIO device name: GPIOA */
#define BOARD_THER_B_PIN 7U              /*!<@brief GPIOA pin index: 7 */
#define BOARD_THER_B_PIN_MASK kGPIO_Pin7 /*!<@brief PORT pin mask */
                                         /* @} */

/*! @name GPIOA4 (number 21), J4[4]/ANA4
  @{ */
#define BOARD_ANA4_GPIO GPIOA          /*!<@brief GPIO device name: GPIOA */
#define BOARD_ANA4_PIN 4U              /*!<@brief GPIOA pin index: 4 */
#define BOARD_ANA4_PIN_MASK kGPIO_Pin4 /*!<@brief PORT pin mask */
                                       /* @} */

/*! @name GPIOA5 (number 20), J4[6]/ANA5
  @{ */
#define BOARD_ANA5_GPIO GPIOA          /*!<@brief GPIO device name: GPIOA */
#define BOARD_ANA5_PIN 5U              /*!<@brief GPIOA pin index: 5 */
#define BOARD_ANA5_PIN_MASK kGPIO_Pin5 /*!<@brief PORT pin mask */
                                       /* @} */

/*! @name GPIOB0 (number 33), J2[3]/CUR_B
  @{ */
#define BOARD_CUR_B_GPIO GPIOB          /*!<@brief GPIO device name: GPIOB */
#define BOARD_CUR_B_PIN 0U              /*!<@brief GPIOB pin index: 0 */
#define BOARD_CUR_B_PIN_MASK kGPIO_Pin0 /*!<@brief PORT pin mask */
                                        /* @} */

/*! @name GPIOB1 (number 34), J4[8]/ANB1/DACB_O
  @{ */
#define BOARD_DACB_O_GPIO GPIOB          /*!<@brief GPIO device name: GPIOB */
#define BOARD_DACB_O_PIN 1U              /*!<@brief GPIOB pin index: 1 */
#define BOARD_DACB_O_PIN_MASK kGPIO_Pin1 /*!<@brief PORT pin mask */
                                         /* @} */

/*! @name GPIOB2 (number 36), J2[9]/CUR_DCB
  @{ */
#define BOARD_CUR_DCB_GPIO GPIOB          /*!<@brief GPIO device name: GPIOB */
#define BOARD_CUR_DCB_PIN 2U              /*!<@brief GPIOB pin index: 2 */
#define BOARD_CUR_DCB_PIN_MASK kGPIO_Pin2 /*!<@brief PORT pin mask */
                                          /* @} */

/*! @name GPIOB3 (number 42), J2[5]/CUR_C/GB3
  @{ */
#define BOARD_CUR_CB_GPIO GPIOB          /*!<@brief GPIO device name: GPIOB */
#define BOARD_CUR_CB_PIN 3U              /*!<@brief GPIOB pin index: 3 */
#define BOARD_CUR_CB_PIN_MASK kGPIO_Pin3 /*!<@brief PORT pin mask */
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
