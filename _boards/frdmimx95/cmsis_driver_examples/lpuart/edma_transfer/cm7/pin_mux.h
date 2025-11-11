/*
 * Copyright 2025 NXP
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
void LPUART3_InitPins(void);                               /*!< Function assigned for the core: Cortex-M7F[cm7] */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void LPUART3_DeinitPins(void);                             /*!< Function assigned for the core: Cortex-M7F[cm7] */

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
