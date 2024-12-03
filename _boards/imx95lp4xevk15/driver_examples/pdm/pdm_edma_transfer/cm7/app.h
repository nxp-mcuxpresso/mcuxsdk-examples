/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define DEMO_EDMA              DMA3
#define DEMO_EDMA_CHANNEL      Dma3RequestMuxPDMRequest

#define DEMO_PDM                      PDM
#define PDM_CLOCK_ROOT                hal_clock_pdm
#define DEMO_PDM_CLK_FREQ             HAL_ClockGetIpFreq(PDM_CLOCK_ROOT)
#define DEMO_PDM_FIFO_WATERMARK       (FSL_FEATURE_PDM_FIFO_DEPTH / 2U)
#define DEMO_PDM_QUALITY_MODE         kPDM_QualityModeHigh
#define DEMO_PDM_CIC_OVERSAMPLE_RATE  (0U)
#define DEMO_PDM_ENABLE_CHANNEL_LEFT  (0U)
#define DEMO_PDM_ENABLE_CHANNEL_RIGHT (1U)

#define DEMO_PDM_ERROR_IRQn        PDM_ERROR_IRQn
#define DEMO_PDM_ERROR_IRQHandler  PDM_ERROR_IRQHandler

#define DEMO_AUDIO_SAMPLE_RATE (16000U)

#undef FSL_FEATURE_EDMA_HAS_CHANNEL_MUX
#undef FSL_FEATURE_EDMA_HAS_MP_CHANNEL_MUX
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
