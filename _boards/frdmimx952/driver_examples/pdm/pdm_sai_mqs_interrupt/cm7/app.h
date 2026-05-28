/*
 * Copyright 2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

#include "fsl_sai.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define DEMO_PDM                      PDM
#define PDM_CLOCK_ROOT                kCLOCK_pdm
#define DEMO_PDM_CLK_FREQ             CLOCK_GetRate(PDM_CLOCK_ROOT)
#define DEMO_PDM_FIFO_WATERMARK       (FSL_FEATURE_PDM_FIFO_DEPTH / 2U - 1U)
#define DEMO_PDM_QUALITY_MODE         kPDM_QualityModeHigh
#define DEMO_PDM_CIC_OVERSAMPLE_RATE  (16U)
#define DEMO_PDM_ENABLE_CHANNEL_LEFT  (0U)
#define DEMO_PDM_ENABLE_CHANNEL_RIGHT (1U)
#define DEMO_PDM_HWVAD_SIGNAL_GAIN    0
#define DEMO_PDM_CHANNEL_GAIN         kPDM_DfOutputGain0

#define DEMO_SAI              SAI1
#define DEMO_SAI_CHANNEL      (0)
#define DEMO_SAI_IRQ          SAI1_IRQn
#define DEMO_SAITxIRQHandler  SAI1_IRQHandler
#define DEMO_SAI_TX_SYNC_MODE kSAI_ModeAsync
#define DEMO_SAI_RX_SYNC_MODE kSAI_ModeSync
#define DEMO_SAI_MASTER_SLAVE kSAI_Master
#define DEMO_SAI_CLOCK_SOURCE (kSAI_BclkSourceMclkDiv)

#define DEMO_AUDIO_DATA_CHANNEL (2U)
#define DEMO_AUDIO_BIT_WIDTH    kSAI_WordWidth16bits
#define DEMO_AUDIO_SAMPLE_RATE  (kSAI_SampleRate16KHz)
#define DEMO_AUDIO_MASTER_CLOCK DEMO_SAI_CLK_FREQ

/* MQS output */
#define DEMO_BOARD_MQS_INIT BOARD_MQS_Init

/* Get frequency of sai1 clock */
#define SAI_CLOCK_ROOT    kCLOCK_sai1
#define DEMO_SAI_CLK_FREQ CLOCK_GetRate(SAI_CLOCK_ROOT)
#define HMCLK_FREQ        DEMO_SAI_CLK_FREQ

#define BOARD_MasterClockConfig()
#define BOARD_SAI_RXCONFIG(config, mode)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void BOARD_MQS_Init(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
