/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016 NXP
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

#define EXAMPLE_SPI_MASTER      SPI2
#define EXAMPLE_SPI_MASTER_IRQ  FLEXCOMM2_IRQn
#define EXAMPLE_SPI_SLAVE       SPI8
#define EXAMPLE_SPI_SLAVE_IRQ   FLEXCOMM8_IRQn
#define EXAMPLE_SPI_SSEL        kSPI_Ssel0
#define EXAMPLE_MASTER_SPI_SPOL kSPI_SpolActiveAllLow
#define EXAMPLE_SLAVE_SPI_SPOL  kSPI_SpolActiveAllLow

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
