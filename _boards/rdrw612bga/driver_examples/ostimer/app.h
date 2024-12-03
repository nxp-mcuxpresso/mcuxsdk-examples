/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
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
#define EXAMPLE_OSTIMER      OSTIMER
#define EXAMPLE_OSTIMER_FREQ CLOCK_GetOSTimerClkFreq()

/* Leave AON modules on in PM2 */
#define APP_PM2_MEM_PU_CFG ((uint32_t)kPOWER_Pm2MemPuAon1 | (uint32_t)kPOWER_Pm2MemPuAon0)
/* All ANA in low power mode in PM2 */
#define APP_PM2_ANA_PU_CFG (0U)
/* Buck18 and Buck11 both sleep in PM3 */
#define APP_PM3_BUCK_CFG (0U)
/* All clock gated */
#define APP_SOURCE_CLK_GATE ((uint32_t)kPOWER_ClkGateAll)
/* All SRAM kept in retention in PM3, AON SRAM shutdown in PM4 */
#define APP_MEM_PD_CFG (1UL << 8)

#define EXAMPLE_OSTIMER_IRQn OS_EVENT_IRQn

/* Enable OSTIMER IRQ under deep sleep mode. */
void EXAMPLE_EnableDeepSleepIRQ(void);
/* Enter deep sleep mode. */
void EXAMPLE_EnterDeepSleep(void);

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
