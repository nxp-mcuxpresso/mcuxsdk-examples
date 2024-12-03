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
#define DEMO_DAC_BASE             DAC
#define DEMO_DAC_ENABLE_ONESHOT   false
#define DEMO_DAC_MAX_SETTING_TIME 2.0f /* DAC module's maximum setting time, the unit is us.*/
#define DEMO_CLOCK_FREQUENCY      CLOCK_GetFreq(kCLOCK_SysClk)
#define DEMO_CLOCK_IP_NAME        kCLOCK_DAC
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
