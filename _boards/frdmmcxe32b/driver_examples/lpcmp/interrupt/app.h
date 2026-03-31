/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

#include "fsl_siul2.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define DEMO_LPCMP_BASE             LPCMP_1
#define DEMO_LPCMP_USER_CHANNEL     0U
#define DEMO_LPCMP_DAC_CHANNEL      0U
#define DEMO_LPCMP_IRQ_ID           CMP1_IRQn
#define DEMO_LPCMP_IRQ_HANDLER_FUNC CMP1_IRQHandler

#define BOARD_SIUL2_BASE     SIUL2
#define BOARD_LED_GPIO       BOARD_LED_RED_GPIO 
#define BOARD_LED_GPIO_PIN   BOARD_LED_RED_GPIO_PIN
#define BOARD_LED_OFF_LOGIC  LOGIC_LED_OFF

#define LED_INIT() \
    SIUL2_SetPinOutputBuffer(BOARD_SIUL2_BASE, \
                            (BOARD_LED_BLUE_GPIO * 32U + BOARD_LED_GPIO_PIN), \
                            true,  \
                            kPORT_MUX_AS_GPIO); \
    SIUL2_PortPinWrite(BOARD_SIUL2_BASE, \
                       BOARD_LED_GPIO, \
                       BOARD_LED_GPIO_PIN, \
                       BOARD_LED_OFF_LOGIC)


#define LED_ON()        SIUL2_PortClear(BOARD_SIUL2_BASE, BOARD_LED_GPIO, 1u << BOARD_LED_GPIO_PIN)
#define LED_OFF()       SIUL2_PortSet(BOARD_SIUL2_BASE, BOARD_LED_GPIO, 1u << BOARD_LED_GPIO_PIN)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
