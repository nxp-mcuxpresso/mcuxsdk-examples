/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef APP_H_
#define APP_H_

/*${header:start}*/
#include "fsl_endat2p2.h"
#include "pin_mux.h"
#include "fsl_pwm.h"
#include "fsl_xbar.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/

#define ENDAT2P2_BASE           ENDAT2P2_1

/* Interrupt number and interrupt handler for the TPM instance used */
#define ENDAT2P2_IRQn           Reserved163_IRQn
#define ENDAT2P2_IRQHandler     Reserved163_IRQnHandler

#define ENDAT2P2_FTCLK          2000000 /* 2MHz */
#define ENDAT2P2_SOURCE_CLOCK   HAL_ClockGetFreq(hal_clock_endat21)

/* The PWM base address */
#define BOARD_PWM_BASEADDR              PWM1
#define PWM_SRC_CLK_FREQ                HAL_ClockGetRate(hal_clock_busaon)
#define DEMO_PWM_CLOCK_DEVIDER          kPWM_Prescale_Divide_128
#define APP_DEFAULT_PWM_FREQUENCE       (16UL)
#define TRANSACTION_TIME_US             10U
#define ENCODER_ACCESS_FREQ_VS_PWM_FRE0 16

/* XBAR */
#define DEMO_XBARA_BASEADDR            XBARA1
#define DEMO_XBARA_IRQn                XBAR1_CH0_CH1_IRQn
#define DEMO_XBARA_IRQHandler          XBAR1_CH0_CH1_IRQHandler

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void PWM_Trigger_Init(PWM_Type *PWMBase);
/*${prototype:end}*/

#endif /* APP_H_ */
