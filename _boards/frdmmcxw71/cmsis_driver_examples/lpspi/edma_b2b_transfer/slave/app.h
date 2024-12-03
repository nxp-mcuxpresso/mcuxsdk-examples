/*
 * Copyright 2024 NXP
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
/* Master related */
#define DRIVER_SLAVE_SPI                 Driver_SPI0
#define EXAMPLE_LPSPI_SLAVE_DMA_BASEADDR DMA0
#define EXAMPLE_LPSPI_SLAVE_IRQN         (LPSPI0_IRQn)
#define LPSPI_SLAVE_CLK_FREQ             (CLOCK_GetIpFreq(EXAMPLE_LPSPI_SLAVE_CLOCK_NAME))
#define EXAMPLE_LPSPI_SLAVE_CLOCK_NAME   (kCLOCK_Lpspi0)
#define EXAMPLE_LPSPI_SLAVE_CLOCK_SOURCE (kCLOCK_IpSrcFro192M)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif
