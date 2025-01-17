/*
 * Copyright 2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define BOARD_LPI2C_SLAVE_BASE LPI2C1
#define BOARD_LPI2C_SLAVE_IRQn LPI2C1_IRQn
#define BOARD_FLEXIO_BASE      FLEXIO1
#define LPI2C_CLOCK_FREQUENCY  CLOCK_GetIpFreq(kCLOCK_Root_Lpi2c1)
#define FLEXIO_CLOCK_FREQUENCY CLOCK_GetIpFreq(kCLOCK_Root_Flexio1)
#define FLEXIO_I2C_SDA_PIN     3U
#define FLEXIO_I2C_SCL_PIN     2U

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
