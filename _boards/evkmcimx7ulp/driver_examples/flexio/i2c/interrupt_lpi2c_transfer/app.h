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
#define BOARD_LPI2C_SLAVE_BASE LPI2C3
#define BOARD_LPI2C_SLAVE_IRQn LPI2C3_IRQn
#define BOARD_FLEXIO_BASE      FLEXIO0
#define LPI2C_CLOCK_FREQUENCY  CLOCK_GetIpFreq(kCLOCK_Lpi2c3)
#define FLEXIO_CLOCK_FREQUENCY CLOCK_GetIpFreq(kCLOCK_Flexio0)
#define FLEXIO_I2C_SDA_PIN     1U
#define FLEXIO_I2C_SCL_PIN     0U

/* I2C Baudrate 400K */
#define I2C_BAUDRATE (400000)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
