/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
/* RT2660 has no top-level CCM/CoreSys API; map the benchmark's freq probe to
 * the CMPT CPU clock source (kCLOCK_SRC_CPU = CGU MAIN root after CPU divider).
 */
#define CLOCK_GetCoreSysClkFreq() CLOCK_GetClockSrcFreq(kCLOCK_SRC_CPU)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
