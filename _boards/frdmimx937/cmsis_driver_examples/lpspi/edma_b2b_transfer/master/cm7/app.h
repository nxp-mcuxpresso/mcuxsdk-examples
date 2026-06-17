/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "fsl_clock.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define EXAMPLE_LPSPI_MASTER_IRQN       (LPSPI7_IRQn)
#define EXAMPLE_LPSPI_MASTER_CLOCK_NAME (kCLOCK_lpspi7)

#define EXAMPLE_LPSPI_MASTER_DMA_BASEADDR EDMA5_2

#define DRIVER_MASTER_SPI         Driver_SPI7
#define EXAMPLE_LPSPI_DEALY_COUNT 0xfffffU
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
