/*
 * Copyright 2021 NXP
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
#define DEMO_ADC16_BASE    SADC
#define DEMO_ADC16_GROUPID 0U
#define DEMO_DMA_BASEADDR  DMA0
#define DEMO_DMA_CHANNEL_0 kDMA_Channel0
#define DEMO_DMA_IRQ_ID    DMA0_IRQn
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
