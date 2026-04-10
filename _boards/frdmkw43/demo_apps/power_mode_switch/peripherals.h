/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _PERIPHERALS_H_
#define _PERIPHERALS_H_

#include "fsl_common.h"
#include "fsl_lptmr.h"
#include "fsl_wuu.h"

#if defined(__cplusplus)
extern "C" {
#endif

#define DEMO_WUU_PERIPHERAL WUU0
#define DEMO_WUU_IRQN       WUU0_IRQn
#define DEMO_WUU_IRQHANDLER WUU0_IRQHandler

#define DEMO_LPTMR_PERIPHERAL  LPTMR0
#define DEMO_LPTMR_CLK_FREQ    32768
#define DEMO_LPTMR_USEC_COUNT  1000000UL
#define DEMO_LPTMR_TICKS       DEMO_LPTMR_CLK_FREQ
#define DEMO_LPTMR_IRQN        LPTMR0_IRQn
#define DEMO_LPTMR_IRQHANDLER  LPTMR0_IRQHandler

extern const lptmr_config_t DEMO_LPTMR_config;

void BOARD_InitPeripherals(void);
void BOARD_InitBootPeripherals(void);

#if defined(__cplusplus)
}
#endif

#endif /* _PERIPHERALS_H_ */
