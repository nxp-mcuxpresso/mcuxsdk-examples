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
#define DEMO_CADC_BASEADDR             ADC
#define DEMO_CLOCK_IP_NAME             kCLOCK_CYCADC
#define DEMO_DMAMUX_BASEADDR           DMAMUX
#define DEMO_DMA_ADC_SOURCE            kDmaRequestMuxADCAES
#define DEMO_DMA_CHANNEL_0             kEDMA_Channel0
#define DEMO_DMA_CHANNEL_1             kEDMA_Channel1
#define DEMO_DMA_BASEADDR              DMA0
#define CADC_RESULT_REG_BYTE_ADDR      SDK_GET_REGISTER_BYTE_ADDR(ADC_Type, ADC, RSLT[0])
#define CDAC_THERMISTOR_CHANNEL_MODE   kCADC_ANA6_7_FullyDifferential
#define CADC_THERMISTOR_CHANNEL_NUMBER kCADC_SingleEndANA6_DiffANA6pANA7n
#define DEMO_CADC_SAMPLE_COUNT         16U /* The cadc sample count. */

#define DEMO_XBARA_USER_CHANNEL_INPUT  kXBARA_InputPit0SyncOut
#define DEMO_XBARA_USER_CHANNEL_OUTPUT kXBARA_OutputAdcSync0
#define DEMO_XBARA_BASEADDR            XBARA

#define DEMO_PIT_BASEADDR        PIT0
#define PIT_CLOCK_SOURCE         kPIT_CountBusClk         /* 50M bus clock */
#define PIT_CLOCK_SOURCE_DIVIDER kPIT_PrescalerDivBy16384 /* divide source by 32768*/
/* Equation: interrupt rate = peripheral clock rate / ((2 ^ prescaler) * modulo value). */
#define PIT_PERIOD_COUNT 382U /* 0.125s count of clock cycles, calculate by MSEC_TO_COUNT(382, source_clock/divider)*/
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
extern void DMA_Channel0_IRQHandler(void);
void BOARD_InitHardware(void);
void EDMA_Configuration(void);
void DMAMUX_Configuration(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
