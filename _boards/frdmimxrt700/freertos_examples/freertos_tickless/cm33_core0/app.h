/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/

#define BOARD_SW_GPIO        BOARD_SW5_GPIO
#define BOARD_SW_GPIO_PIN    BOARD_SW5_GPIO_PIN
#define BOARD_SW_IRQ         PIN_INT0_IRQn
#define BOARD_SW_IRQ_HANDLER PIN_INT0_IRQHandler
#define BOARD_SW_NAME        BOARD_SW5_NAME

#define PINT                   PINT0
#define INPUTMUX               INPUTMUX0
#define BOARD_PINT_PIN_INT_SRC kINPUTMUX_GpioPort0Pin9ToPintsel
/* @brief FreeRTOS tickless timer configuration. */
#define BOARD_OSTIMER_TIMER_INSTANCE OSTIMER_CPU0  /*!< Tickless timer base address. */
#define BOARD_OSTIMER_TIMER_IRQ      OS_EVENT_IRQn /*!< Tickless timer IRQ number. */
#define BOARD_TIMER_IRQ_HANDLER      OS_EVENT_IRQHandler
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
