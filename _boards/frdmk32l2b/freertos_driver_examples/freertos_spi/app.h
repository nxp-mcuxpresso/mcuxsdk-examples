/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define EXAMPLE_SPI_MASTER_BASE (SPI0_BASE)
#define EXAMPLE_SPI_MASTER_IRQN (SPI0_IRQn)
#define SPI_MASTER_CLK_SRC      (SPI0_CLK_SRC)
#define SPI_MASTER_CLK_FREQ     CLOCK_GetFreq((SPI0_CLK_SRC))
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
