/*
 * Copyright 2017 NXP
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
#define EXAMPLE_LPI2C_DMAMUX_BASEADDR DMAMUX1
#define EXAMPLE_LPI2C_DMA_BASEADDR    DMA1
#define DMA0_IRQn                     DMA1_04_IRQn
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
