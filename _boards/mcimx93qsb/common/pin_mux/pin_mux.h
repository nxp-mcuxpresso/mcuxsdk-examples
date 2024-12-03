/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _PIN_MUX_H_
#define _PIN_MUX_H_

#include "fsl_iomuxc.h"

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
void BOARD_InitPins(void);                                 /*!< Function assigned for the core: Cortex-M33[cm33] */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitDEBUG_UARTPins(void);                       /*!< Function assigned for the core: Cortex-M33[cm33] */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitLPI2C1Pins(void);                           /*!< Function assigned for the core: Cortex-M33[cm33] */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitLPI2C2Pins(void);                           /*!< Function assigned for the core: Cortex-M33[cm33] */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitLPSPI3Pins(void);                           /*!< Function assigned for the core: Cortex-M33[cm33] */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitCANPins(void);                              /*!< Function assigned for the core: Cortex-M33[cm33] */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitNETQOSPins(void);                           /*!< Function assigned for the core: Cortex-M33[cm33] */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitFLEXIOPins(void);                           /*!< Function assigned for the core: Cortex-M33[cm33] */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitXSPI1Pins(void);                            /*!< Function assigned for the core: Cortex-M33[cm33] */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitI3CPins(void);                              /*!< Function assigned for the core: Cortex-M33[cm33] */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitPDMPins(void);                              /*!< Function assigned for the core: Cortex-M33[cm33] */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitIRGPIOPins(void);                           /*!< Function assigned for the core: Cortex-M33[cm33] */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitSAIPins(void);                              /*!< Function assigned for the core: Cortex-M33[cm33] */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitTPMPins(void);                              /*!< Function assigned for the core: Cortex-M33[cm33] */

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
