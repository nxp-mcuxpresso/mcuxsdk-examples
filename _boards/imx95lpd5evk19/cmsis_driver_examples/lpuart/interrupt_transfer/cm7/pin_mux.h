/*
 * Copyright 2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _PIN_MUX_H_
#define _PIN_MUX_H_

#include "board.h"
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
void BOARD_InitPins(void);                                 /*!< Function assigned for the core: undefined[cm33] */

#if BOARD_DEBUG_UART_INSTANCE == 1
void LPUART1_InitPins(void);
void LPUART1_DeinitPins(void);                             /*!< Function assigned for the core: undefined[cm33] */
#elif BOARD_DEBUG_UART_INSTANCE == 2
void LPUART2_InitPins(void);
void LPUART2_DeinitPins(void);                             /*!< Function assigned for the core: undefined[cm33] */
#elif BOARD_DEBUG_UART_INSTANCE == 3
void LPUART3_InitPins(void);
void LPUART3_DeinitPins(void);                             /*!< Function assigned for the core: undefined[cm33] */
#endif

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
