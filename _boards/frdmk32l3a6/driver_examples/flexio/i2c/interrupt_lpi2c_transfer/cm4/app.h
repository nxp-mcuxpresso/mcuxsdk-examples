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

#define BOARD_FLEXIO_BASE FLEXIO0

#define MASTER_FLEXIO_CLOCK_NAME   (kCLOCK_Flexio0)
#define MASTER_FLEXIO_CLOCK_SOURCE (kCLOCK_IpSrcFircAsync)
#define FLEXIO_CLOCK_FREQUENCY     (CLOCK_GetIpFreq(MASTER_FLEXIO_CLOCK_NAME))

#define FLEXIO_I2C_SDA_PIN 4U
#define FLEXIO_I2C_SCL_PIN 3U

#define BOARD_LPI2C_SLAVE_BASE LPI2C0
#define BOARD_LPI2C_SLAVE_IRQn LPI2C0_IRQn

#define SLAVE_LPI2C_CLOCK_NAME   (kCLOCK_Lpi2c0)
#define SLAVE_LPI2C_CLOCK_SOURCE (kCLOCK_IpSrcFircAsync)

#define LPI2C_CLOCK_FREQUENCY (CLOCK_GetIpFreq(SLAVE_LPI2C_CLOCK_NAME))

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
