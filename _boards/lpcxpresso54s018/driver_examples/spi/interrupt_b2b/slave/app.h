/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
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
#define EXAMPLE_SPI_SLAVE     SPI9
#define EXAMPLE_SPI_SLAVE_IRQ FLEXCOMM9_IRQn
#define EXAMPLE_SPI_SSEL      0
#define SPI_SLAVE_IRQHandler  FLEXCOMM9_IRQHandler
#define EXAMPLE_SPI_SPOL      kSPI_SpolActiveAllLow
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
