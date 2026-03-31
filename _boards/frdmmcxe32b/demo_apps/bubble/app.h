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
#define TIMER_BASEADDR        PIT_0
#define TIMER_CHANNEL         kPIT_Chnl_0
#define TIMER_IRQ_HANDLER     PIT0_IRQHandler
#define TIMER_IRQ_ID          PIT0_IRQn

/* Get source clock for PIT driver */
#define PIT_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_Pit0Clk)

/* LPI2C */
#define I2C_BAUDRATE (100000U)

/* USER_LED*/
#define USER_LED_ON()      LED_RED_ON()
#define USER_LED_OFF()     LED_RED_OFF()
#define USER_LED_TOGGLE()  LED_RED_TOGGLE()
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
