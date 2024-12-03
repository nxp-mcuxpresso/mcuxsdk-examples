/*
 * Copyright 2024 NXP
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
#define DEMO_ADC_IRQn             ADC_ER_IRQn
#define DEMO_ADC_IRQ_HANDLER_FUNC ADC_ER_IRQHandler

#define DEMO_USED_CHANNELS 3U
#define DEMO_ADC_CHANNEL0  0U
#define DEMO_ADC_CHANNEL1  1U
#define DEMO_ADC_CHANNEL2  2U
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
