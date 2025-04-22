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
#define EXAMPLE_LPSPI_SLAVE_BASEADDR         (LPSPI1)
#define EXAMPLE_LPSPI_SLAVE_IRQN             (LPSPI1_IRQn)
#define EXAMPLE_LPSPI_SLAVE_PCS_FOR_INIT     (kLPSPI_Pcs1)
#define EXAMPLE_LPSPI_SLAVE_PCS_FOR_TRANSFER (kLPSPI_SlavePcs1)
#define EXAMPLE_LPSPI_SLAVE_IRQHandler       (LPSPI1_IRQHandler)
#define DRIVER_SLAVE_SPI                      Driver_SPI1
#define EXAMPLE_LPSPI_CLOCK_FREQ              CLOCK_GetLpspiClkFreq(1)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
