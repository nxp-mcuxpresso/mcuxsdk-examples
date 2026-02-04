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
#define EXAMPLE_LPSPI_MASTER_BASEADDR         (LPSPI0)
#define EXAMPLE_LPSPI_MASTER_IRQN             (LPSPI0_IRQn)
#define EXAMPLE_LPSPI_DEALY_COUNT             0xFFFFF
#define LPSPI_MASTER_CLK_FREQ                 (CLOCK_GetLpspiClkFreq(0))
#define EXAMPLE_LPSPI_MASTER_PCS_FOR_INIT     (kLPSPI_Pcs0)
#define EXAMPLE_LPSPI_MASTER_PCS_FOR_TRANSFER (kLPSPI_MasterPcs0)
#define EXAMPLE_LPSPI_MASTER_IRQHandler       (LPSPI0_IRQHandler)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
