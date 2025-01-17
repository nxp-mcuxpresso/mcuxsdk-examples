/*
 * Copyright 2021 NXP
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

#define IOCON_PIO_DIGITAL_EN 0x0100u   /*!<@brief Enables digital function */
#define IOCON_PIO_FUNC1 0x01u          /*!<@brief Selects pin function 1 */
#define IOCON_PIO_INV_DI 0x00u         /*!<@brief Input function is not inverted */
#define IOCON_PIO_MODE_INACT 0x00u     /*!<@brief No addition pin function */
#define IOCON_PIO_OPENDRAIN_DI 0x00u   /*!<@brief Open drain is disabled */
#define IOCON_PIO_SLEW_STANDARD 0x00u  /*!<@brief Standard mode, output slew rate control is enabled */
#define PIO0_29_DIGIMODE_DIGITAL 0x01u /*!<@brief Select Digital mode: Enable Digital mode. Digital input is enabled. */

/*! @name FC0_RXD_SDA_MOSI_DATA (number 93), P0_29-FC0_RXD_SDA_MOSI_DATA-TRACED2
  @{ */
/* @} */

/*! @name FC0_TXD_SCL_MISO_WS (number 95), JP49[3]/P0_30-FC0_TXD_SCL_MISO_WS-TRACED1-CAN0_TD
  @{ */
/* @} */

/*! @name CAN0_TD (number 61), JP49[1]/J41[5]/J92[12]/J7[4]/P1_2-CAN0_TD-PDM_CLK23-HS_SPI_SCK
  @{ */
/* @} */

/*! @name CAN0_RD (number 62), JP48[1]/J41[6]/J92[10]/J7[5]/P1_3-CAN0_RD-PDM_DATA23-HS_SPI_MISO
  @{ */
/* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitPins(void); /* Function assigned for the Cortex-M33 */

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
