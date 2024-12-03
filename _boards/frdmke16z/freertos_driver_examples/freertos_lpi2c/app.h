/*
 * Copyright 2018 NXP
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
#define EXAMPLE_CONNECT_I2C BOARD_TO_BOARD
#define I2C_MASTER_SLAVE    isMASTER /* Change to isSLAVE to run as slave. */

#define EXAMPLE_I2C_MASTER_BASE (LPI2C0_BASE)
#define EXAMPLE_I2C_MASTER_IRQN (LPI2C0_IRQn)

#define EXAMPLE_I2C_SLAVE_BASE (LPI2C0_BASE)
#define EXAMPLE_I2C_SLAVE_IRQN (LPI2C0_IRQn)

#define LPI2C_CLOCK_FREQUENCY (CLOCK_GetIpFreq(kCLOCK_Lpi2c0))
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
