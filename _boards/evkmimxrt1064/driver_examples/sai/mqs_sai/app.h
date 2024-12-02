/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "fsl_iomuxc.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
/* SAI instance and clock */
#define DEMO_SAI SAI3

#define HMCLK_FREQ CLOCK_GetFreq(kCLOCK_IpgClk)
#define MQS_IOMUXC IOMUXC_GPR

/* Select Audio/Video PLL (786.48 MHz) as sai1 clock source */
#define DEMO_SAI_CLOCK_SOURCE_SELECT (2U)
/* Clock pre divider for sai1 clock source */
#define DEMO_SAI_CLOCK_SOURCE_PRE_DIVIDER (3U)
/* Clock divider for sai1 clock source */
#define DEMO_SAI_CLOCK_SOURCE_DIVIDER (15U)
/* Get frequency of sai1 clock */
#define DEMO_SAI_CLK_FREQ                                                       \
    (CLOCK_GetFreq(kCLOCK_AudioPllClk) / (DEMO_SAI_CLOCK_SOURCE_DIVIDER + 1U) / \
     (DEMO_SAI_CLOCK_SOURCE_PRE_DIVIDER + 1U))

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
