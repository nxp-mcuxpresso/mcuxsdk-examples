/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "board.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define LED_INIT()          //not possible to use on-board LED from the secondary core
#define LED_TOGGLE()        //not possible to use on-board LED from the secondary core

/* core1 (M7_1, secondary) uses the B-side of MU2, the inter-CPU messaging unit.
 * MU2_B has dedicated NVIC lines on MCXE32B (TX, RX and general-purpose).
 * core1 services the B-side vectors directly; all three collapse into a single
 * joined handler. The MU flag is cleared by the MU driver inside the handler. */
#define APP_MU              MU2_B
#define APP_MU_IRQHandler   MU2_B_RX_IRQHandler(void){MU_B_Joined_IRQHandler();}; \
                         void MU2_B_TX_IRQHandler(void){MU_B_Joined_IRQHandler();}; \
                         void MU2_B_IRQHandler(void){MU_B_Joined_IRQHandler();}; \
                         void MU_B_Joined_IRQHandler

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void MU_B_Joined_IRQHandler(void);
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
/*******************************************************************************
 * EOF
 ******************************************************************************/
