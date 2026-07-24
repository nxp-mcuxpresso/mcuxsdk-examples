/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

#include "fsl_common.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*${macro:start}*/
/* RT2660 has 3 SRAMCTL instances. This example targets SRAM bank0 via SRAMCTL0. */
#define SRAMCTL_BASE CMPT__SRAMCTL_0

/* IRQ for SRAMCTL0 on RT2660. */
#define SRAMCTL_IRQ_ID      CMPT_SRAMCTL_0_IRQn
#define SRAMCTL_IRQ_HANDLER CMPT_SRAMCTL_0_IRQHandler

/*
 * RT2660 RM Table 55: A single-bit correctable error (SGLERR) generates
 * NO interrupt request and is NOT sent to the external fault receiver.
 * Only multi-bit uncorrectable errors (MLTERR/AERR) produce an interrupt.
 * Therefore the single-bit example must use the polling-timeout path.
 */
#define SRAMCTL_USE_INTERRUPT (0)

/* SRAM bank0 address range (256KB): 0x2200_0000 ~ 0x2203_FFFF */
#define APP_SRAM_BANK0_BASE (0x22000000u)
#define APP_SRAM_BANK0_END  (0x2203FFFFu)

/*
 * SRAMCTL test address (must be within SRAM bank0 range above).
 */
#define APP_SRAMCTL_TEST_REGION_BASE (0x2203F000u)
#define APP_SRAMCTL_TEST_REGION_END  (0x2203FFFFu)
#define APP_SRAMCTL_TEST_ADDR        (0x2203FF00u) /* 16-byte aligned */

/* Use a small init range that covers the test address. RAMIAE is inclusive. */
#define APP_SRAMCTL_INIT_ADDR_START  (APP_SRAMCTL_TEST_ADDR)
#define APP_SRAMCTL_INIT_ADDR_END    (APP_SRAMCTL_TEST_ADDR + 0x0Fu)

/* This is the single-bit example, multi-bit test is not used here. */
#define SRAMCTL_ENABLE_MULTIBIT_TEST (0)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

void BOARD_InitHardware(void);

#endif /* _APP_H_ */
