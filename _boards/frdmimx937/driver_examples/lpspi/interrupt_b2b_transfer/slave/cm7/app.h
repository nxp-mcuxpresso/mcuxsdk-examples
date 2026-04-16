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
#define EXAMPLE_LPSPI_SLAVE_BASEADDR         LPSPI3
#define EXAMPLE_LPSPI_SLAVE_IRQN             LPSPI3_IRQn
#define EXAMPLE_LPSPI_SLAVE_PCS_FOR_INIT     kLPSPI_Pcs0
#define EXAMPLE_LPSPI_SLAVE_PCS_FOR_TRANSFER kLPSPI_SlavePcs0

#define LPSPI_SLAVE_CLOCK_ROOT         kCLOCK_lpspi3
#define EXAMPLE_LPSPI_SLAVE_CLOCK_NAME (LPSPI_SLAVE_CLOCK_ROOT)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
