/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#if configGENERATE_RUN_TIME_STATS==1
/**
 * @brief Performance measurement helper, do not include in release builds
 *
 */
void vTaskClearUsage(void);
#endif