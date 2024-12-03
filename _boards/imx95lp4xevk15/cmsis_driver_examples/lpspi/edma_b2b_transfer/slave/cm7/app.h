/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define EXAMPLE_LPSPI_SLAVE_IRQN       (LPSPI1_IRQn)
#define EXAMPLE_LPSPI_SLAVE_CLOCK_NAME (hal_clock_lpspi1)

#define EXAMPLE_LPSPI_SLAVE_DMA_BASEADDR DMA3

#define DRIVER_SLAVE_SPI Driver_SPI1
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
