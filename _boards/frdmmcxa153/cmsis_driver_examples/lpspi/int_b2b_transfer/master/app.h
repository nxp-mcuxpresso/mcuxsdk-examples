/*
 * Copyright 2023 NXP
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
#define EXAMPLE_LPSPI_DEALY_COUNT             0xFFFFF
#define EXAMPLE_LPSPI_MASTER_BASEADDR         (LPSPI0)
#define EXAMPLE_LPSPI_MASTER_IRQN             (LPSPI0_IRQn)
#define EXAMPLE_LPSPI_DEALY_COUNT             0xFFFFF
#define LPSPI_MASTER_CLK_FREQ                 (CLOCK_GetLpspiClkFreq(0))
#define EXAMPLE_LPSPI_MASTER_PCS_FOR_INIT     (kLPSPI_Pcs0)
#define EXAMPLE_LPSPI_MASTER_PCS_FOR_TRANSFER (kLPSPI_MasterPcs0)
#define EXAMPLE_LPSPI_MASTER_IRQHandler       (LPSPI0_IRQHandler)
#define DRIVER_MASTER_SPI                     Driver_SPI0
#define EXAMPLE_LPSPI_CLOCK_FREQ              CLOCK_GetLpspiClkFreq(0)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
