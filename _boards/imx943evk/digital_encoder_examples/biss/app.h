/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef APP_H_
#define APP_H_

/*${header:start}*/
#include "fsl_biss.h"
#include "pin_mux.h"
#include "fsl_pwm.h"
#include "fsl_xbar.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/

#define BISS_BASE               BISS1

/* Interrupt number and interrupt handler for the TPM instance used */
#define BISS_SOT_IRQn           Reserved165_IRQn
#define BISS_SOT_IRQHandler     Reserved165_IRQHandler
#define BISS_EOT_IRQn           Reserved166_IRQn
#define BISS_EOT_IRQHandler     Reserved166_IRQHandler

#define BISS_SYS_CLK_ROOT       hal_clock_biss
#define BISS_SYS_CLK_FREQ       20000000UL /* 20MHz */
#define BISS_MA_CLK_FREQ        200000  /* 200KHz */
#define BISS_AGS_CLK_FREQ       1000   /* 1KHz */

#define BISS_DEVICE_WR_ER_LEN   2
#define BISS_DEVICE_MT_LEN      12
#define BISS_DEVICE_ST_LEN      16
#define BISS_DEVICE_DATA_LEN    (BISS_DEVICE_MT_LEN + BISS_DEVICE_ST_LEN + BISS_DEVICE_WR_ER_LEN)
#define BISS_DEVICE_CRC_LEN     6

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
