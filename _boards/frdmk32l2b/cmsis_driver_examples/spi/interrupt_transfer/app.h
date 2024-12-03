/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_
#include "fsl_clock.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/

#define EXAMPLE_SPI_MASTER_IRQ SPI0_IRQn
#define EXAMPLE_SPI_SLAVE_IRQ  SPI1_IRQn
#define DRIVER_MASTER_SPI      Driver_SPI0
#define DRIVER_SLAVE_SPI       Driver_SPI1

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
