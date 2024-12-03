/*
 * Copyright 2024 NXP
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
/* Whether the SW buttons use the separate IRQ handler */
#define DEMO_SW_USE_SEPARATE_HANDLER 1
#define LIN_CLOCK_NAME               kCLOCK_ScgSircClk
#define TJA_WAKEUP                   1
#define TIMER_TPM                    1
/* Whether to disable the PrintBuffer function */
#define DISABLE_PRINT_BUFFER 0
#define DEMO_TPM_BASEADDR    TPM0
#define DEMO_TPM_IRQn        TPM0_IRQn
#define DEMO_TPM_IRQHandler  TPM0_IRQHandler
#define DEMO_TPM_CLOCK       kCLOCK_Tpm0
/* timer frequency */
#define TIMER_FREQ CLOCK_GetFreq(kCLOCK_ScgSircClk)
/* (timer period (us) * (timer clock frequency)(Hz)) - 1 ) */
#define MODULO_VALUE ((500U * (CLOCK_GetFreq(kCLOCK_ScgSircClk) / 1000000U)) - 1U)
/* nanoseconds / timer clock frequency  */
#define TIMER_1TICK_DURATION_PS (1000000000000U / TIMER_FREQ)

#define DEMO_LIN_IRQHandler LPUART0_IRQHandler

#define LI0_Master 0x0

#define DEMO_BUTTON1_GPIO BOARD_SW3_GPIO
#define DEMO_BUTTON1_PORT BOARD_SW3_PORT
#define DEMO_BUTTON1_PIN  BOARD_SW3_GPIO_PIN
#define DEMO_BUTTON2_GPIO BOARD_SW2_GPIO
#define DEMO_BUTTON2_PORT BOARD_SW2_PORT
#define DEMO_BUTTON2_PIN  BOARD_SW2_GPIO_PIN
#define DEMO_LED_GPIO     BOARD_LED3_GPIO
#define DEMO_LED_PIN      BOARD_LED3_GPIO_PIN
#define DEMO_SLPN_GPIO    GPIOC
#define DEMO_SLPN_PIN     7
#define DEMO_LED_ON()     LED3_ON()
#define DEMO_LED_OFF()    LED3_OFF()

#define DEMO_SW2_IRQn        BOARD_SW2_IRQ
#define DEMO_SW2_IRQ_HANDLER BOARD_SW2_IRQ_HANDLER
#define DEMO_SW3_IRQn        BOARD_SW3_IRQ
#define DEMO_SW3_IRQ_HANDLER BOARD_SW3_IRQ_HANDLER
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
