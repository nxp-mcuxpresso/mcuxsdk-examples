/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
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
#define APP_SKIP_LOW_BYTE_TEST 1
#ifdef ENABLE_RAM_VECTOR_TABLE
/* Put the IRQ handler in RAM to reduce latency. */
AT_QUICKACCESS_SECTION_CODE(void WDOG_IRQHandler(void));
#else
/* Map example IRQHandler to name of vector table */
#define WDOG_IRQHandler WDOG0_IRQHandler
#endif

/* RESET_CHECK_FLAG is a RAM variable
 * make sure this variable's location is proper that it will not be affected by watchdog reset
 */
#define RESET_CHECK_FLAG       (*((uint32_t *)0x20002000))
#define RESET_CHECK_INIT_VALUE 0x0D0D
#define EXAMPLE_WDOG_BASE      WDOG0
#define DELAY_TIME             1000000U

#define APP_WDOG_RESET_FLAG_SET() ((CMC_RTD->SRS & CMC_SRS_WDOG0_MASK) != 0U)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
