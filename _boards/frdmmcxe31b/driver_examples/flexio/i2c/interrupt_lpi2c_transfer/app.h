/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define BOARD_FLEXIO_BASE FLEXIO

#define I2C_BAUDRATE 400000

#define FLEXIO_CLOCK_FREQUENCY CLOCK_GetFreq(kCLOCK_FlexioClk)

#define FLEXIO_I2C_SDA_PIN 6U
#define FLEXIO_I2C_SCL_PIN 7U

#define BOARD_LPI2C_SLAVE_BASE LPI2C_1
#define BOARD_LPI2C_SLAVE_IRQn LPI2C_1_IRQn

#define LPI2C_CLOCK_FREQUENCY CLOCK_GetFreq(kCLOCK_Lpi2c1Clk)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
