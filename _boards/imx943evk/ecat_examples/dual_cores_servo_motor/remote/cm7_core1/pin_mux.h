/*
 * Copyright 2024 NXP
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

void BOARD_Init_M1_PWM(void);
void BOARD_Init_M1_ENDAT2P2(void);
void BOARD_Init_M1_SINC(void);
void BOARD_Init_M1_FAULTS(void);

void BOARD_Init_M2_PWM(void);
void BOARD_Init_M2_ENDAT2P2(void);
void BOARD_Init_M2_SINC(void);
void BOARD_Init_M2_FAULTS(void);


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
