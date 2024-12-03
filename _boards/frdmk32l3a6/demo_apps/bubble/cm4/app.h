/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
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
#define BOARD_TIMER_BASEADDR       TPM2
#define BOARD_FIRST_TIMER_CHANNEL  0U
#define BOARD_SECOND_TIMER_CHANNEL 1U
/* Get source clock for TPM driver */
#define BOARD_TIMER_SOURCE_CLOCK (CLOCK_GetIpFreq(kCLOCK_Tpm2))
/* LPI2C */
#define I2C_BAUDRATE 100000U

#define I2C_RELEASE_SDA_PORT  PORTE
#define I2C_RELEASE_SCL_PORT  PORTE
#define I2C_RELEASE_SDA_GPIO  GPIOE
#define I2C_RELEASE_SDA_PIN   29U
#define I2C_RELEASE_SCL_GPIO  GPIOE
#define I2C_RELEASE_SCL_PIN   30U
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
