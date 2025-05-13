/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

#include "fsl_cpu.h"
#include "fsl_power.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define APP_SYSTICK_TIMER TPM5
#define APP_WAKEUP_TIMER TPM6
#define APP_UART LPUART8

#define APP_UART_IRQHANDLER LPUART8_IRQHandler

#define APP_SYSTICK_TIMER_IRQ TPM5_IRQn
#define APP_WAKEUP_TIMER_IRQ TPM6_IRQn
#define APP_UART_IRQ LPUART8_IRQn

#define APP_WAKEUP_TIMER_IRQ_PRIO     (5U)
#define APP_UART_IRQ_PRIO    (5U)

#define APP_SYSTICK_TIMER_HAL_CLOCK_ID (kCLOCK_Tpm5)
#define APP_SYSTICK_TIMER_CLOCK_RATE CLOCK_GetRate(APP_SYSTICK_TIMER_HAL_CLOCK_ID)
#define APP_SYSTICK_TIMER_INSTANCE_IDX 5

#define APP_WAKEUP_TIMER_HAL_CLOCK_ID (kCLOCK_Tpm6)
#define APP_WAKEUP_TIMER_CLOCK_RATE CLOCK_GetRate(APP_WAKEUP_TIMER_HAL_CLOCK_ID)
#define APP_WAKEUP_TIMER_INSTANCE_IDX 6

#define APP_CPU_PER_LPI_IDX_UART CPU_PER_LPI_IDX_LPUART8

#define APP_WAKEUP_TIMER_PWR_MIX_SLICE_IDX POWER_MIX_SLICE_IDX_WAKEUP
#define APP_UART_PWR_MIX_SLICE_IDX POWER_MIX_SLICE_IDX_WAKEUP

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
