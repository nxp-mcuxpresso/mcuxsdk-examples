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
/* I2C source clock */
#define ACCEL_I2C_CLK_SRC        I2C0_CLK_SRC
#define ACCEL_I2C_CLK_FREQ       CLOCK_GetFreq(I2C0_CLK_SRC)
#define BOARD_ACCEL_I2C_BASEADDR I2C0
#define I2C_BAUDRATE             100000U

#define I2C_RELEASE_SDA_PORT  PORTB
#define I2C_RELEASE_SCL_PORT  PORTB
#define I2C_RELEASE_SDA_GPIO  GPIOB
#define I2C_RELEASE_SDA_PIN   4U
#define I2C_RELEASE_SCL_GPIO  GPIOB
#define I2C_RELEASE_SCL_PIN   3U
#define I2C_RELEASE_BUS_COUNT 100U
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void BOARD_I2C_ReleaseBus(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
