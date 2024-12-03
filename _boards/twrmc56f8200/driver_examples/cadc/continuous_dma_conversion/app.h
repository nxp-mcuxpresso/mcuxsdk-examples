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
#define CADC_THERMISTOR_CHANNEL_NUMBER kCADC_SingleEndANA4_DiffANA4pANA5n
#define CDAC_THERMISTOR_CHANNEL_MODE   kCADC_ANA4_5_FullyDifferential
#define CADC_RESULT_REG_BYTE_ADDR      SDK_GET_REGISTER_BYTE_ADDR(ADC_Type, ADC, RSLT[0])

#define DEMO_DMA_CHANNEL_REQUEST_SOURCE kDmaCH0_ADCA_ES
#define DEMO_DMA_BASEADDR               DMA0
#define DEMO_DMA_CHANNEL_0              kDMA_Channel0

#define DEMO_CLOCK_IP_NAME kCLOCK_CYCADC

/*${macro:end}*/
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
extern void DMA_Channel0_IRQHandler(void);
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
