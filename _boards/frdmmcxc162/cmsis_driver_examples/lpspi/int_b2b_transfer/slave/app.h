/*
 * Copyright 2026 NXP
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
#define EXAMPLE_LPSPI_SLAVE_BASEADDR         (LPSPI0)
#define EXAMPLE_LPSPI_SLAVE_IRQN             (LPSPI0_IRQn)
#define EXAMPLE_LPSPI_SLAVE_PCS_FOR_INIT     (kLPSPI_Pcs1)
#define EXAMPLE_LPSPI_SLAVE_PCS_FOR_TRANSFER (kLPSPI_SlavePcs1)
#define EXAMPLE_LPSPI_SLAVE_IRQHandler       (LPSPI0_IRQHandler)
#define DRIVER_SLAVE_SPI                      Driver_SPI0
#define EXAMPLE_LPSPI_CLOCK_FREQ              CLOCK_GetLpspiClkFreq(0)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
