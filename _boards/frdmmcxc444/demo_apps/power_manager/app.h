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
#define APP_DEBUG_UART_BAUDRATE 9600 /* Debug console baud rate. */

/* Default debug console clock source. */
#define APP_DEBUG_UART_DEFAULT_CLKSRC_NAME kCLOCK_McgPeriphClk /* MCGPCLK */
#define APP_DEBUG_UART_DEFAULT_CLKSRC      0x01                /* MCGPCLK */

/* Debug console clock source in VLPR mode. */
#define APP_DEBUG_UART_VLPR_CLKSRC_NAME kCLOCK_McgInternalRefClk /* MCGIRCLK */
#define APP_DEBUG_UART_VLPR_CLKSRC      0x03                     /* MCGIRCCLK */

#define LLWU_LPTMR_IDX       0U                                  /* LLWU_M0IF */
#define LLWU_WAKEUP_PIN_IDX  7U                                  /* LLWU_P7 */
#define LLWU_WAKEUP_PIN_TYPE kLLWU_ExternalPinFallingEdge

#define APP_WAKEUP_BUTTON_GPIO        BOARD_SW2_GPIO
#define APP_WAKEUP_BUTTON_PORT        BOARD_SW2_PORT
#define APP_WAKEUP_BUTTON_GPIO_PIN    BOARD_SW2_GPIO_PIN
#define APP_WAKEUP_BUTTON_IRQ         BOARD_SW2_IRQ
#define APP_WAKEUP_BUTTON_IRQ_HANDLER BOARD_SW2_IRQ_HANDLER
#define APP_WAKEUP_BUTTON_NAME        BOARD_SW2_NAME
#define APP_WAKEUP_BUTTON_IRQ_TYPE    kPORT_InterruptFallingEdge

/* Debug console RX pin: PORTA1 MUX: 2 */
#define DEBUG_CONSOLE_RX_PORT   PORTA
#define DEBUG_CONSOLE_RX_GPIO   GPIOA
#define DEBUG_CONSOLE_RX_PIN    1
#define DEBUG_CONSOLE_RX_PINMUX kPORT_MuxAlt2
/* Debug console TX pin: PORTA2 MUX: 2 */
#define DEBUG_CONSOLE_TX_PORT   PORTA
#define DEBUG_CONSOLE_TX_GPIO   GPIOA
#define DEBUG_CONSOLE_TX_PIN    2
#define DEBUG_CONSOLE_TX_PINMUX kPORT_MuxAlt2
#define CORE_CLK_FREQ           CLOCK_GetFreq(kCLOCK_CoreSysClk)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
