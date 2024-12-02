/*
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "fsl_wm8524.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define DEMO_SAI              (I2S2)
#define DEMO_SAI_MASTER_SLAVE kSAI_Master
#define DEMO_SAI_CLK_FREQ                                                                  \
    CLOCK_GetPllFreq(kCLOCK_SystemPll1Ctrl) / (CLOCK_GetRootPreDivider(kCLOCK_RootSai2)) / \
        (CLOCK_GetRootPostDivider(kCLOCK_RootSai2)) / 6
#define DEMO_CODEC_WM8524       (1)
#define CODEC_USEGPIO           (1)
#define DEMO_CODEC_BUS_PIN      (NULL)
#define DEMO_CODEC_BUS_PIN_NUM  (0)
#define DEMO_CODEC_MUTE_PIN     (GPIO1)
#define DEMO_CODEC_MUTE_PIN_NUM (8)
#define DEMO_IRQn               I2S2_IRQn
#define EXAMPLE_DMA             SDMAARM1
#define EXAMPLE_CHANNEL         (1)
#define SAI_TX_SOURCE           (11)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void BOARD_WM8524_Mute_GPIO(uint32_t output);
/*${prototype:end}*/

#endif /* _APP_H_ */
