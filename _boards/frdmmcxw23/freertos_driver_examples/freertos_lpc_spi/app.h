/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/

#define EXAMPLE_SPI_MASTER      SPI1
#define EXAMPLE_SPI_MASTER_IRQ  FLEXCOMM1_IRQn
#define EXAMPLE_SPI_SLAVE       SPI2
#define EXAMPLE_SPI_SLAVE_IRQ   FLEXCOMM2_IRQn
#define EXAMPLE_SPI_SSEL        kSPI_Ssel1
#define EXAMPLE_MASTER_SPI_SPOL SPI_CFG_SPOL1(0)
#define EXAMPLE_SLAVE_SPI_SPOL  SPI_CFG_SPOL1(0)

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
