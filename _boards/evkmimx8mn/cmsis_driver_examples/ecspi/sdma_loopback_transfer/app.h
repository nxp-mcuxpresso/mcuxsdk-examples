/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "fsl_ecspi_cmsis.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define DRIVER_MASTER_SPI          Driver_SPI2
#define DRIVER_MASTER_SPI_DMA_BASE SDMAARM1
#define EXAMPLE_MASTER_SPI_BASE    ECSPI2
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void ECSPI_EnableLoopBackTransfer(ECSPI_Type *base);
/*${prototype:end}*/

#endif /* _APP_H_ */
