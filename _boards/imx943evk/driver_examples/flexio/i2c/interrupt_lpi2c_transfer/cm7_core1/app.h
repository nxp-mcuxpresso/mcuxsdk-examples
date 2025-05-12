/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "fsl_clock.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define BOARD_LPI2C_SLAVE_BASE LPI2C4
#define BOARD_LPI2C_SLAVE_IRQn LPI2C4_IRQn
#define BOARD_FLEXIO_BASE      FLEXIO1
#define DEMO_LPI2C_CLOCK (kCLOCK_Lpi2c4)
#define LPI2C_CLOCK_FREQUENCY  CLOCK_GetRate(DEMO_LPI2C_CLOCK)
#define DEMO_FLEXIO_CLOCK (kCLOCK_Flexio1)
#define FLEXIO_CLOCK_FREQUENCY CLOCK_GetRate(DEMO_FLEXIO_CLOCK)
#define FLEXIO_I2C_SDA_PIN     4U
#define FLEXIO_I2C_SCL_PIN     5U

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
