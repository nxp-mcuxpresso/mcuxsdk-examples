/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
/* The PWM base address */
#define BOARD_PWM_BASEADDR PWM1

#define PWM_SRC_CLK_FREQ       CLOCK_GetRate(kCLOCK_Busaon)
#define DEMO_PWM_CLOCK_DEVIDER kFLEXPWM_Prescale_Divide_4
#define DEMO_ENABLE_FAULT_TO_PWM_MAPPING (0)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
