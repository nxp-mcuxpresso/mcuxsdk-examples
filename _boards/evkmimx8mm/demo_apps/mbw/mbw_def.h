/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _MBW_DEF_H_
#define _MBW_DEF_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_GPT_BASE       GPT1
#define DEMO_GPT_CLOCK_NAME (kCLOCK_Gpt1)
#define DEMO_GPT_CLOCK_FREQ                                                                 \
    (CLOCK_GetPllFreq(kCLOCK_SystemPll1Ctrl) / (CLOCK_GetRootPreDivider(kCLOCK_RootGpt1)) / \
     (CLOCK_GetRootPostDivider(kCLOCK_RootGpt1)) / 2) /* SYSTEM PLL1 DIV2 */
#define DEMO_GPT_IRQ        (GPT1_IRQn)
#define DEMO_GPT_CLK_DIV    (3U)
#define DEMO_GPT_IRQHandler GPT1_IRQHandler
#endif /* _MBW_DEF_H_ */
