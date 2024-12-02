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
#define DEMO_ADC_IRQHANDLER     GAU_GPADC0_INT_FUNC11_IRQHandler
#define DEMO_ADC_BASE           GAU_GPADC0
#define DEMO_ADC_CHANNEL_SOURCE kADC_CH4
#define DEMO_ADC_IRQn           GAU_GPADC0_INT_FUNC11_IRQn

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
