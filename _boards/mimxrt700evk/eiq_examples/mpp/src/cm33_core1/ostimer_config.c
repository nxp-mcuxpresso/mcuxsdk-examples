/*
 * Copyright 2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*
 * Utick timer setup for supporting FreeRTOS runtime
 * task statistics
 */

#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"

#if ((configGENERATE_RUN_TIME_STATS == 1) && defined(HAL_TIMER_PRECISION_1_US))

/* board includes */
#include "clock_config.h"
#include "fsl_ostimer.h"

#include "ostimer_config.h"

#define BOARD_OSTIMER OSTIMER_CPU1

void vConfigureTimerForRunTimeStats(void) {

	/*Added for OSTIMER */
	CLOCK_AttachClk(kLPOSC_to_OSTIMER);
	CLOCK_SetClkDiv(kCLOCK_DivOstimerClk, 1U);

	/* Intialize the OS timer, setting clock configuration. */
	OSTIMER_Init(BOARD_OSTIMER);
}

unsigned long vGetTimerForRunTimeStats()
{
	uint64_t us = OSTIMER_GetCurrentTimerValue(BOARD_OSTIMER);
	return us;
}

#endif /* ((configGENERATE_RUN_TIME_STATS == 1) && defined(HAL_TIMER_PRECISION_1_US)) */

