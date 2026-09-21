/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_DEFINITIONS_H_
#define _APP_DEFINITIONS_H_

/*${header:start}*/
#include "fsl_wm8962.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/

/* Codec selection: RT2660 EVK is populated with WM8962 (U56) on the AUDIO
 * subsystem. Keep the same DEMO_CODEC_WM8962 flag the shared streamer_pcm.c
 * consumes so the codec-init hooks are compiled in. */
#define DEMO_CODEC_WM8962 1

/* SAI settings — RT2660 AUDIO__SAI_0 drives the WM8962 codec on the EVK. */
#define DEMO_SAI              AUDIO__SAI_0
#define DEMO_SAI_CHANNEL      (0)
#define DEMO_SAI_BITWIDTH     (kSAI_WordWidth16bits)
#define DEMO_SAI_IRQ          AUDIO_SAI0_IRQn
#define DEMO_SAI_IRQHANDLER   AUDIO_SAI0_IRQHandler
#define DEMO_SAI_TX_SYNC_MODE kSAI_ModeAsync
#define DEMO_SAI_MASTER_SLAVE kSAI_Master

#define DEMO_AUDIO_DATA_CHANNEL (2U)
#define DEMO_AUDIO_BIT_WIDTH    kSAI_WordWidth16bits
#define DEMO_AUDIO_SAMPLE_RATE  (kSAI_SampleRate48KHz)
#define DEMO_AUDIO_MASTER_CLOCK DEMO_SAI_CLK_FREQ

/* AUDIO__EDMA3 channel 0 fed by mux2 SAI0-TX request line. */
#define DEMO_DMA                  AUDIO__EDMA3
#define DEMO_TX_EDMA_CHANNEL      (0U)
#define DEMO_SAI_TX_EDMA_CHANNEL  kDmaRequestMux2AudioSAI0Tx

/* Master clock is derived from the AUDIO PLL (49.152 MHz) with divider 2. */
#define DEMO_SAI_CLK_FREQ CLOCK_GetRootClockFreq(kCLOCK_Root_AUDIO_sai0_mclk0)

#define DEMO_CHANNEL_NUM 2

/*${macro:end}*/

#endif /* _APP_DEFINITIONS_H_ */
