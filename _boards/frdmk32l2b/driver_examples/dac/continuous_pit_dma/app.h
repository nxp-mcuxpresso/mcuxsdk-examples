/*
 * Copyright 2019 NXP
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
#define DEMO_DAC_BASEADDR    DAC0
#define DAC_DATA_REG_ADDR    0x4003F000U
#define DEMO_DMAMUX_BASEADDR DMAMUX0
#define DEMO_DMA_CHANNEL     0U
#define DEMO_DMA_DAC_SOURCE  45U
#define DEMO_DMA_BASEADDR    DMA0
#define DEMO_DMA_IRQ_ID      DMA0_IRQn
/* Get source clock for PIT driver */
#define PIT_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_BusClk)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
