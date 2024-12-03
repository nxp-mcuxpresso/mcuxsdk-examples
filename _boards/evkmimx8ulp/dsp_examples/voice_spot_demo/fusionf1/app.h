/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
//#include "dsp_config.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define USE_PDM_EDMA (1)
#define DEMO_DMA              DMA0
#define DEMO_PDM_EDMA_SOURCE  kDmaRequestMux0MICFIL
#define DEMO_PDM_EDMA_CHANNEL 0U

#define DEMO_PDM                      PDM
#define DEMO_PDM_CLK_FREQ             CLOCK_GetMicfilFreq()
#define DEMO_PDM_FIFO_WATERMARK       (FSL_FEATURE_PDM_FIFO_DEPTH / 2U)
#define DEMO_PDM_QUALITY_MODE         kPDM_QualityModeHigh
#define DEMO_PDM_DFOUTPUT_GAIN        kPDM_DfOutputGain0
#define DEMO_PDM_CIC_OVERSAMPLE_RATE  (0U)
#define DEMO_PDM_ENABLE_CHANNEL_LEFT  (0U)
#define DEMO_PDM_ENABLE_CHANNEL_RIGHT (1U)
#define DEMO_PDM_HWVAD_SIGNAL_GAIN    0

#define DEMO_AUDIO_SAMPLE_RATE (16000U)

#define BOARD_XTAL0_CLK_HZ (24000000U)

/* DMIC configuration */
#define BYTE_DEPTH_DMIC (4U)

/* VoiceSpot configuration */
#define DEVICE_ID Device_IMX8ULP_FUSIONF1

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
