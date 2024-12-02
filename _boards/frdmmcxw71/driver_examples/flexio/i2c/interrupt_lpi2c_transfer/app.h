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
#define BOARD_FLEXIO_BASE FLEXIO0

#define I2C_BAUDRATE 400000

#define FLEXIO_CLOCK_FREQUENCY CLOCK_GetIpFreq(kCLOCK_Flexio0)

#define FLEXIO_I2C_SDA_PIN 30U
#define FLEXIO_I2C_SCL_PIN 31U

#define BOARD_LPI2C_SLAVE_BASE LPI2C0
#define BOARD_LPI2C_SLAVE_IRQn LPI2C0_IRQn

#define LPI2C_CLOCK_FREQUENCY (CLOCK_GetIpFreq(kCLOCK_Lpi2c0))
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
