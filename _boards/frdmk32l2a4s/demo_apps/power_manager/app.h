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
#define APP_DEBUG_UART_BAUDRATE 9600 /* Debug console baud rate.           */

#define LLWU_LPTMR_IDX       0U      /* LLWU_M0IF */
#define LLWU_WAKEUP_PIN_IDX  2U      /* LLWU_P2  */
#define LLWU_WAKEUP_PIN_TYPE kLLWU_ExternalPinFallingEdge

#define APP_WAKEUP_BUTTON_GPIO        BOARD_SW3_GPIO
#define APP_WAKEUP_BUTTON_PORT        BOARD_SW3_PORT
#define APP_WAKEUP_BUTTON_GPIO_PIN    BOARD_SW3_GPIO_PIN
#define APP_WAKEUP_BUTTON_IRQ         BOARD_SW3_IRQ
#define APP_WAKEUP_BUTTON_IRQ_HANDLER BOARD_SW3_IRQ_HANDLER
#define APP_WAKEUP_BUTTON_NAME        BOARD_SW3_NAME
#define APP_WAKEUP_BUTTON_IRQ_TYPE    kPORT_InterruptFallingEdge

/* Debug console RX pin: PORTB16 MUX: 3 */
#define DEBUG_CONSOLE_RX_PORT   PORTB
#define DEBUG_CONSOLE_RX_GPIO   GPIOB
#define DEBUG_CONSOLE_RX_PIN    16
#define DEBUG_CONSOLE_RX_PINMUX kPORT_MuxAlt3
/* Debug console TX pin: PORTB17 MUX: 3 */
#define DEBUG_CONSOLE_TX_PORT   PORTB
#define DEBUG_CONSOLE_TX_GPIO   GPIOB
#define DEBUG_CONSOLE_TX_PIN    17
#define DEBUG_CONSOLE_TX_PINMUX kPORT_MuxAlt3
#define CORE_CLK_FREQ           CLOCK_GetFreq(kCLOCK_CoreSysClk)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
