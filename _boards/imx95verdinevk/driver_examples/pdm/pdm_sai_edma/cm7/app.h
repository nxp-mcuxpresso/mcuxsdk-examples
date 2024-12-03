/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

#include "fsl_wm8904.h"
#include "fsl_sai.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define DEMO_DMA               EDMA5_2
#define DEMO_PDM_EDMA_SOURCE   Dma5RequestMuxPDMRequest
#define DEMO_PDM_EDMA_CHANNEL  0U
#define DEMO_SAI_EDMA_SOURCE   Dma5RequestMuxSai3Tx
#define DEMO_SAI_EDMA_CHANNEL  16U
#define EXAMPLE_DMA_CLOCK_ROOT kCLOCK_Root_WakeupAxi

#define DEMO_PDM                      PDM
#define PDM_CLOCK_ROOT                hal_clock_pdm
#define DEMO_PDM_CLK_FREQ             HAL_ClockGetIpFreq(PDM_CLOCK_ROOT)
#define DEMO_PDM_FIFO_WATERMARK       (FSL_FEATURE_PDM_FIFO_DEPTH / 2U)
#define DEMO_PDM_QUALITY_MODE         kPDM_QualityModeHigh
#define DEMO_PDM_CIC_OVERSAMPLE_RATE  (0U)
#define DEMO_PDM_ENABLE_CHANNEL_LEFT  (0U)
#define DEMO_PDM_ENABLE_CHANNEL_RIGHT (1U)
#define DEMO_PDM_HWVAD_SIGNAL_GAIN    0
#define DEMO_PDM_CHANNEL_GAIN         kPDM_DfOutputGain0

#define DEMO_CODEC_WM8960
#define DEMO_SAI              SAI3
#define DEMO_SAI_CHANNEL      (0)
#define DEMO_SAI_IRQ          SAI3_IRQn
#define DEMO_SAITxIRQHandler  SAI3_IRQHandler
#define DEMO_SAI_TX_SYNC_MODE kSAI_ModeAsync
#define DEMO_SAI_RX_SYNC_MODE kSAI_ModeSync
#define DEMO_SAI_MASTER_SLAVE kSAI_Master
#define DEMO_SAI_CLOCK_SOURCE (kSAI_BclkSourceMclkDiv)

#define DEMO_AUDIO_DATA_CHANNEL (2U)
#define DEMO_AUDIO_BIT_WIDTH    kSAI_WordWidth16bits
#define DEMO_AUDIO_SAMPLE_RATE  (kSAI_SampleRate16KHz)
#define DEMO_AUDIO_MASTER_CLOCK DEMO_SAI_CLK_FREQ

/* Get frequency of sai1 clock */
#define SAI_CLOCK_ROOT    hal_clock_sai3
#define DEMO_SAI_CLK_FREQ HAL_ClockGetIpFreq(SAI_CLOCK_ROOT)

/* I2C instance and clock */
#define DEMO_I2C LPI2C1

/* Get frequency of lpi2c clock */
#define LPI2C_MASTER_CLOCK_ROOT hal_clock_lpi2c4
#define DEMO_I2C_CLK_FREQ       HAL_ClockGetIpFreq(LPI2C_MASTER_CLOCK_ROOT)
#define BOARD_MasterClockConfig()
#define BOARD_SAI_RXCONFIG(config, mode)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
