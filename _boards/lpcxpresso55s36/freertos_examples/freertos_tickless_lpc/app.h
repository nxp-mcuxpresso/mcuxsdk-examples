/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016 NXP
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
#define BOARD_LED_GPIO     BOARD_LED_GREEN_GPIO
#define BOARD_LED_GPIO_PIN BOARD_LED_GREEN_GPIO_PIN

#define BOARD_SW_GPIO        BOARD_SW3_GPIO
#define BOARD_SW_PORT        BOARD_SW3_GPIO_PORT
#define BOARD_SW_GPIO_PIN    BOARD_SW3_GPIO_PIN
#define BOARD_SW_IRQ         PIN_INT0_IRQn
#define BOARD_SW_IRQ_HANDLER PIN_INT0_IRQHandler
#define BOARD_SW_NAME        BOARD_SW3_NAME

/* @brief FreeRTOS tickless timer configuration. */
#define TICKLESS_OSTIMER_BASE_PTR OSTIMER0      /*!< Tickless timer base address. */
#define TICKLESS_OSTIMER_IRQn     OS_EVENT_IRQn /*!< Tickless timer IRQ number. */

#define BOARD_PINT_PIN_INT_SRC kINPUTMUX_GpioPort0Pin17ToPintsel
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
extern OSTIMER_Type *vPortGetOstimerBase(void);
extern IRQn_Type vPortGetOstimerIrqn(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
