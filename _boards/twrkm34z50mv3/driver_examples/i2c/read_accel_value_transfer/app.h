/*
 * Copyright 2021 NXP
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
#define ACCEL_I2C_CLK_SRC  I2C0_CLK_SRC
#define ACCEL_I2C_CLK_FREQ CLOCK_GetFreq(I2C0_CLK_SRC)
#define I2C_BAUDRATE       400000U

#define I2C_RELEASE_SCL_GPIO       GPIOD
#define I2C_RELEASE_SCL_PORT       PORTD
#define I2C_RELEASE_SCL_PIN        7U
#define I2C_RELEASE_SDA_GPIO       GPIOE
#define I2C_RELEASE_SDA_PORT       PORTE
#define I2C_RELEASE_SDA_PIN        0U
#define I2C_RELEASE_BUS_COUNT      100U
#define BOARD_TILT_ENABLE_GPIO     GPIOF
#define BOARD_TILT_ENABLE_GPIO_PIN 7U
#define I2C_ACCEL_MMA8491          1
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void BOARD_I2C_ReleaseBus(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
