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
#define BOARD_SIUL2_BASE     SIUL2
#define BOARD_LED_GPIO       BOARD_LED_RED_GPIO 
#define BOARD_LED_GPIO_PIN   BOARD_LED_RED_GPIO_PIN
#define BOARD_SW_EIRQ        13U /* EIRQ13*/
#define BOARD_SW_NAME        "SW3"

#define EXAMPLE_DMAMUX_BASEADDR DMAMUX_1
#define EXAMPLE_DMAMUX_CHANNEL  0U
#define EXAMPLE_DMA_REQUEST     kDmaRequestMux1SIUL2Req13
#define EXAMPLE_DMA_BASEADDR    EDMA
#define DEMO_DMA_CHANNEL        16U
#define APP_DMA_IRQ             DMATCD16_IRQn
#define APP_DMA_IRQ_HANDLER     DMATCD16_IRQHandler
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
