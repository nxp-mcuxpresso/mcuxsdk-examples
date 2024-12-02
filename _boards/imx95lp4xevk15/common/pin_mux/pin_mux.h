/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _PIN_MUX_H_
#define _PIN_MUX_H_

#include "hal_pinctrl.h"

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
void BOARD_InitCAN1Pins(void);                             /*!< Function assigned for the core: Cortex-M7F[cm7] */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitCAN2Pins(void);                             /*!< Function assigned for the core: Cortex-M7F[cm7] */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitLPI2C1Pins(void);                           /*!< Function assigned for the core: Cortex-M7F[cm7] */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitLPI2C2Pins(void);                           /*!< Function assigned for the core: Cortex-M7F[cm7] */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitLPI2C4Pins(void);                           /*!< Function assigned for the core: Cortex-M7F[cm7] */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitLPI2C6Pins(void);                           /*!< Function assigned for the core: Cortex-M7F[cm7] */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitFLEXIOPins(void);                           /*!< Function assigned for the core: Cortex-M7F[cm7] */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitLPSPI1Pins(void);                           /*!< Function assigned for the core: Cortex-M7F[cm7] */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitXSPI1Pins(void);                            /*!< Function assigned for the core: Cortex-M7F[cm7] */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitI3C1Pins(void);                             /*!< Function assigned for the core: Cortex-M7F[cm7] */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitCLKOPins(void);                             /*!< Function assigned for the core: Cortex-M7F[cm7] */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitNETPins(void);                              /*!< Function assigned for the core: Cortex-M7F[cm7] */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitETH0Pins(void);                             /*!< Function assigned for the core: Cortex-M7F[cm7] */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitETH1Pins(void);                             /*!< Function assigned for the core: Cortex-M7F[cm7] */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitSAI3Pins(void);                             /*!< Function assigned for the core: Cortex-M7F[cm7] */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitPDMPins(void);                              /*!< Function assigned for the core: Cortex-M7F[cm7] */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitGPIO2Pins(void);                            /*!< Function assigned for the core: Cortex-M7F[cm7] */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitTPM2Pins(void);                             /*!< Function assigned for the core: Cortex-M7F[cm7] */

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
