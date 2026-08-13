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
#ifndef DEMO_CODEC_WM8962
#define DEMO_CODEC_WM8962 1
#endif
#ifndef DEMO_CODEC_CS42448
#define DEMO_CODEC_CS42448 0
#endif
#if DEMO_CODEC_WM8962 && DEMO_CODEC_CS42448
#error "Duplicate codec defined"
#endif

#define DEMO_SAI                AUDIO__SAI_0
#define DEMO_SAI_CHANNEL        (0)
#define DEMO_SAI_IRQ            AUDIO_SAI0_IRQn
#define DEMO_SAITxIRQHandler    AUDIO_SAI0_IRQHandler
#define DEMO_SAI_MASTER_SLAVE   kSAI_Master

#define DEMO_AUDIO_DATA_CHANNEL (2U)
#define DEMO_AUDIO_BIT_WIDTH    kSAI_WordWidth16bits
#define DEMO_AUDIO_SAMPLE_RATE  (kSAI_SampleRate48KHz)
#define DEMO_AUDIO_MASTER_CLOCK DEMO_SAI_CLK_FREQ

#define DEMO_SAI_TX_IRQ         AUDIO_SAI0_IRQn
#define DEMO_SAI_RX_IRQ         AUDIO_SAI0_IRQn

/* DMA — SAI0 TX/RX via AUDIO__EDMA3 channels 0/1 with mux2 request sources. */
#define DEMO_DMA                        AUDIO__EDMA3
#define DEMO_EDMA_CHANNEL               (0U)
#define DEMO_TX_EDMA_CHANNEL            (0U)
#define DEMO_RX_EDMA_CHANNEL            (1U)
#define DEMO_SAI_TX_EDMA_CHANNEL        kDmaRequestMux2AudioSAI0Tx
#define DEMO_SAI_RX_EDMA_CHANNEL        kDmaRequestMux2AudioSAI0Rx

#define DEMO_SAI_TX_SYNC_MODE          kSAI_ModeAsync
#define DEMO_SAI_RX_SYNC_MODE          kSAI_ModeSync
#define DEMO_WM8962_I2C_INSTANCE       BOARD_CODEC_I2C_INSTANCE

#define DEMO_SAI_CLK_FREQ  CLOCK_GetRootClockFreq(kCLOCK_Root_AUDIO_sai0_mclk0)

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
