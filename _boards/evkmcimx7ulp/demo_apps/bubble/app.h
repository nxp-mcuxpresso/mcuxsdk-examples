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
#define BOARD_TPM_BASEADDR         TPM0
#define BOARD_FIRST_TIMER_CHANNEL  3U
#define BOARD_SECOND_TIMER_CHANNEL 4U

/* Get source clock for TPM driver */
#define BOARD_TIMER_SOURCE_CLOCK CLOCK_GetIpFreq(kCLOCK_Tpm0)

/* LPI2C */
#define I2C_BAUDRATE 100000U

#define I2C_RELEASE_SDA_PORT  PORTB
#define I2C_RELEASE_SCL_PORT  PORTB
#define I2C_RELEASE_SDA_GPIO  GPIOB
#define I2C_RELEASE_SDA_PIN   13U
#define I2C_RELEASE_SCL_GPIO  GPIOB
#define I2C_RELEASE_SCL_PIN   12U
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
