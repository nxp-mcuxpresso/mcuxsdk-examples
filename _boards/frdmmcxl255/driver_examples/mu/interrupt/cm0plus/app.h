/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*${macro:start}*/
#define LED_INIT()        LED_RED_INIT(LOGIC_LED_OFF)
#define LED_TOGGLE()      LED_RED_TOGGLE()
#define APP_MU            MUB
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
