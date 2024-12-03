/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "fsl_smc.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define DEMO_SMC SMC
/* Accelerometer I2C address*/
#define ACCEL_I2C_ADDR 0x1CU
/* Accelerometer Device identification register read value*/
#define ACCEL_WHO_AM_I 0xC7U
/* Accelerometer Reset PIN */
#define ACCEL_RESET_GPIO (GPIOB)
#define ACCEL_RESET_PIN  (9U)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
