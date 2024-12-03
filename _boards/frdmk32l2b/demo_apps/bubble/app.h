/*
 * Copyright 2019 NXP
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
#define BOARD_TIMER_BASEADDR       TPM0
#define BOARD_FIRST_TIMER_CHANNEL  4U
#define BOARD_SECOND_TIMER_CHANNEL 5U
/* Get source clock for TPM driver */
#define BOARD_TIMER_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_McgIrc48MClk)
#define TIMER_CLOCK_MODE         1U
/* I2C source clock */
#define I2C_BAUDRATE 100000U

/* Accelerometer Reset PIN */
#define BOARD_ACCEL_RESET_GPIO GPIOE
#define BOARD_ACCEL_RESET_PIN  1U
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void BOARD_ACCEL_Reset(void);

/*${prototype:end}*/

#endif /* _APP_H_ */
