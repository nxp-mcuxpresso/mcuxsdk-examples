/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * RT2660 board-local macros for maestro_record.
 *
 * Playback path -- AUDIO__SAI_0 on AUDIO__EDMA3 mux2 SAI0-TX request.
 * Capture path  -- AUDIO__MICFIL on AUDIO__EDMA3 mux2 MIC (PDM) request.
 */
#ifndef _APP_DEFINITIONS_H_
#define _APP_DEFINITIONS_H_

/*${header:start}*/
#include "fsl_wm8962.h"
/*${header:end}*/

/*${macro:start}*/

/* Codec */
#define DEMO_CODEC_WM8962 1

/* SAI (playback) */
#define DEMO_SAI              AUDIO__SAI_0
#define DEMO_SAI_CHANNEL      (0)
#define DEMO_SAI_BITWIDTH     (kSAI_WordWidth32bits)
#define DEMO_SAI_IRQ          AUDIO_SAI0_IRQn
#define DEMO_SAI_IRQHANDLER   AUDIO_SAI0_IRQHandler
#define DEMO_SAI_TX_SYNC_MODE kSAI_ModeAsync
#define DEMO_SAI_MASTER_SLAVE kSAI_Master
#define DEMO_SAI_CLOCK_SOURCE (kSAI_BclkSourceMclkDiv)
#define DEMO_SAI_CLK_FREQ     CLOCK_GetRootClockFreq(kCLOCK_Root_AUDIO_sai0_mclk0)

/* Audio format (record path -- 16 kHz 32-bit stereo). */
#define DEMO_AUDIO_DATA_CHANNEL (2U)
#define DEMO_AUDIO_BIT_WIDTH    kSAI_WordWidth32bits
#define DEMO_AUDIO_SAMPLE_RATE  (kSAI_SampleRate16KHz)
#define DEMO_AUDIO_MASTER_CLOCK DEMO_SAI_CLK_FREQ

/* EDMA */
#define DEMO_EDMA                 AUDIO__EDMA3
#define DEMO_TX_CHANNEL           (0U)
#define DEMO_RX_CHANNEL           (1U)
#define DEMO_SAI_TX_SOURCE        kDmaRequestMux2AudioSAI0Tx
#define DEMO_SAI_RX_SOURCE        kDmaRequestMux2AudioMic

/* Legacy names re-used inside record streamer_pcm.c (adapted from RT1152). */
#define DEMO_DMA                  DEMO_EDMA
#define DEMO_TX_EDMA_CHANNEL      DEMO_TX_CHANNEL
#define DEMO_SAI_TX_EDMA_CHANNEL  DEMO_SAI_TX_SOURCE

/* PDM (MICFIL) */
#define DEMO_PDM                      AUDIO__MICFIL
#define DEMO_PDM_ERROR_IRQn           AUDIO_PDM_ERROR_IRQn
#define DEMO_PDM_EVENT_IRQn           AUDIO_PDM_EVENT_IRQn
#define DEMO_PDM_CLK_FREQ             24576000U
#define DEMO_PDM_FIFO_WATERMARK       (FSL_FEATURE_PDM_FIFO_DEPTH - 1U)
#define DEMO_PDM_QUALITY_MODE         kPDM_QualityModeHigh
#define DEMO_PDM_CIC_OVERSAMPLE_RATE  (0U)
#define DEMO_PDM_ENABLE_CHANNEL_LEFT  (0U)
#define DEMO_PDM_ENABLE_CHANNEL_RIGHT (1U)
#define DEMO_PDM_SAMPLE_CLOCK_RATE    (2048000U)

#define DEMO_MIC_CHANNEL_NUM 2
#define DEMO_MIC_SAMPLE_SIZE 32
#define DEMO_MIC_SAMPLE_RATE 16000
#define DEMO_MIC_FRAME_SIZE  30
#define DEMO_CODEC_CHANNEL   (kCODEC_PlayChannelHeadphoneLeft | kCODEC_PlayChannelHeadphoneRight)

#define DEMO_VOLUME (60)
#define DEMO_CHANNEL_NUM 2

/*${macro:end}*/

#endif /* _APP_DEFINITIONS_H_ */
