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
/* Get source clock for LPIT driver */
#define DEMO_LPTMR_BASE LPTMR0
#define DEMO_LPTMR_COMPARE_VALUE
#define LPTMR_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_LpoClk)
/* Define LPTMR microseconds counts value */
#define LPTMR_USEC_COUNT     10000U
#define DEMO_DAC_BASEADDR    DAC0
#define DEMO_DMAMUX_BASEADDR DMAMUX0
#define DEMO_DMA_CHANNEL     0U
#define DEMO_DMA_DAC_SOURCE  54U
#define DEMO_DMA_BASEADDR    DMA0
#define DAC_DATA_REG_ADDR    0x4006a000U
#define DEMO_DMA_IRQ_ID      DMA0_IRQn
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
