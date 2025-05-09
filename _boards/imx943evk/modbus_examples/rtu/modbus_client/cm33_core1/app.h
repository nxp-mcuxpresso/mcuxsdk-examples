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
#define LPUART_CLOCK_ROOT      kCLOCK_Lpuart11
#define DEMO_LPUART_CLK_FREQ   CLOCK_GetRate(LPUART_CLOCK_ROOT)
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
#define LPTPM6_CLOCK_ROOT kCLOCK_Tpm6
#define TPM_SOURCE_CLOCK CLOCK_GetRate(LPTPM6_CLOCK_ROOT)

#define MB_MASTER_DELAY_US_CONVERT              (200000 )
#define MB_MASTER_TIMEOUT_US_RESPOND            (100000 )

#define BOARD_TPM_SECOND TPM5
/* Interrupt number and interrupt handler for the TPM instance used */
#define BOARD_TPM_SECOND_IRQ_NUM TPM5_IRQn
#define BOARD_TPM_SECOND_HANDLER TPM5_IRQHandler

/* Get source clock for TPM driver */
#define LPTPM5_CLOCK_ROOT kCLOCK_Tpm5

#define TIME_PERIOD_US  1750U
#ifndef TPM_PRESCALER
/* Calculate the clock division based on the PWM frequency to be obtained */
#define TPM_PRESCALER TPM_CalculateCounterClkDiv(BOARD_TPM, 1000000U / TIME_PERIOD_US, TPM_SOURCE_CLOCK);
#endif
#ifndef TPM_PRESCALER_CONVERT
/* Calculate the clock division based on the PWM frequency to be obtained */
#define TPM_PRESCALER_CONVERT TPM_CalculateCounterClkDiv(BOARD_TPM, 1000000U / MB_MASTER_DELAY_US_CONVERT, TPM_SOURCE_CLOCK);
#endif
#ifndef TPM_PRESCALER_RESPOND
/* Calculate the clock division based on the PWM frequency to be obtained */
#define TPM_PRESCALER_RESPOND TPM_CalculateCounterClkDiv(BOARD_TPM, 1000000U / MB_MASTER_TIMEOUT_US_RESPOND, TPM_SOURCE_CLOCK);
#endif

/* Set counter period to 1ms */
#define DEMO_TIMER_PERIOD_US (100000U)
/* Calculate the clock division based on the PWM frequency to be obtained */
#define TPM2_PRESCALER TPM_CalculateCounterClkDiv(BOARD_TPM5, 1000000U / DEMO_TIMER_PERIOD_US, TPM_SOURCE_CLOCK);

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */