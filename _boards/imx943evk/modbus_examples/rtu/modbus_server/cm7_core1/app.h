/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "board.h"
#include "fsl_rgpio.h"
#include "fsl_lpuart.h"
#include "fsl_tpm.h"
/*${header:end}*/

/* Get source clock for LPIT driver */
#define DEMO_LPUART            LPUART11
#define DEMO_LPUART_INSTANCE_IDX (11U)
#define LPUART_CLOCK_ROOT      hal_clock_lpuart11
#define DEMO_LPUART_CLK_FREQ   HAL_ClockGetRate(LPUART_CLOCK_ROOT)
#define DEMO_LPUART_IRQn       LPUART11_IRQn
#define DEMO_LPUART_IRQHandler LPUART11_IRQHandler

#define BOARD_LED_RGPIO                 GPIO2
#define BOARD_LED_RGPIO_PIN1            31U
#define BOARD_LED_RGPIO_PIN2            1U

/* define instance */
#define BOARD_TPM TPM6

/* Interrupt number and interrupt handler for the TPM instance used */
#define BOARD_TPM_IRQ_NUM TPM6_IRQn
#define BOARD_TPM_HANDLER TPM6_IRQHandler

/* Get source clock for TPM driver */
#define LPTPM_CLOCK_ROOT hal_clock_tpm6
#define TPM_SOURCE_CLOCK HAL_ClockGetRate(LPTPM_CLOCK_ROOT)

#define TIME_PERIOD_US  1750U
#ifndef TPM_PRESCALER
/* Calculate the clock division based on the PWM frequency to be obtained */
#define TPM_PRESCALER TPM_CalculateCounterClkDiv(BOARD_TPM, 1000000U / TIME_PERIOD_US, TPM_SOURCE_CLOCK);
#endif

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */