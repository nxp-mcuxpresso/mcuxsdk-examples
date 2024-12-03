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
#define DEMO_LPADC_BASE                  ADC0
#define DEMO_LPADC_USER_CHANNEL          0U
#define DEMO_LPADC_USER_CMDID            1U /* CMD1 */
#define DEMO_LPADC_VREF_SOURCE           kLPADC_ReferenceVoltageAlt2
#define DEMO_LPADC_DO_OFFSET_CALIBRATION false
#define DEMO_LPADC_OFFSET_VALUE_A        0x10U
#define DEMO_LPADC_OFFSET_VALUE_B        0x10U
#define DEMO_LPADC_RESFIFO_REG_ADDR      (uint32_t)(&(ADC0->RESFIFO[0]))
#define DEMO_RESULT_FIFO_READY_FLAG      kLPADC_ResultFIFO0ReadyFlag

#define DEMO_DMA_BASE          DMA0
#define DEMO_DMA_ADC_CHANNEL   21U
#define DEMO_DMA_TRANSFER_TYPE kDMA_PeripheralToMemory
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
