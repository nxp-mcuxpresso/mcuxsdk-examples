/*
 * Copyright 2021 NXP
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
/* The PWM base address */
#define DEMO_PWM_BASEADDR                  PWM1
#define DEMO_PWM_SUBMODULE                 (0U)
#define DEMO_PWM_SUBMODULE_MASK            kFLEXPWM_SubmoduleMask_0
#define DEMO_PWM_FAULT_INPUT_PIN_MASK      kFLEXPWM_FaultMask_0
#define DEMO_PWM_DELAY_VAL                 0x0FFFU
#define DEMO_PWM_CHANNEL_LOCATION_ON_BOARD "J17-6"

#define DEMO_CMP_BASE                        CMP1
#define DEMO_CMP_USER_CHANNEL                0U
#define DEMO_CMP_DAC_CHANNEL                 7U
#define DEMO_CMP_INPUT_PIN_LOCATION_ON_BOARD "J17-9"

#define DEMO_DEADTIME_VAL 650U
#define PWM_SRC_CLK_FREQ  CLOCK_GetFreq(kCLOCK_IpgClk)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
