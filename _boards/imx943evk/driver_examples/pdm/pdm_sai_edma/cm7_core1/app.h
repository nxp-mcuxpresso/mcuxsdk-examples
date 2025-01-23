/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

#include "fsl_wm8962.h"
#include "fsl_sai.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define DEMO_DMA               EDMA1
/* The mapping of channel number and dma request signals for edma1 is 1:1 */
#define DEMO_PDM_EDMA_SOURCE   kDma1RequestMuxPdm
#define DEMO_PDM_EDMA_CHANNEL  kDma1RequestMuxPdm

#define DEMO_SAI_EDMA_SOURCE   kDma1RequestMuxSai1Tx
#define DEMO_SAI_EDMA_CHANNEL  kDma1RequestMuxSai1Tx
#define EXAMPLE_DMA_CLOCK_ROOT hal_clock_m33

#define DEMO_PDM                      PDM
#define PDM_CLOCK_ROOT                hal_clock_pdm
#define DEMO_PDM_CLK_FREQ             HAL_ClockGetRate(PDM_CLOCK_ROOT)
#define DEMO_PDM_FIFO_WATERMARK       (FSL_FEATURE_PDM_FIFO_DEPTH / 2U)
#define DEMO_PDM_QUALITY_MODE         kPDM_QualityModeHigh
#define DEMO_PDM_CIC_OVERSAMPLE_RATE  (0U)
#define DEMO_PDM_ENABLE_CHANNEL_LEFT  (0U)
#define DEMO_PDM_ENABLE_CHANNEL_RIGHT (1U)
#define DEMO_PDM_HWVAD_SIGNAL_GAIN    0
#define DEMO_PDM_CHANNEL_GAIN         kPDM_DfOutputGain0

#define DEMO_CODEC_WM8962
#define DEMO_SAI              SAI1
#define DEMO_SAI_CHANNEL      (0)
#define DEMO_SAI_IRQ          SAI1_IRQn
#define DEMO_SAITxIRQHandler  SAI1_IRQHandler
#define DEMO_SAI_TX_SYNC_MODE kSAI_ModeAsync
#define DEMO_SAI_RX_SYNC_MODE kSAI_ModeSync
#define DEMO_SAI_MASTER_SLAVE kSAI_Master
#define DEMO_SAI_CLOCK_SOURCE (kSAI_BclkSourceMclkDiv)

#define DEMO_AUDIO_DATA_CHANNEL (2U)
#define DEMO_AUDIO_BIT_WIDTH    kSAI_WordWidth32bits
#define DEMO_AUDIO_SAMPLE_RATE  (kSAI_SampleRate16KHz)
#define DEMO_AUDIO_MASTER_CLOCK DEMO_SAI_CLK_FREQ

/* Get frequency of sai1 clock */
#define SAI_CLOCK_ROOT    hal_clock_sai1
#define DEMO_SAI_CLK_FREQ HAL_ClockGetRate(SAI_CLOCK_ROOT)

/* I2C instance and clock */
#define DEMO_I2C LPI2C3

/* Get frequency of lpi2c clock */
#define LPI2C_MASTER_CLOCK_ROOT hal_clock_lpi2c3
#define DEMO_I2C_CLK_FREQ       HAL_ClockGetRate(LPI2C_MASTER_CLOCK_ROOT)
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
