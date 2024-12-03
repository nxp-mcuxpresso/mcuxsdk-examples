/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Variables
 ******************************************************************************/
/*${variable:start}*/

/*${variable:end}*/

extern uint32_t excludeFromDS[2];
extern uint32_t wakeupFromDS[4];
extern uint32_t excludeFromPD[1];
extern uint32_t wakeupFromPD[4];
extern uint32_t excludeFromDPD[1];
extern uint32_t wakeupFromDPD[2];

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define DEMO_WAKEUP_WITH_GINT            (1)
#define APP_USART_RX_ERROR               kUSART_RxError
#define APP_RUNNING_INTERNAL_CLOCK       BOARD_BootClockFRO12M()
#define APP_USER_WAKEUP_KEY_NAME         "SW1"
#define APP_USER_WAKEUP_KEY_GPIO         BOARD_SW1_GPIO
#define APP_USER_WAKEUP_KEY_PORT         BOARD_SW1_GPIO_PORT
#define APP_USER_WAKEUP_KEY_PIN          BOARD_SW1_GPIO_PIN
#define APP_USER_WAKEUP_KEY_INPUTMUX_SEL kINPUTMUX_GpioPort1Pin18ToPintsel

#define DEMO_GINT0_PORT kGINT_Port0

/* Select one input, active low for GINT0 */
#define DEMO_GINT0_POL_MASK ~(1U << BOARD_SW3_GPIO_PIN)
#define DEMO_GINT0_ENA_MASK (1U << BOARD_SW3_GPIO_PIN)

#define APP_EXCLUDE_FROM_DEEPSLEEP (excludeFromDS)

#define APP_EXCLUDE_FROM_POWERDOWN (excludeFromPD)

#define APP_EXCLUDE_FROM_DEEPPOWERDOWN (excludeFromDPD)

#define APP_WAKEUP_FROM_DEEPSLEEP (wakeupFromDS)

#define APP_WAKEUP_FROM_POWERDOWN (wakeupFromPD)

#define APP_WAKEUP_FROM_DEEPPOWERDOWN (wakeupFromDPD)

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
