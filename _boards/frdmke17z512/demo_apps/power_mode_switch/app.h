/*
 * Copyright 2023 NXP
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

#define APP_LPTMR            DEMO_LPTMR_PERIPHERAL
#define APP_LPTMR_IRQHANDLER DEMO_LPTMR_IRQHANDLER

#define APP_WAKEUP_BUTTON_GPIO        BOARD_SW2_GPIO
#define APP_WAKEUP_BUTTON_PORT        BOARD_SW2_PORT
#define APP_WAKEUP_BUTTON_GPIO_PIN    BOARD_SW2_GPIO_PIN
#define APP_WAKEUP_BUTTON_IRQ         BOARD_SW2_IRQ
#define APP_WAKEUP_BUTTON_IRQ_HANDLER BOARD_SW2_IRQ_HANDLER
#define APP_WAKEUP_BUTTON_NAME        BOARD_SW2_NAME
#define APP_WAKEUP_BUTTON_IRQ_TYPE    kPORT_InterruptFallingEdge

/* Debug console RX pin: PORTD MUX: 17 */
#define DEBUG_CONSOLE_RX_PORT   PORTD
#define DEBUG_CONSOLE_RX_GPIO   GPIOD
#define DEBUG_CONSOLE_RX_PIN    17U
#define DEBUG_CONSOLE_RX_PINMUX kPORT_MuxAlt3
/* Debug console TX pin: PORTE MUX: 12 */
#define DEBUG_CONSOLE_TX_PORT   PORTE
#define DEBUG_CONSOLE_TX_GPIO   GPIOE
#define DEBUG_CONSOLE_TX_PIN    12U
#define DEBUG_CONSOLE_TX_PINMUX kPORT_MuxAlt3
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
