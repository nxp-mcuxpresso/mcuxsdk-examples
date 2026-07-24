/*
 * Copyright 2021, 2026 NXP
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
/* Slave related */
#define EXAMPLE_LPSPI_SLAVE_BASEADDR   HSP__LPSPI_1
#define EXAMPLE_LPSPI_SLAVE_IRQN       HSP_LPSPI1_IRQn
#define EXAMPLE_LPSPI_SLAVE_IRQHandler HSP_LPSPI1_IRQHandler

#define EXAMPLE_LPSPI_SLAVE_PCS_FOR_INIT     kLPSPI_Pcs0
#define EXAMPLE_LPSPI_SLAVE_PCS_FOR_TRANSFER kLPSPI_SlavePcs0

#define LPSPI_MASTER_CLK_FREQ CLOCK_GetRootClockFreq(kCLOCK_Root_MAIN_lpspi1_fclk)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif
