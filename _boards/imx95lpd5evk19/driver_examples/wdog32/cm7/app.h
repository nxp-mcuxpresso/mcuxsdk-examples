/*
 * Copyright 2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "hal_clock.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define APP_SKIP_LOW_BYTE_TEST 1
#ifdef ENABLE_RAM_VECTOR_TABLE
/* Put the IRQ handler in RAM to reduce latency. */
AT_QUICKACCESS_SECTION_CODE(void WDOG_IRQHandler(void));
#else
/* Map example IRQHandler to name of vector table */
#define WDOG_IRQHandler WDOG5_IRQHandler
#endif

/* RESET_CHECK_FLAG is a RAM variable (SRC General Purpose Register 1)
 * make sure this variable's location is proper that it will not be affected by watchdog reset
 */
#define RESET_CHECK_FLAG       (*((uint32_t *)0x4446005c))
#define RESET_CHECK_INIT_VALUE 0x0D0D
#define EXAMPLE_WDOG_BASE      WDOG5
#define EXAMPLE_WDOG_IRQ       WDOG5_IRQn
#define EXAMPLE_WDOG_CLOCK     kCLOCK_Wdog5
#define DELAY_TIME             1000000U
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
