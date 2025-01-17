/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
/* RESET_CHECK_CNT_VALUE and RESET_CHECK_FLAG is RAM variables used for wdog32 self test.
 * Make sure these variables' locations are proper and will not be affected by watchdog reset,
 * that is, these variables will not be intialized in startup code.
 */
#define RESET_CHECK_CNT_VALUE  (*((uint32_t *)0x20001000))
#define RESET_CHECK_FLAG       (*((uint32_t *)0x20002000))
#define RESET_CHECK_INIT_VALUE 0x0D0DU
#define EXAMPLE_WDOG_BASE      RTWDOG3
#define DELAY_TIME             100000U
#define WDOG_IRQHandler        RTWDOG3_IRQHandler

/* When WDOG IRQ get enabled, the first WDOG IRQ is a very short pulse which can not be caught by
 * SRC so reset will not happen, after twice the TOVAL value, a backup reset will happen which is
 * a long pulse. SRC captured backup reset and do the SOC reset. WDOG CNT rolls back to 0, then 
 * grows to 1 before RESET happening. So code will not check whether CNT equals to timeout value.
 */
#define RTWDOG_HAS_ERRATA_051578 (1)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
