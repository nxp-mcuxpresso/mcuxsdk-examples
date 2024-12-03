/*
 * Copyright 2020 NXP
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
#define DEMO_CAPT_BASE            CAPT
#define DEMO_CAPT_IRQn            CMP_CAPT_IRQn
#define DEMO_CAPT_IRQHandler      CMP_CAPT_IRQHandler
#define DEMO_CAPT_ENABLE_PINS     kCAPT_X0Pin | kCAPT_X1Pin | kCAPT_X2Pin | kCAPT_X3Pin | kCAPT_X4Pin
#define DEMO_CAPT_ENABLE_PINS_NUM 5
#define DEMO_CAPT_CLOCK_FREQ      CLOCK_GetFroFreq()
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
