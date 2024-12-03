/*
 * Copyright 2024 NXP
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
#define APP_SKIP_LOW_BYTE_TEST 0

#define RESET_CHECK_FLAG       (*((uint32_t *)0x20014000))
#define RESET_CHECK_INIT_VALUE 0x0D0D
#define EXAMPLE_WDOG_BASE      WDOG0
#define DELAY_TIME             100000U
#define WDOG_IRQHandler        WDOG0_IRQHandler
#define EXAMPLE_CMC_BASE       CMC0
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
