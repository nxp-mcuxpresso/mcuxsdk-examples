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
/* The Flextimer instance/channel used for board */
#define BOARD_TIMER_BASEADDR       TPM0
#define BOARD_FIRST_TIMER_CHANNEL  0U
#define BOARD_SECOND_TIMER_CHANNEL 1U
/* Get source clock for TPM driver */
#define BOARD_TIMER_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_McgIrc48MClk)
#define TIMER_CLOCK_MODE         1U
/* I2C source clock */
#define I2C_BAUDRATE 100000U

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
