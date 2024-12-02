/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
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
/* The Flextimer instance/channel used for board */
#define BOARD_FTM_BASEADDR         FTM0
#define BOARD_FIRST_TIMER_CHANNEL  0U
#define BOARD_SECOND_TIMER_CHANNEL 2U
#define BOARD_FIRST_CHANNEL_INT    kFTM_Chnl0InterruptEnable
#define BOARD_SECOND_CHANNEL_INT   kFTM_Chnl2InterruptEnable
#define BOARD_FTM_IRQ_HANDLER_FUNC FTM0_IRQHandler
#define BOARD_FTM_IRQ_ID           FTM0_IRQn
#define BOARD_FTM_PRESCALE_DIVIDER kFTM_Prescale_Divide_128

/* Get source clock for FTM driver */
#define BOARD_TIMER_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_ScgFircClk)

/* LPI2C */
#define I2C_BAUDRATE 100000U

/* Accelerometer Reset PIN */
#define BOARD_ACCEL_RESET_GPIO GPIOB
#define BOARD_ACCEL_RESET_PIN  9U
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void BOARD_ACCEL_Reset(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
