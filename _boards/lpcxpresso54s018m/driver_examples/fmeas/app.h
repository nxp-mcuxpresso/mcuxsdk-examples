/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
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

#define EXAMPLE_REFERENCE_CLOCK (kINPUTMUX_MainClkToFreqmeas)
#define EXAMPLE_TARGET_CLOCK_1  (kINPUTMUX_WdtOscToFreqmeas)
#define EXAMPLE_TARGET_CLOCK_2  (kINPUTMUX_32KhzOscToFreqmeas)
#define EXAMPLE_TARGET_CLOCK_3  (kINPUTMUX_Fro12MhzToFreqmeas)

#define EXAMPLE_REFERENCE_CLOCK_NAME "main clock"
#define EXAMPLE_TARGET_CLOCK_1_NAME  "Watchdog oscillator"
#define EXAMPLE_TARGET_CLOCK_2_NAME  "RTC32K oscillator"
#define EXAMPLE_TARGET_CLOCK_3_NAME  "FRO oscillator"

#define EXAMPLE_REFERENCE_CLOCK_FREQUENCY (CLOCK_GetCoreSysClkFreq())
#define EXAMPLE_TARGET_CLOCK_1_FREQUENCY  (CLOCK_GetWdtOscFreq())
#define EXAMPLE_TARGET_CLOCK_2_FREQUENCY  (CLOCK_GetOsc32KFreq())
#define EXAMPLE_TARGET_CLOCK_3_FREQUENCY  (CLOCK_GetFro12MFreq())

#define EXAMPLE_REFERENCE_CLOCK_REGISTRY_INDEX (0U)
#define EXAMPLE_TARGET_CLOCK_REGISTRY_INDEX    (1U)

#if defined(FSL_FEATURE_FMEAS_USE_ASYNC_SYSCON) && (FSL_FEATURE_FMEAS_USE_ASYNC_SYSCON)
#define FMEAS_SYSCON ASYNC_SYSCON
#else
#define FMEAS_SYSCON SYSCON
#endif

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
