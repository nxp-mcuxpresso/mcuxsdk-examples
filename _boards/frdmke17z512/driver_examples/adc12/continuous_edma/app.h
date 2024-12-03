/*
 * Copyright 2023 NXP
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
#define DEMO_ADC12_BASEADDR      ADC0
#define DEMO_ADC12_CLOCK_NAME    kCLOCK_Adc0
#define DEMO_ADC12_CLOCK_SOURCE  kADC12_ClockSourceAlt0
#define DEMO_ADC12_USER_CHANNEL  0U
#define DEMO_ADC12_CHANNEL_GROUP 0U
#define DEMO_DMAMUX_BASEADDR     DMAMUX
#define DEMO_DMA_CHANNEL         0U
#define DEMO_DMA_ADC_SOURCE      40U
#define DEMO_DMA_BASEADDR        DMA0
#define ADC12_RESULT_REG_ADDR    0x4003b048U
#define DEMO_DMA_IRQ_ID          DMA0_IRQn
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
