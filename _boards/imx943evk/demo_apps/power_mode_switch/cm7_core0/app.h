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
#define APP_SYSTICK_TIMER LPTMR1
#define APP_WAKEUP_TIMER LPTMR2
#define APP_UART LPUART11

#define APP_SYSTICK_TIMER_IRQ LPTMR1_IRQn
#define APP_WAKEUP_TIMER_IRQ LPTMR2_IRQn
#define APP_UART_IRQ LPUART11_IRQn

#define APP_UART_IRQHANDLER LPUART11_IRQHandler

#define APP_WAKEUP_TIMER_IRQ_PRIO     (5U)
#define APP_UART_IRQ_PRIO    (5U)

#define APP_SYSTICK_TIMER_HAL_CLOCK_ID (kCLOCK_Lptmr1)
#define APP_SYSTICK_TIMER_CLOCK_RATE CLOCK_GetRate(APP_SYSTICK_TIMER_HAL_CLOCK_ID)
#define APP_SYSTICK_TIMER_INSTANCE_IDX 1

#define APP_WAKEUP_TIMER_HAL_CLOCK_ID (kCLOCK_Lptmr2)
#define APP_WAKEUP_TIMER_CLOCK_RATE (32000U)
#define APP_WAKEUP_TIMER_INSTANCE_IDX 2

#define APP_CPU_PER_LPI_IDX_UART CPU_PER_LPI_IDX_LPUART11

#define APP_UART_PWR_MIX_SLICE_IDX POWER_MIX_SLICE_IDX_WAKEUP
#define APP_WAKEUP_TIMER_PWR_MIX_SLICE_IDX POWER_MIX_SLICE_IDX_WAKEUP

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
