/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define APP_DEBUG_UART_BAUDRATE    9600          /* Debug console baud rate. */
#define APP_DEBUG_UART_CLKSRC_NAME kCLOCK_BusClk /* Bus clock. */

#define LLWU_LPTMR_IDX       0U                  /* LLWU_M0IF */
#define LLWU_WAKEUP_PIN_IDX  15U                 /* LLWU_P15 */
#define LLWU_WAKEUP_PIN_TYPE kLLWU_ExternalPinRisingEdge

#define APP_WAKEUP_BUTTON_GPIO        BOARD_SW1_GPIO
#define APP_WAKEUP_BUTTON_PORT        BOARD_SW1_PORT
#define APP_WAKEUP_BUTTON_GPIO_PIN    BOARD_SW1_GPIO_PIN
#define APP_WAKEUP_BUTTON_IRQ         BOARD_SW1_IRQ
#define APP_WAKEUP_BUTTON_IRQ_HANDLER BOARD_SW1_IRQ_HANDLER
#define APP_WAKEUP_BUTTON_NAME        BOARD_SW1_NAME
#define APP_WAKEUP_BUTTON_IRQ_TYPE    kPORT_InterruptFallingEdge
/* Debug console RX pin: PORTI6 MUX: 2 */
#define DEBUG_CONSOLE_RX_PORT   PORTI
#define DEBUG_CONSOLE_RX_GPIO   GPIOI
#define DEBUG_CONSOLE_RX_PIN    6U
#define DEBUG_CONSOLE_RX_PINMUX kPORT_MuxAlt2
/* Debug console TX pin: PORTI7 MUX: 2 */
#define DEBUG_CONSOLE_TX_PORT   PORTI
#define DEBUG_CONSOLE_TX_GPIO   GPIOI
#define DEBUG_CONSOLE_TX_PIN    7U
#define DEBUG_CONSOLE_TX_PINMUX kPORT_MuxAlt2
#define CORE_CLK_FREQ           CLOCK_GetFreq(kCLOCK_CoreSysClk)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void APP_PowerPostSwitchHook(smc_power_state_t originPowerState, app_power_mode_t targetMode);
void APP_PowerPreSwitchHook(smc_power_state_t originPowerState, app_power_mode_t targetMode);
/*${prototype:end}*/

#endif /* _APP_H_ */
