/*
 * Copyright 2019 NXP
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
#define EXAMPLE_I2C_MASTER_BASE (LPI2C1_BASE)
#define EXAMPLE_I2C_MASTER_IRQN (LPI2C1_IRQn)

#define EXAMPLE_I2C_SLAVE_BASE (LPI2C2_BASE)
#define EXAMPLE_I2C_SLAVE_IRQN (LPI2C2_IRQn)

#define LPI2C_CLOCK_FREQUENCY 48000000U

/*INTMUX setting*/
#define EXAMPLE_LPSPI_INTMUX_CHANNEL (0U)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
