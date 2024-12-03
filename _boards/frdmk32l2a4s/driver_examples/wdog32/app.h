/*
 * Copyright 2019 NXP
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
/* RESET_CHECK_FLAG is a RAM variable used for wdog32 self test.
 * Make sure this variable's location is proper that it will not be affected by watchdog reset,
 * that is, the variable shall not be intialized in startup code.
 */
#define RESET_CHECK_FLAG       (*((uint32_t *)0x20002000))
#define RESET_CHECK_INIT_VALUE 0x0D0D
#define EXAMPLE_WDOG_BASE      WDOG0
#define DELAY_TIME             100000U
#define WDOG_IRQHandler        WDOG0_IRQHandler
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
