/*
 * Copyright 2021-2022 NXP
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
#define DEMO_SFA_BASEADDR SFA0
#define DEMO_SFA_REF_FREQ CLOCK_GetSysOscFreq()

#define DEMO_CUT_SCG_CLK_OUT_SELECT kSFA_CUTSelect1
#define DEMO_CUT_32KHZ_SELECT       kSFA_CUTSelect6
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void APP_SetupFastClock(sfa_config_t *configPtr);
void APP_SetupSlowClock(sfa_config_t *configPtr);
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
