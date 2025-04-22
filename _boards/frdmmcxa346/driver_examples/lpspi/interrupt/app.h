/*
 * Copyright 2024 NXP
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
/* Master related */
#define EXAMPLE_LPSPI_MASTER_BASEADDR   (LPSPI0)
#define EXAMPLE_LPSPI_MASTER_IRQN       (LPSPI0_IRQn)
#define EXAMPLE_LPSPI_MASTER_IRQHandler (LPSPI0_IRQHandler)

#define EXAMPLE_LPSPI_MASTER_PCS_FOR_INIT     (kLPSPI_Pcs0)
#define EXAMPLE_LPSPI_MASTER_PCS_FOR_TRANSFER (kLPSPI_MasterPcs0)

/* Slave related */
#define EXAMPLE_LPSPI_SLAVE_BASEADDR   (LPSPI1)
#define EXAMPLE_LPSPI_SLAVE_IRQN       (LPSPI1_IRQn)
#define EXAMPLE_LPSPI_SLAVE_IRQHandler (LPSPI1_IRQHandler)

#define EXAMPLE_LPSPI_SLAVE_PCS_FOR_INIT     (kLPSPI_Pcs0)
#define EXAMPLE_LPSPI_SLAVE_PCS_FOR_TRANSFER (kLPSPI_SlavePcs0)

#define EXAMPLE_LPSPI_MASTER_CLOCK_FREQ CLOCK_GetLpspiClkFreq(0u)
#define EXAMPLE_LPSPI_SLAVE_CLOCK_FREQ  CLOCK_GetLpspiClkFreq(1u)

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif
