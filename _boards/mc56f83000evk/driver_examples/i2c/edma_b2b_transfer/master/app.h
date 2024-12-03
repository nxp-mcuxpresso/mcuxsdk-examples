/*
 * Copyright 2020 NXP
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
/* I2C source clock */
#define I2C_MASTER_CLK_FREQ         CLOCK_GetIpClkSrcFreq(kCLOCK_I2C0)
#define EXAMPLE_I2C_MASTER_BASEADDR I2C0
#define EXAMPLE_DMA_REQUEST_SRC     kDmaRequestMuxI2C0
#define EXAMPLE_I2C_DMAMUX_BASEADDR DMAMUX
#define EXAMPLE_I2C_DMAMUX_CHANNEL  kDMAMUX_DMAChannel0
#define EXAMPLE_I2C_DMA_BASEADDR    DMA0
#define EXAMPLE_I2C_DMA_CHANNEL     kEDMA_Channel0
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
