/*
 * Copyright 2022 NXP
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
#define DRIVER_MASTER_SPI                 Driver_SPI0
#define EXAMPLE_LPSPI_MASTER_IRQN         (LPSPI0_IRQn)
#define EXAMPLE_LPSPI_MASTER_DMA_BASEADDR DMA0
#define LPSPI_MASTER_CLK_FREQ             (CLOCK_GetIpFreq(EXAMPLE_LPSPI_MASTER_CLOCK_NAME))
#define EXAMPLE_LPSPI_MASTER_CLOCK_NAME   (kCLOCK_Lpspi0)
#define EXAMPLE_LPSPI_MASTER_CLOCK_SOURCE (kCLOCK_IpSrcFro192M)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif
