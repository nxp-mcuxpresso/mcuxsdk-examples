/*
 * Copyright 2020,2021 NXP
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
