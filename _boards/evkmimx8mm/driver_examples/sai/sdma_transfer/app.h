/*
 * Copyright 2018 NXP
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
#define DEMO_SAI              (I2S3)
#define DEMO_SAI_MASTER_SLAVE kSAI_Master
#define DEMO_SAI_CLK_FREQ                                                                  \
    (CLOCK_GetPllFreq(kCLOCK_AudioPll1Ctrl) / (CLOCK_GetRootPreDivider(kCLOCK_RootSai3)) / \
     (CLOCK_GetRootPostDivider(kCLOCK_RootSai3)))
#define DEMO_CODEC_WM8524       (1)
#define DEMO_CODEC_BUS_PIN      (NULL)
#define DEMO_CODEC_BUS_PIN_NUM  (0)
#define DEMO_CODEC_MUTE_PIN     (GPIO5)
#define DEMO_CODEC_MUTE_PIN_NUM (21)
#define DEMO_IRQn               I2S3_IRQn
#define EXAMPLE_DMA             SDMAARM3
#define EXAMPLE_CHANNEL         (1)
#define SAI_TX_SOURCE           (5)
/*set Bclk source to Mclk clock*/
#define DEMO_SAI_CLOCK_SOURCE   (1U)
#define SDMA_FREQ_EQUALS_ARM    (1U)
#define DEMO_AUDIO_MASTER_CLOCK DEMO_SAI_CLK_FREQ

#define BOARD_SAI_RXCONFIG(config, mode)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void BOARD_WM8524_Mute_GPIO(uint32_t output);
void BOARD_MASTER_CLOCK_CONFIG(void);

/*${prototype:end}*/

#endif /* _APP_H_ */
