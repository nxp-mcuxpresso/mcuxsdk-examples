/*
 * Copyright 2026 NXP
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

#define RESET_CHECK_FLAG              (*((uint32_t *)0x20016000))
#define RESET_CHECK_INIT_VALUE        0x0D0D
#define EXAMPLE_WDOG_BASE             WDOG_0
#define DELAY_TIME                    100000U
#define WDOG_IRQHandler               WDOG0_IRQHandler
#define EXAMPLE_CMC_BASE              CMC0
#define EXAMPLE_CMC_WDOG_RESET_SOURCE kCMC_Watchdog0Reset
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
