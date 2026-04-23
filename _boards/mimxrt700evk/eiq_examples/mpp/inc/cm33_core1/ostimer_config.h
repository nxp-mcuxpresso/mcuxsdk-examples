/*
 * Copyright 2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*
 * OS timer setup for supporting FreeRTOS runtime
 * task statistics
 */
#ifndef _OSTIMER_CONFIG_H_
#define _OSTIMER_CONFIG_H_

#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#if ((configGENERATE_RUN_TIME_STATS == 1) && defined(HAL_TIMER_PRECISION_1_US))

void vConfigureTimerForRunTimeStats(void);
unsigned long vGetTimerForRunTimeStats(void);

#endif /* ((configGENERATE_RUN_TIME_STATS == 1) && defined(HAL_TIMER_PRECISION_1_US)) */

#endif /* _OSTIMER_CONFIG_H_ */
