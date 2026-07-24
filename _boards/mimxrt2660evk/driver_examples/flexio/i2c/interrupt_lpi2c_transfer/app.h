/*
 * Copyright 2024, 2026 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define BOARD_LPI2C_SLAVE_BASE HSP__LPI2C_1
#define BOARD_LPI2C_SLAVE_IRQn HSP_LPI2C1_IRQn

/* Get frequency of lpi2c clock */
#define LPI2C_CLOCK_FREQUENCY CLOCK_GetRootClockFreq(kCLOCK_Root_MAIN_lpi2c1_fclk)

#define BOARD_FLEXIO_BASE      HSP__FLEXIO_1
#define FLEXIO_CLOCK_FREQUENCY CLOCK_GetRootClockFreq(kCLOCK_Root_MAIN_flexio1_fclk)
#define FLEXIO_I2C_SDA_PIN     20U
#define FLEXIO_I2C_SCL_PIN     21U

/* I2C Baudrate 100K */
#define I2C_BAUDRATE (100000)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
