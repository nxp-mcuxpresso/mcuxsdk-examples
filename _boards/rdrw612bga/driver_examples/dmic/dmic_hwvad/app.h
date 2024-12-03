/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "fsl_power.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define DEMO_DMIC_CHANNEL_1        kDMIC_Channel2
#define DEMO_DMIC_CHANNEL_1_ENABLE DMIC_CHANEN_EN_CH2(1)

#define BOARD_LED_GPIO_PORT BOARD_LED_BLUE_GPIO_PORT
#define BOARD_LED_GPIO_PIN  BOARD_LED_BLUE_GPIO_PIN
#define DisableDeepSleepIRQ POWER_DisableWakeup
#define EnableDeepSleepIRQ  POWER_EnableWakeup
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
