/*
 * Copyright 2017 NXP
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
#define DEMO_ACOMP_BASE            ACOMP
#define DEMO_ACOMP_CAPT_CHANNEL    5U
#define DEMO_CAPT_BASE             CAPT
#define DEMO_CAPT_NOISE            0U
#define DEMO_CAPT_IRQ_NUMBER       CMP_CAPT_IRQn
#define DEMO_CAPT_IRQ_HANDLER_FUNC CMP_CAPT_DriverIRQHandler
#define DEMO_CAPT_ENABLE_PINS                                                                                       \
    kCAPT_X0Pin | kCAPT_X1Pin | kCAPT_X2Pin | kCAPT_X3Pin | kCAPT_X4Pin | kCAPT_X5Pin | kCAPT_X6Pin | kCAPT_X7Pin | \
        kCAPT_X8Pin

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void BOARD_InitLED(void);
void BOARD_LED_ON(uint32_t index);
void BOARD_LED_OFF(void);

/*${prototype:end}*/

#endif /* _APP_H_ */
