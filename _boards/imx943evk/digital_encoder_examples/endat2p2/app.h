/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef APP_H_
#define APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/

#define ENDAT2P2_BASE           ENDAT2P2_1

/* Interrupt number and interrupt handler for the TPM instance used */
#define ENDAT2P2_IRQn           Reserved163_IRQn
#define ENDAT2P2_IRQHandler     Reserved163_IRQnHandler

#define ENDAT2P2_FTCLK          2000000 /* 2MHz */
#define ENDAT2P2_SOURCE_CLOCK   HAL_ClockGetFreq(hal_clock_endat21)

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* APP_H_ */
