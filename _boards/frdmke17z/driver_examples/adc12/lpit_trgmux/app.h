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
#define DEMO_ADC12_BASEADDR ADC0
/* PTA0, ADC0_SEL0 */
#define DEMO_ADC12_USER_CHANNEL (0U)

#define DEMO_ADC12_CHANNEL_GROUP    (0U)
#define DEMO_ADC12_IRQ_ID           ADC0_IRQn
#define DEMO_ADC12_IRQ_HANDLER_FUNC ADC0_IRQHandler

#define DEMO_LPIT_USER_CHANNEL  kLPIT_Chnl_0
#define DEMO_LPIT_USER_TIMER_CH kLPIT_Trigger_TimerChn0
#define DEMO_LPIT_SOURCECLOCK   CLOCK_GetIpFreq(kCLOCK_Lpit0)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void BOARD_ConfigTriggerSource(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
