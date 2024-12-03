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
/* The Ctimer instance/channel used for board */
#define BOARD_TIMER_BASEADDR       CTIMER3
#define BOARD_FIRST_TIMER_CHANNEL  kCTIMER_Match_2
#define BOARD_SECOND_TIMER_CHANNEL kCTIMER_Match_3
/* Get source clock for Ctimer driver */
#define BOARD_TIMER_SOURCE_CLOCK CLOCK_GetCTimerClkFreq(3U)
/* I2C source clock */
#define I2C_BAUDRATE 100000U

#define I2C_RELEASE_SDA_PORT  PORT4
#define I2C_RELEASE_SCL_PORT  PORT4
#define I2C_RELEASE_SDA_GPIO  GPIO4
#define I2C_RELEASE_SDA_PIN   0U
#define I2C_RELEASE_SCL_GPIO  GPIO4
#define I2C_RELEASE_SCL_PIN   1U
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
