/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define BOARD_SW_GPIO        BOARD_WAKEUP_GPIO
#define BOARD_SW_PORT        BOARD_WAKEUP_GPIO_PORT
#define BOARD_SW_GPIO_PIN    BOARD_WAKEUP_GPIO_PIN
#define BOARD_SW_IRQ         PIN_INT0_IRQn
#define BOARD_SW_IRQ_HANDLER PIN_INT0_IRQHandler
#define BOARD_SW_NAME        "SW3"

/* @brief FreeRTOS tickless timer configuration. */
#define TICKLESS_RTC_BASE_PTR RTC       /*!< Tickless timer base address. */
#define TICKLESS_RTC_IRQn     RTC_IRQn  /*!< Tickless timer IRQ number. */

#define BOARD_PINT_PIN_INT_SRC kINPUTMUX_GpioPort0Pin21ToPintsel
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
extern void vPortRtcIsr(void);
IRQn_Type vPortGetRtcIrqn(void);
RTC_Type *vPortGetRtcBase(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
