/*
 * Copyright 2024-2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*
 * Utick timer setup for supporting FreeRTOS runtime
 * task statistics
 */
#ifndef _UTICK_CONFIG_H_
#define _UTICK_CONFIG_H_

#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#if ((configGENERATE_RUN_TIME_STATS == 1) && !defined(HAL_TIMER_PRECISION_1_US))

#define BOARD_UTICK UTICK0
/* number of us between each timer IRQ */
#define US_PER_TICK 100 /* 100 us is the max recommended for FreeRTOS (10xSpeed of systick) */

void vConfigureTimerForRunTimeStats(void);
unsigned long vGetTimerForRunTimeStats(void);

#endif /* (configGENERATE_RUN_TIME_STATS == 1) */

#endif /* _UTICK_CONFIG_H_ */
