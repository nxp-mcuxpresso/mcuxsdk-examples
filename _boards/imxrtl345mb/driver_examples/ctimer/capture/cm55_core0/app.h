/*
 * Copyright 2025-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

#include "fsl_swm.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define DEMO_CTIMER   CTRLSS__CTIMER_2
#define DEMO_GPIO     SWM_TO_GPIO_PORT(SWM2_03)
#define DEMO_GPIO_PIN SWM_TO_GPIO_PIN(SWM2_03)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void DEMO_InitCtimerInput(void);
void DEMO_InitGpioPin(void);
void DEMO_PullGpioPin(uint8_t level);
/*${prototype:end}*/

#endif /* _APP_H_ */
/*******************************************************************************
 * EOF
 ******************************************************************************/