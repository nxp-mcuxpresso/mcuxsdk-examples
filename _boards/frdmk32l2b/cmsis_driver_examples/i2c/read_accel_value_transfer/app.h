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
#define EXAMPLE_I2C_MASTER Driver_I2C0

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
