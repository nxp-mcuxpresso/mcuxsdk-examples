/*
 * Copyright 2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define DEMO_LPIT_BASE       LPIT1
#define DEMO_LPIT_IRQn       LPIT1_IRQn
#define DEMO_LPIT_IRQHandler LPIT1_IRQHandler
/* Get source clock for LPIT driver */
#define LPIT_MASTER_CLOCK_ROOT kCLOCK_Root_Lpit1
#define LPIT_MASTER_CLOCK_GATE kCLOCK_Lpit1
#define LPIT_SOURCECLOCK       CLOCK_GetIpFreq(LPIT_MASTER_CLOCK_ROOT)
#define LED_INIT()
#define LED_TOGGLE()
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
