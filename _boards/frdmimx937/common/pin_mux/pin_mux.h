/*
 * Copyright 2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _PIN_MUX_H_
#define _PIN_MUX_H_

#include "fsl_iomuxc.h"
#include "board.h"

/***********************************************************************************************************************
 * Definitions
 **********************************************************************************************************************/

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
void BOARD_InitPins(void);                                 /*!< Function assigned for the core: Cortex-M7F[cm7] */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitDEBUG_UARTPins(void);                       /*!< Function assigned for the core: Cortex-M7F[cm7] */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitNETPins(void);                              /*!< Function assigned for the core: Cortex-M7F[cm7] */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitI2C7Pins(void);                             /*!< Function assigned for the core: Cortex-M7F[cm7] */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitCANPins(void);                              /*!< Function assigned for the core: Cortex-M7F[cm7] */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitI2C6Pins(void);                             /*!< Function assigned for the core: Cortex-M7F[cm7] */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitITPMPins(void);                             /*!< Function assigned for the core: Cortex-M7F[cm7] */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitLPSPIPins(void);                            /*!< Function assigned for the core: Cortex-M7F[cm7] */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_DeInitLPSPIPins(void);                          /*!< Function assigned for the core: Cortex-M7F[cm7] */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitI2C4Pins(void);                             /*!< Function assigned for the core: Cortex-M7F[cm7] */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitMQSPins(void);                             /*!< Function assigned for the core: Cortex-M7F[cm7] */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitI2C2Pins(void);                             /*!< Function assigned for the core: Cortex-M7F[cm7] */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitFLEXIOUARTPins(void);                       /*!< Function assigned for the core: Cortex-M7F[cm7] */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitPDMPins(void);                              /*!< Function assigned for the core: Cortex-M7F[cm7] */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_DeInitI2C6Pins(void);                           /*!< Function assigned for the core: Cortex-M7F[cm7] */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitXSPIPins(void);                             /*!< Function assigned for the core: Cortex-M7F[cm7] */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitI2C3Pins(void);                             /*!< Function assigned for the core: Cortex-M7F[cm7] */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitCLKOPins(void);                             /*!< Function assigned for the core: Cortex-M7F[cm7] */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitFLEXIOPins(void);                           /*!< Function assigned for the core: Cortex-M7F[cm7] */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitGPIO2Pins(void);                            /*!< Function assigned for the core: Cortex-M7F[cm7] */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_DeInitDEBUG_UARTPins(void);                     /*!< Function assigned for the core: Cortex-M7F[cm7] */

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
