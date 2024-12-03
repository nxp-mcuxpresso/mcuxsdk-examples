/*
 * Copyright 2024 NXP
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
#define ACCEL_I2C_CLK_SRC  I2C1_CLK_SRC
#define ACCEL_I2C_CLK_FREQ CLOCK_GetFreq(I2C1_CLK_SRC)

#define I2C_RELEASE_SDA_PORT  PORTD
#define I2C_RELEASE_SCL_PORT  PORTD
#define I2C_RELEASE_SDA_GPIO  GPIOD
#define I2C_RELEASE_SDA_PIN   6U
#define I2C_RELEASE_SCL_GPIO  GPIOD
#define I2C_RELEASE_SCL_PIN   7U
#define I2C_RELEASE_BUS_COUNT 100U
#define I2C_BAUDRATE          100000U
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void BOARD_I2C_ReleaseBus(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
