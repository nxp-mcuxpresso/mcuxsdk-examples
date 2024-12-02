/*
 * Copyright 2017-2020 NXP
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
#define DEMO_SAI         (I2S2)
#define DEMO_SAI_CHANNEL (0)
#define DEMO_SAI_CLK_FREQ                                                                  \
    CLOCK_GetPllFreq(kCLOCK_SystemPll1Ctrl) / (CLOCK_GetRootPreDivider(kCLOCK_RootSai2)) / \
        (CLOCK_GetRootPostDivider(kCLOCK_RootSai2)) / 6
#define DEMO_CODEC_WM8524       (1)
#define CODEC_USEGPIO           (1)
#define DEMO_CODEC_BUS_PIN      (NULL)
#define DEMO_CODEC_BUS_PIN_NUM  (0)
#define DEMO_CODEC_MUTE_PIN     (GPIO1)
#define DEMO_CODEC_MUTE_PIN_NUM (8)

#define DEMO_SAI_TX_SYNC_MODE kSAI_ModeAsync
#define DEMO_SAI_RX_SYNC_MODE kSAI_ModeSync
#define DEMO_SAI_MCLK_OUTPUT  true
#define DEMO_SAI_MASTER_SLAVE kSAI_Master

#define DEMO_AUDIO_DATA_CHANNEL (2U)
#define DEMO_AUDIO_BIT_WIDTH    kSAI_WordWidth16bits
#define DEMO_AUDIO_SAMPLE_RATE  (kSAI_SampleRate16KHz)
#define DEMO_AUDIO_MASTER_CLOCK DEMO_SAI_CLK_FREQ

#define BOARD_MASTER_CLOCK_CONFIG()
#define BOARD_SAI_RXCONFIG(config, mode)

#define DEMO_BOARD_CODEC_INIT BOARD_Codec_Init

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void BOARD_WM8524_Mute_GPIO(uint32_t output);
void BOARD_Codec_Init(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
