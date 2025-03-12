/*
 * Copyright 2025 NXP
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
#define APP_SCG_SPLL_PREDIV  0 /* Devide by 1.    */
#define APP_SCG_SPLL_MULT    2 /* Multiply by 18. */

#define APP_SCG_SOSC_FREQ      8000000U
#define APP_SCG_SOSC_DIV2      kSCG_AsyncClkDivBy1
#define APP_SCG_SOSC_WORK_MODE kSCG_SysOscModeOscLowPower
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
