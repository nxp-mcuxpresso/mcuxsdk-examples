/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define BOARD_SW_GPIO          BOARD_SW4_GPIO_PORT
#define BOARD_SW_GPIO_PIN      BOARD_SW4_GPIO_PIN
#define BOARD_PINT_PIN_INT_SRC kINPUTMUX_GpioPort0Pin25ToPintsel
#define BOARD_SW_IRQ           PIN_INT0_IRQn
#define BOARD_SW_IRQ_HANDLER   PIN_INT0_DriverIRQHandler
#define BOARD_SW_NAME          "SW4"

/* @brief FreeRTOS tickless timer configuration. */
#define TICKLESS_RTC_BASE_PTR RTC      /*!< Tickless timer base address. */
#define TICKLESS_RTC_IRQn     RTC_IRQn /*!< Tickless timer IRQ number. */

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
