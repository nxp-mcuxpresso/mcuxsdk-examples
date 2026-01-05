/*
 * Copyright 2023-2024 NXP
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
#define DEMO_ADC_BASE             ADC
#define DEMO_ADC_IRQn             ADC_WD_IRQn
#define DEMO_ADC_IRQ_HANDLER_FUNC ADC_WD_IRQHandler

#define DEMO_ADC_CHANNEL      1U
#define DEMO_ADC_WDGID        0U
#define DEMO_ADC_WDG_INT_TYPE kADC_HighThresholdIntEnable

/* The range of 12bit ADC is [0, 4095],  */
#define DEMO_ADC_LOW_THRESHOLD  0U
#define DEMO_ADC_HIGH_THRESHOLD 3900U
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
