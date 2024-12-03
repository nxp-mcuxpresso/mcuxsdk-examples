/*
 * Copyright 2019-2021 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_
#if defined(DEMO_MMCAU_PASS_RATE)
#include "fsl_rnga.h"
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define CORE_CLK_FREQ CLOCK_GetFreq(kCLOCK_CoreSysClk)
#if defined(DEMO_MMCAU_PASS_RATE)
#define BOARD_RandomInit() RNGA_Init(RNG)
#define GetRandomData32(x) RNGA_GetRandomData(RNG, x, 4U);
#endif /* DEMO_MMCAU_PASS_RATE */
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
