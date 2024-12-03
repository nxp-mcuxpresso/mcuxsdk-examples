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
#define LPI2C_CLOCK_FREQUENCY    CLOCK_GetIpFreq(kCLOCK_Lpi2c3)
#define BOARD_ACCEL_I2C_BASEADDR LPI2C3

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
