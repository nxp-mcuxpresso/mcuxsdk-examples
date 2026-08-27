/*
 * Copyright 2022, 2024, 2026NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "FreeRTOS.h"

void vApplicationIdleHook(void)
{
	/* No more activity */
	__WFI();
}
