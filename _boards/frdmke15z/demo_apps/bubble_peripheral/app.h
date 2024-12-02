/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
/* Accelerometer Reset PIN */
#define BOARD_ACCEL_RESET_GPIO GPIOB
#define BOARD_ACCEL_RESET_PIN  9U

/* The Flextimer instance/channel used for board */
#define BOARD_FTM_BASEADDR         FTM0
#define BOARD_FIRST_TIMER_CHANNEL  2U
#define BOARD_SECOND_TIMER_CHANNEL 1U
#define BOARD_FIRST_CHANNEL_INT    kFTM_Chnl2InterruptEnable
#define BOARD_SECOND_CHANNEL_INT   kFTM_Chnl1InterruptEnable
#define BOARD_FTM_IRQ_HANDLER_FUNC FTM0_IRQHandler
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void BOARD_ACCEL_Reset(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
