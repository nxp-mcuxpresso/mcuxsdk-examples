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

#define EXAMPLE_I2C_MASTER_IRQ FLEXCOMM1_IRQn
#define EXAMPLE_I2C_SLAVE_IRQ  FLEXCOMM2_IRQn

#define EXAMPLE_I2C_MASTER_BASE (I2C1_BASE)
#define EXAMPLE_I2C_SLAVE_BASE  (I2C2_BASE)

#define I2C_MASTER_CLOCK_FREQUENCY (12000000)
#define I2C_SLAVE_CLOCK_FREQUENCY  (12000000)

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
