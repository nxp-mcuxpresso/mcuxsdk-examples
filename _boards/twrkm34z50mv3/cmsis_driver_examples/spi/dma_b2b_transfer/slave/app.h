/*
 * Copyright 2021 NXP
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
#define EXAMPLE_SPI_SLAVE_DMA_MUX_TX_BASEADDR DMAMUX3
#define EXAMPLE_SPI_SLAVE_DMA_MUX_RX_BASEADDR DMAMUX2
#define EXAMPLE_SPI_SLAVE_DMA_BASEADDR        DMA0
#define DRIVER_SLAVE_SPI                      Driver_SPI1
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
