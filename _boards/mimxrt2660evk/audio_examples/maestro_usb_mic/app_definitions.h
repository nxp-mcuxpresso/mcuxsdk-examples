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

/*${macro:start}*/
/* Codec */
#define DEMO_CODEC_WM8962 1

/* SAI settings — RT2660 AUDIO__SAI_0 drives WM8962. */
#define DEMO_SAI              AUDIO__SAI_0
#define DEMO_SAI_CHANNEL      (0)
#define DEMO_SAI_BITWIDTH     (kSAI_WordWidth16bits)
#define DEMO_SAI_IRQ          AUDIO_SAI0_IRQn
#define DEMO_SAI_TX_SYNC_MODE kSAI_ModeAsync
#define DEMO_SAI_RX_SYNC_MODE kSAI_ModeSync
#define DEMO_SAI_MASTER_SLAVE kSAI_Master
/* Bit clock derived from the SAI MCLK divider (maestro_record aligned). */
#define DEMO_SAI_CLOCK_SOURCE (kSAI_BclkSourceMclkDiv)

#define DEMO_AUDIO_DATA_CHANNEL (2U)
#define DEMO_AUDIO_BIT_WIDTH    kSAI_WordWidth16bits
#define DEMO_AUDIO_SAMPLE_RATE  (kSAI_SampleRate16KHz)
#define DEMO_AUDIO_MASTER_CLOCK DEMO_SAI_CLK_FREQ

/* IRQ (used only for NVIC priority set). Codec I2C is LPI2C1 in HSP domain. */
#define DEMO_I2C_IRQ    HSP_LPI2C1_IRQn

/* EDMA on AUDIO__EDMA3 — RX only for the mic path. Channel mux via edma4. */
#define DEMO_DMA            AUDIO__EDMA3
#define DEMO_DMA_RX_CHANNEL (1U)
#define DEMO_DMA_RX_IRQ     AUDIO_EDMA3_CH1_IRQn
#define DEMO_SAI_RX_SOURCE  kDmaRequestMux2AudioSAI0Rx

/* Root clock: SAI0 MCLK0 derived from AUDIOPLL/2 (~24.576 MHz for 48 kHz
 * WM8962 MCLK; the USB-mic path uses 16 kHz sample rate. */
#define DEMO_SAI_CLK_FREQ CLOCK_GetRootClockFreq(kCLOCK_Root_AUDIO_sai0_mclk0)

#define DEMO_CHANNEL_NUM 2
/*${macro:end}*/

#endif /* _APP_DEFINITIONS_H_ */
