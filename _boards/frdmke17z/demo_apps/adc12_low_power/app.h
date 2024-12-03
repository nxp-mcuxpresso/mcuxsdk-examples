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
#define DEMO_ADC12_BASEADDR      ADC0
#define DEMO_ADC12_CHANNEL_GROUP 0U

#define DEMO_ADC12_IRQ_ID           ADC0_IRQn
#define DEMO_ADC12_IRQ_HANDLER_FUNC ADC0_IRQHandler

/*
 * Low Power Timer interrupt time in millisecond
 * LPTMR may use LPO 1KHz or LPO 128KHz clock as prescaler/glitch filter clock, this is chip specific.
 */
#define LPTMR_COMPARE_VALUE (500 * 128U)
#define DEMO_LPTMR_BASE     LPTMR0

#define DEMO_ADC12_CLOCK_NAME kCLOCK_Adc0

#define LED1_INIT() LED_RED_INIT(LOGIC_LED_OFF)
#define LED1_ON()   LED_RED_ON()
#define LED1_OFF()  LED_RED_OFF()

#define LED2_INIT() LED_GREEN_INIT(LOGIC_LED_OFF)
#define LED2_ON()   LED_GREEN_ON()
#define LED2_OFF()  LED_GREEN_OFF()
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void BOARD_ConfigTriggerSource(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
