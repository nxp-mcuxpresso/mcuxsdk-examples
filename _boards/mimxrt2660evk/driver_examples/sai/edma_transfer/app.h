/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "fsl_wm8962.h"
#include "fsl_edma.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
/* SAI settings — RT2660 AUDIO__SAI_0 driving on-board WM8962 codec (U56). */
#define DEMO_CODEC_VOLUME       75
#define DEMO_SAI                AUDIO__SAI_0
#define DEMO_SAI_CHANNEL        (0)
#define DEMO_SAI_TX_SYNC_MODE   kSAI_ModeAsync
#define DEMO_SAI_MASTER_SLAVE   kSAI_Master
#define DEMO_AUDIO_DATA_CHANNEL (2U)
#define DEMO_AUDIO_BIT_WIDTH    kSAI_WordWidth16bits
#define DEMO_AUDIO_SAMPLE_RATE  (kSAI_SampleRate48KHz)

/* DMA — SAI0 TX via AUDIO__EDMA3 channel 0 with mux2 request source. */
#define DEMO_DMA                        AUDIO__EDMA3
#define DEMO_EDMA_CHANNEL               (0U)
#define DEMO_SAI_EDMA_CHANNEL           kDmaRequestMux2AudioSAI0Tx

#define DEMO_AUDIO_MASTER_CLOCK DEMO_SAI_CLK_FREQ

#define DEMO_SAI_CLK_FREQ       CLOCK_GetRootClockFreq(kCLOCK_Root_AUDIO_sai0_mclk0)
#define BOARD_MASTER_CLOCK_CONFIG() BOARD_MasterClockConfig()

#define BOARD_SAI_RXCONFIG(config, mode)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void BOARD_MasterClockConfig(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
