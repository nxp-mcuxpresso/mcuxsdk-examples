/*
 * Copyright 2024 NXP
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

#define XBAR_DIR_CTRL1_IOMUXC_XBAR_DIR_SEL_17_IOMUX 0x00U /*!< IOMUXC XBAR_INOUT17 function direction select: XBAR_INOUT as input */
#define XBAR_DIR_CTRL1_IOMUXC_XBAR_DIR_SEL_18_IOMUX 0x00U /*!< IOMUXC XBAR_INOUT18 function direction select: XBAR_INOUT as input */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitPins(void);                    /* Function assigned for the Cortex-M7F */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_ECAT_MDIO_InitPins(void);          /* Function assigned for the Cortex-M7F */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_Port0_RMII_InitPins(void);         /* Function assigned for the Cortex-M7F */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_Port1_RMII_InitPins(void);         /* Function assigned for the Cortex-M7F */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_ECAT_I2C_InitPins(void);           /* Function assigned for the Cortex-M7F */

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
