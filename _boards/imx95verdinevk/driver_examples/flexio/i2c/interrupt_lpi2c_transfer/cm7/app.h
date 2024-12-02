/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "hal_clock.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define BOARD_LPI2C_SLAVE_BASE LPI2C6
#define BOARD_LPI2C_SLAVE_IRQn LPI2C6_IRQn
#define BOARD_FLEXIO_BASE      FLEXIO1
#define LPI2C_CLOCK_FREQUENCY  HAL_ClockGetIpFreq(hal_clock_lpi2c6)
#define FLEXIO_CLOCK_FREQUENCY HAL_ClockGetIpFreq(hal_clock_flexio1)
#define FLEXIO_I2C_SDA_PIN     18U
#define FLEXIO_I2C_SCL_PIN     13U

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
