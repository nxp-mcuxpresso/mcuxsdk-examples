/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define TIMER_PERIPHERAL      GPT1
#define TIMER_COMPARE_CHANNEL (kGPT_OutputCompare_Channel2)
#define TIMER_IRQ_HANDLER     GPT1_IRQHandler
#define TIMER_IRQ_ID          GPT1_IRQn

/* Get source clock for QTMR driver */
#define TIMER_CLOCK_SOURCE CLOCK_GetFreq(kCLOCK_IpgClk)

/* LPI2C */
#define I2C_BAUDRATE         (100000U)
#define EXAMPLE_LED_GPIO     BOARD_USER_LED_GPIO
#define EXAMPLE_LED_GPIO_PIN BOARD_USER_LED_GPIO_PIN
#define EXAMPLE_DELAY_COUNT  8000000

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
