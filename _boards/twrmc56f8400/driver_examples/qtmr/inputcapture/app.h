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
#define DEMO_DMA_BASEADDR               DMA0
#define DEMO_DMA_CHANNEL                kDMA_Channel0
#define DEMO_DMA_CHANNEL_IRQn           DMA0_IRQn
#define DEMO_DMA_CHANNEL_REQUEST_SOURCE kDmaCH0_TMRA0_CP

#define BOARD_QTMR_BASEADDR       TMRA
#define BOARD_QTMR_CHANNEL        kQTMR_Channel0
#define QTMR_INPUT_CAPTURE_SOURCE kQTMR_SecondarySrcInputPin2

/* QTMR Clock source divider for Ip bus clock source, the value of two macros below should be aligned. */
#define QTMR_PRIMARY_SOURCE       (kQTMR_PrimarySrcIPBusClockDivide2)
#define QTMR_CLOCK_SOURCE_DIVIDER (2U)
#define QTMR_SOURCE_CLOCK         (CLOCK_GetFreq(kCLOCK_BusClk) / QTMR_CLOCK_SOURCE_DIVIDER)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void DMA_Channel_IRQHandler(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
