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
#define EXAMPLE_SPI_SLAVE     SPI1
#define EXAMPLE_SPI_SSEL      1
#define EXAMPLE_SPI_SPOL      SPI_CFG_SPOL1(0)
#define EXAMPLE_SPI_SLAVE_IRQ FLEXCOMM1_IRQn
#define SPI_SLAVE_IRQHandler  FLEXCOMM1_IRQHandler
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
