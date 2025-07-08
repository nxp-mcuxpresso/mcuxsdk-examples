/*
 * Copyright 2019-2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define BOARD_SW_GPIO          BOARD_SW2_GPIO
#define BOARD_SW_GPIO_PORT     BOARD_SW2_GPIO_PORT
#define BOARD_SW_GPIO_PIN      BOARD_SW2_GPIO_PIN
#define BOARD_PINT_PIN_INT_SRC kINPUTMUX_GpioPort0Pin10ToPintsel
#define BOARD_SW_IRQ           PIN_INT0_IRQn
#define BOARD_SW_NAME          "SW2"
#define BOARD_SW_IRQ_HANDLER   PIN_INT0_IRQHandler

/* @brief FreeRTOS tickless timer configuration. */
#define BOARD_RTC_TIMER_INSTANCE RTC      /*!< Tickless timer base address. */
#define BOARD_RTC_TIMER_IRQ      RTC_IRQn /*!< Tickless timer IRQ number. */
#define BOARD_TIMER_IRQ_HANDLER  RTC_IRQHandler
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
