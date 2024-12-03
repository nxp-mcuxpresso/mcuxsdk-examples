/*
 * Copyright 2016-2018 NXP
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
#define APP_DEBUG_UART_BAUDRATE 9600U /* Debug console baud rate.           */

/* Default debug console clock source. */
#define APP_DEBUG_UART_DEFAULT_CLKSRC_NAME kCLOCK_ScgSircClk /* SCG SIRC clock. */

#define APP_WAKEUP_BUTTON_GPIO        BOARD_SW3_GPIO
#define APP_WAKEUP_BUTTON_PORT        BOARD_SW3_PORT
#define APP_WAKEUP_BUTTON_GPIO_PIN    BOARD_SW3_GPIO_PIN
#define APP_WAKEUP_BUTTON_IRQ         BOARD_SW3_IRQ
#define APP_WAKEUP_BUTTON_IRQ_HANDLER BOARD_SW3_IRQ_HANDLER
#define APP_WAKEUP_BUTTON_NAME        BOARD_SW3_NAME
#define APP_WAKEUP_BUTTON_IRQ_TYPE    kPORT_InterruptFallingEdge

/* Debug console RX pin: PORTB MUX: 2 */
#define DEBUG_CONSOLE_RX_PORT   PORTB
#define DEBUG_CONSOLE_RX_GPIO   GPIOB
#define DEBUG_CONSOLE_RX_PIN    0U
#define DEBUG_CONSOLE_RX_PINMUX kPORT_MuxAlt2
/* Debug console TX pin: PORTB MUX: 2 */
#define DEBUG_CONSOLE_TX_PORT   PORTB
#define DEBUG_CONSOLE_TX_GPIO   GPIOB
#define DEBUG_CONSOLE_TX_PIN    1U
#define DEBUG_CONSOLE_TX_PINMUX kPORT_MuxAlt2
#define CORE_CLK_FREQ           CLOCK_GetFreq(kCLOCK_CoreSysClk)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
extern void APP_InitClock(void); /* SCG init function defined in clock_scg.c */
void BOARD_InitHardware(void);
void APP_PowerPreSwitchHook(smc_power_state_t originPowerState, app_power_mode_t targetMode);
void APP_PowerPostSwitchHook(smc_power_state_t originPowerState, app_power_mode_t targetMode);
/*${prototype:end}*/

#endif /* _APP_H_ */
