/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
/* The PWM base address */
#define BOARD_PWM_BASEADDR PWM1

#define PWM_SRC_CLK_FREQ       HAL_ClockGetRate(hal_clock_busaon)
#define DEMO_PWM_CLOCK_DEVIDER kPWM_Prescale_Divide_128
#define APP_DEFAULT_PWM_FREQUENCE (16UL)
#define TRANSACTION_TIME_US 10U
#define ENCODER_ACCESS_FREQ_VS_PWM_FRE0 		16

#define DEMO_XBARA_BASEADDR            XBARA1
#define DEMO_XBARA_IRQn                XBAR1_CH0_CH1_IRQn
#define DEMO_XBARA_IRQHandler          XBAR1_CH0_CH1_IRQHandler

#define BOARD_ENDAT3_BASEADDR 	ENDAT3
#define DEMO_ENDAT3_FG_IRQn		Reserved164_IRQn
#define DEMO_FG_IRQ_IRQHandler Reserved164_IRQHandler

#define DEMO_ENDAT3_BG_IRQn 	Reserved163_IRQn
#define DEMO_BG_IRQ_IRQHandler 	Reserved163_IRQHandler

#define ENDAT3_CLOCK_ROOT       hal_clock_endat31fast
#define ENDAT3_SOURCE_CLOCK     HAL_ClockGetFreq(ENDAT3_CLOCK_ROOT)
#define ENDAT3_STRETCHER_CTRL_ASYNC_EN 1
#define ENDAT3_STRETCHER_CTRL_POL_SEL 1
#define ENDAT3_STRETCHER_CTRL_HW_STROBE_COUNTER  7

#define ENCODER_TYPE_ROTARY

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void PWM_Trigger_Init(PWM_Type *PWMBase);
/*${prototype:end}*/

#endif /* _APP_H_ */

