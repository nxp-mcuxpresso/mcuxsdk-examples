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

void BOARD_InitPins(void);                                 /*!< Function assigned for the core: undefined[cm33] */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */

void BOARD_InitSAI1Pins(void);                                /*!< Function assigned for the core: undefined[cm33] */

void BOARD_InitI2C3Pins(void);                                /*!< Function assigned for the core: undefined[cm33] */


void BOARD_InitPDMPins(void);                                /*!< Function assigned for the core: undefined[cm33] */

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

