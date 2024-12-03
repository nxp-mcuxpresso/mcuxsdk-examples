/*
 * Copyright 2019 NXP
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
#define DEMO_SW_USE_SEPARATE_HANDLER 1U
#define DEMO_TIMER_TPM_ENABLE        1U
#define DEMO_TPM_BASEADDR            TPM0
#define DEMO_TPM_IRQn                TPM0_IRQn
#define DEMO_TPM_IRQHandler          TPM0_IRQHandler
#define DEMO_TIMER_FREQ              CLOCK_GetFreq(kCLOCK_ScgSircClk)
#define DEMO_TPM_CLOCK               kCLOCK_Tpm0

/* (timer period (us) * (timer clock frequency)(Hz)) - 1 ). */
#define DEMO_MODULO_VALUE        ((500U * (CLOCK_GetFreq(kCLOCK_ScgSircClk) / 1000000U)) - 1U)
#define DEMO_TIMER_TICKS_1US     (DEMO_TIMER_FREQ / 1000000)
#define DEMO_TIMER_COMPARE_VALUE (500U * (CLOCK_GetFreq(kCLOCK_ScgSircClk) / 1000000U))

/* Which lpuart instance was used. */
#define DEMO_MASTER_LPUART_INSTANCE 0U
#define DEMO_LIN_IRQHandler         LPUART0_IRQHandler

#define DEMO_BUTTON1_GPIO BOARD_SW3_GPIO
#define DEMO_BUTTON1_PORT BOARD_SW3_PORT
#define DEMO_BUTTON1_PIN  BOARD_SW3_GPIO_PIN
#define DEMO_BUTTON2_GPIO BOARD_SW2_GPIO
#define DEMO_BUTTON2_PORT BOARD_SW2_PORT
#define DEMO_BUTTON2_PIN  BOARD_SW2_GPIO_PIN

#define DEMO_LED1_GPIO BOARD_LED1_GPIO
#define DEMO_LED1_PIN  BOARD_LED1_GPIO_PIN
#define DEMO_LED2_GPIO BOARD_LED2_GPIO
#define DEMO_LED2_PIN  BOARD_LED2_GPIO_PIN
#define DEMO_LED3_GPIO BOARD_LED3_GPIO
#define DEMO_LED3_PIN  BOARD_LED3_GPIO_PIN

#define DEMO_LED1_ON()  LED1_ON()
#define DEMO_LED1_OFF() LED1_OFF()
#define DEMO_LED2_ON()  LED2_ON()
#define DEMO_LED2_OFF() LED2_OFF()
#define DEMO_LED3_ON()  LED3_ON()
#define DEMO_LED3_OFF() LED3_OFF()

#define DEMO_SW3_IRQn        BOARD_SW3_IRQ
#define DEMO_SW3_IRQ_HANDLER BOARD_SW3_IRQ_HANDLER
#define DEMO_SW2_IRQn        BOARD_SW2_IRQ
#define DEMO_SW2_IRQ_HANDLER BOARD_SW2_IRQ_HANDLER
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
