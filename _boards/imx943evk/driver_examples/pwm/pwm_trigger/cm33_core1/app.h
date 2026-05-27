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

#define PWM_SRC_CLK_FREQ       CLOCK_GetRate(kCLOCK_Busaon)
#define DEMO_PWM_CLOCK_DEVIDER kFLEXPWM_Prescale_Divide_4
/*${macro:end}*/

/* define instance */
#define BOARD_TPM_BASEADDR TPM6
#define BOARD_TPM_CHANNEL  kTPM_Chnl_0

/* Interrupt to enable and flag to read; depends on the TPM channel used */
#define TPM_CHANNEL_INTERRUPT_ENABLE kTPM_Chnl0InterruptEnable
#define TPM_CHANNEL_FLAG             kTPM_Chnl0Flag

/* Interrupt number and interrupt handler for the TPM instance used */
#define TPM_INTERRUPT_NUMBER TPM6_IRQn
#define TPM_LED_HANDLER      TPM6_IRQHandler

/* Get source clock for TPM driver */
#define LPTPM_CLOCK_ROOT kCLOCK_Tpm6
#define TPM_SOURCE_CLOCK CLOCK_GetRate(LPTPM_CLOCK_ROOT)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
