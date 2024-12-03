/*
 * Copyright 2017 NXP
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
#define EXAMPLE_I2C_MASTER            Driver_I2C3
#define EXAMPLE_LPI2C_DMAMUX_BASEADDR DMA_CH_MUX0
#define EXAMPLE_LPI2C_DMA_BASEADDR    DMA0

#define DMA0_IRQn DMA0_0_4_IRQn
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
