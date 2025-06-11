/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
/* Get source clock for LPIT driver */
#define DEMO_LPUART          LPUART8
#define DEMO_LPUART_INSTANCE_IDX (8U)
#define LPUART_CLOCK_ROOT    kCLOCK_Lpuart8
#define DEMO_LPUART_CLK_FREQ CLOCK_GetRate(LPUART_CLOCK_ROOT)
/*${macro:end}*/

/* define instance */
#define DEMO_LPIT_BASE       LPIT1
#define DEMO_LPIT_IRQn       LPIT1_IRQn
#define DEMO_LPIT_IRQHandler LPIT1_IRQHandler
/* Get source clock for LPIT driver */
/* LPIT1 is clocked by AON bus */
#define LPIT_MASTER_CLOCK_ROOT kCLOCK_Busaon
#define LPIT_SOURCECLOCK       CLOCK_GetRate(LPIT_MASTER_CLOCK_ROOT)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
