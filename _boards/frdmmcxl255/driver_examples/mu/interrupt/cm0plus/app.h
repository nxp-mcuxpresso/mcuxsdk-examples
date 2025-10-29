/*
 * Copyright 2025 NXP
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
#define APP_MU              MUB
#define APP_MU_IRQHandler MU_B_RX_IRQHandler(void){MU_B_Joined_IRQHandler();}; \
                     void MU_B_TX_IRQHandler(void){MU_B_Joined_IRQHandler();}; \
                     void MU_B_INT_IRQHandler(void){MU_B_Joined_IRQHandler();}; \
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