/*
 * Copyright 2021 NXP
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
/* LPI2C */
#define BOARD_ACCEL_I2C_BASEADDR LPI2C0
#define LPI2C_CLOCK_FREQUENCY    CLOCK_GetIpFreq(kCLOCK_Lpi2c0)
#define I2C_BAUDRATE             100000U

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
