/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef __PDM_CONFIG_H__
#define __PDM_CONFIG_H__

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define DEMO_PDM                      PDM
#define DEMO_PDM_CLK_FREQ             CLOCK_GetMicfilClkFreq()
#define DEMO_PDM_FIFO_WATERMARK       (FSL_FEATURE_PDM_FIFO_DEPTH / 2)
#define DEMO_PDM_QUALITY_MODE         kPDM_QualityModeHigh
#define DEMO_PDM_CIC_OVERSAMPLE_RATE  (0U)
#define DEMO_PDM_ENABLE_CHANNEL       (DEMO_PDM_ENABLE_CHANNEL_RIGHT)
#define DEMO_PDM_ENABLE_CHANNEL_LEFT  (0U)
#define DEMO_PDM_ENABLE_CHANNEL_RIGHT (1U)
#define DEMO_PDM_CHANNEL_GAIN         kPDM_DfOutputGain4

#define DEMO_PDM_DMA          DMA0
#define DEMO_PDM_EDMA_CHANNEL 0
#define DEMO_PDM_EDMA_SOURCE  kDma0RequestMuxMicfil0FifoRequest

#define DEMO_AUDIO_SAMPLE_RATE 16000
/*${macro:end}*/
#endif /* __PDM_CONFIG_H__ */
