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
#define EXAMPLE_I2C_MASTER Driver_I2C1
#define EXAMPLE_LPI2C_DMA_BASEADDR (EDMA)
#define EXAMPLE_LPI2C_DMAMUX_BASEADDR DMAMUX_1
#define LPI2C_CLOCK_FREQUENCY CLOCK_GetFreq(kCLOCK_Lpi2c1Clk)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
