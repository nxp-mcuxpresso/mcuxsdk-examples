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
#include "fsl_sai.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define DEMO_DMA              DMA0
#define DEMO_PDM_EDMA_SOURCE  kDmaRequestMux0MICFIL
#define DEMO_PDM_EDMA_CHANNEL 0U
#define DEMO_SAI_EDMA_SOURCE  kDmaRequestMux0SAI0Tx
#define DEMO_SAI_EDMA_CHANNEL 1U

#define DEMO_PDM                      PDM
#define DEMO_PDM_CLK_FREQ             CLOCK_GetMicfilFreq()
#define DEMO_PDM_FIFO_WATERMARK       (FSL_FEATURE_PDM_FIFO_DEPTH / 2U)
#define DEMO_PDM_QUALITY_MODE         kPDM_QualityModeHigh
#define DEMO_PDM_DFOUTPUT_GAIN        kPDM_DfOutputGain0
#define DEMO_PDM_CIC_OVERSAMPLE_RATE  (0U)
#define DEMO_PDM_ENABLE_CHANNEL_LEFT  (0U)
#define DEMO_PDM_ENABLE_CHANNEL_RIGHT (1U)
#define DEMO_PDM_HWVAD_SIGNAL_GAIN    0

#define DEMO_SAI              SAI0
#define DEMO_SAI_CHANNEL      (0)
#define DEMO_SAI_MASTER_SLAVE kSAI_Master
#define DEMO_SAI_CLOCK_SOURCE (kSAI_BclkSourceMclkDiv)
#define DEMO_SAI_TX_SYNC_MODE kSAI_ModeSync
#define DEMO_SAI_RX_SYNC_MODE kSAI_ModeAsync

#define DEMO_AUDIO_DATA_CHANNEL (2U)
#define DEMO_AUDIO_BIT_WIDTH    kSAI_WordWidth32bits
#define DEMO_AUDIO_SAMPLE_RATE  (kSAI_SampleRate16KHz)
#define DEMO_AUDIO_MASTER_CLOCK CLOCK_GetIpFreq(kCLOCK_Sai0)

#define BOARD_MasterClockConfig()
#define BOARD_SAI_RXCONFIG(config, mode) BOARD_SAI_RxConfig(config, mode);

#define BOARD_XTAL0_CLK_HZ (24000000U)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void BOARD_SAI_RxConfig(sai_transceiver_t *config, sai_sync_mode_t sync);
/*${prototype:end}*/

#endif /* _APP_H_ */
