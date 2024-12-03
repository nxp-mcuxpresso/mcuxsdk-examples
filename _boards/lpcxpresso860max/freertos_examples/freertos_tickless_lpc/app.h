/*
 * Copyright 2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define DEMO_PINT_PIN_INT1_SRC kSYSCON_GpioPort0Pin4ToPintsel

#define BOARD_LED_GPIO     BOARD_LED_GREEN_GPIO
#define BOARD_LED_GPIO_PIN BOARD_LED_GREEN_GPIO_PIN

#define BOARD_SW_GPIO        BOARD_SW2_GPIO
#define BOARD_SW_PORT        BOARD_SW2_GPIO_PORT
#define BOARD_SW_GPIO_PIN    BOARD_SW2_GPIO_PIN
#define BOARD_SW_IRQ         PIN_INT0_IRQn
#define BOARD_SW_IRQ_HANDLER PIN_INT0_IRQHandler
#define BOARD_SW_NAME        BOARD_SW2_NAME

/* @brief FreeRTOS tickless timer configuration. */
#define TICKLESS_OSTIMER_BASE_PTR OSTIMER0      /*!< Tickless timer base address. */
#define TICKLESS_OSTIMER_IRQn     OS_EVENT_IRQn /*!< Tickless timer IRQ number. */

#define BOARD_PINT_PIN_INT_SRC kINPUTMUX_GpioInt4ToDma
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
