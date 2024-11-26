/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

#include "sm_platform.h"
#include "fsl_cpu.h"
#include "fsl_power.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define APP_SYSTICK_TIMER TPM2
#define APP_WAKEUP_TIMER TPM3
#define APP_UART LPUART12

#define APP_SYSTICK_TIMER_IRQ TPM2_IRQn
#define APP_WAKEUP_TIMER_IRQ TPM3_IRQn
#define APP_UART_IRQ LPUART12_IRQn

#define APP_WAKEUP_TIMER_IRQ_PRIO     (5U)
#define APP_UART_IRQ_PRIO    (5U)

#define APP_SYSTICK_TIMER_HAL_CLOCK_ID (hal_clock_tpm2)
#define APP_SYSTICK_TIMER_CLOCK_RATE HAL_ClockGetRate(APP_SYSTICK_TIMER_HAL_CLOCK_ID)
#define APP_SYSTICK_TIMER_INSTANCE_IDX 2

#define APP_WAKEUP_TIMER_HAL_CLOCK_ID (hal_clock_buswakeup)
#define APP_WAKEUP_TIMER_CLOCK_RATE HAL_ClockGetRate(APP_WAKEUP_TIMER_HAL_CLOCK_ID)
#define APP_WAKEUP_TIMER_INSTANCE_IDX 3

#define APP_CPU_PER_LPI_IDX_UART CPU_PER_LPI_IDX_LPUART12

#define APP_UART_PWR_MIX_SLICE_IDX PWR_MIX_SLICE_IDX_WAKEUP
#define APP_WAKEUP_TIMER_PWR_MIX_SLICE_IDX PWR_MIX_SLICE_IDX_WAKEUP

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
