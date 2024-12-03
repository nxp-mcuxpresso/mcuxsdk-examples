/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
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
#define DEMO_WAKEUP_WITH_GINT            (1)
#define APP_USART_RX_ERROR               kUSART_RxError
#define APP_RUNNING_INTERNAL_CLOCK       BOARD_BootClockFRO12M()
#define APP_USER_WAKEUP_KEY_NAME         "SW3"
#define APP_USER_WAKEUP_KEY_GPIO         BOARD_SW3_GPIO
#define APP_USER_WAKEUP_KEY_PORT         BOARD_SW3_GPIO_PORT
#define APP_USER_WAKEUP_KEY_PIN          BOARD_SW3_GPIO_PIN
#define APP_USER_WAKEUP_KEY_INPUTMUX_SEL kINPUTMUX_GpioPort1Pin9ToPintsel

#define DEMO_GINT0_PORT kGINT_Port0

/* Select one input, active low for GINT0 */
#define DEMO_GINT0_POL_MASK ~(1U << BOARD_SW1_GPIO_PIN)
#define DEMO_GINT0_ENA_MASK (1U << BOARD_SW1_GPIO_PIN)

#define APP_EXCLUDE_FROM_SLEEP (kPDRUNCFG_PD_LDOMEM | kPDRUNCFG_PD_LDODEEPSLEEP)

#define APP_EXCLUDE_FROM_DEEPSLEEP (kPDRUNCFG_PD_DCDC | kPDRUNCFG_PD_FRO192M | kPDRUNCFG_PD_FRO32K)

#define APP_EXCLUDE_FROM_POWERDOWN (kPDRUNCFG_PD_LDOMEM | kPDRUNCFG_PD_FRO32K)

#define APP_EXCLUDE_FROM_DEEPPOWERDOWN (kPDRUNCFG_PD_LDOMEM | kPDRUNCFG_PD_FRO32K)

#define APP_SYSCON_STARTER_MASK SYSCON_STARTERSET_GPIO_INT00_SET_MASK
// #define INPUTMUX INPUTMUX0
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void DEMO_PreLowPower(void);
void DEMO_PowerDownWakeup(void);
void DEMO_PreDeepPowerDown(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
