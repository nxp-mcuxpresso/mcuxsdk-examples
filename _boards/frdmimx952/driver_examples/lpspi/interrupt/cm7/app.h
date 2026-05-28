/*
 * Copyright 2026 NXP
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
#define EXAMPLE_LPSPI_MASTER_BASEADDR   (LPSPI3)
#define EXAMPLE_LPSPI_MASTER_IRQN       (LPSPI3_IRQn)
#define EXAMPLE_LPSPI_MASTER_IRQHandler (LPSPI3_IRQHandler)

#define EXAMPLE_LPSPI_MASTER_PCS_FOR_INIT     (kLPSPI_Pcs0)
#define EXAMPLE_LPSPI_MASTER_PCS_FOR_TRANSFER (kLPSPI_MasterPcs0)

#define EXAMPLE_LPSPI_MASTER_CLK_ID (kCLOCK_lpspi3)

/* Slave related */
#define EXAMPLE_LPSPI_SLAVE_BASEADDR   (LPSPI6)
#define EXAMPLE_LPSPI_SLAVE_IRQN       (LPSPI6_IRQn)
#define EXAMPLE_LPSPI_SLAVE_IRQHandler (LPSPI6_IRQHandler)

#define EXAMPLE_LPSPI_SLAVE_PCS_FOR_INIT     (kLPSPI_Pcs0)
#define EXAMPLE_LPSPI_SLAVE_PCS_FOR_TRANSFER (kLPSPI_SlavePcs0)

#define EXAMPLE_LPSPI_SLAVE_CLK_ID (kCLOCK_lpspi6)

#define EXAMPLE_LPSPI_MASTER_CLOCK_FREQ CLOCK_GetRate(EXAMPLE_LPSPI_MASTER_CLK_ID) 
#define EXAMPLE_LPSPI_SLAVE_CLOCK_FREQ  CLOCK_GetRate(EXAMPLE_LPSPI_SLAVE_CLK_ID)

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif
