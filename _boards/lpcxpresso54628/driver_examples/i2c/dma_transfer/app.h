/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
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
#define EXAMPLE_I2C_MASTER_BASE    (I2C9_BASE)
#define EXAMPLE_I2C_SLAVE_BASE     (I2C8_BASE)
#define I2C_MASTER_CLOCK_FREQUENCY (12000000)
#define I2C_SLAVE_CLOCK_FREQUENCY  (12000000)

#define EXAMPLE_DMA                DMA0
#define EXAMPLE_I2C_MASTER_CHANNEL 23

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
