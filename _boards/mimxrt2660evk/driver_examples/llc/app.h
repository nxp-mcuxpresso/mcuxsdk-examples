/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

#include "board.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
/* XSPI1 drives the 32 MB PSRAM used as the LLC test memory. Which part is
 * fitted depends on the board revision, and the two are not software
 * compatible: Proto-1B and later carry a Winbond W958D6NMYA4I (HyperBus,
 * USE_PSRAM_W958D6=1), the older socket board an AP Memory Xccela part
 * (APS256XXN/APS512XXN, USE_PSRAM_W958D6=0). Either way the boot ROM brings
 * XSPI1 up from the XMCD in the boot header, so this example does not depend
 * on the part. */
#define EXAMPLE_XSPI MAIN__XSPI_1

/* LLC instance that owns the external-memory test region on RT2660. */
#define BOARD_LLC_INSTANCE CMPT__LLC

/* Two independent HW paths to the SAME physical PSRAM cells (see board.c
 * BOARD_ConfigMPU / BOARD_EarlyConfigLLC): 0x8000_0000 bypasses the LLC,
 * 0x8800_0000 routes through the LLC. The LLC example uses only the cached
 * window so every CPU burst becomes an aligned LLC line fill. */
#define BOARD_LLC_DIRECT_BASE 0x80000000U
#define BOARD_LLC_CACHED_BASE 0x88000000U
#define BOARD_LLC_REGION_SIZE 0x02000000U /* 32 MB, both supported parts */
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
