/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "fsl_wm8960.h"
#include "fsl_sai.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
/* SAI and I2C instance and clock */
#define DEMO_CODEC_WM8960
#define DEMO_SAI          SAI0
#define DEMO_SAI_CHANNEL  (0)
#define DEMO_I2C          LPI2C0
#define DEMO_SAI_CLK_FREQ CLOCK_GetIpFreq(kCLOCK_Sai0)
#define DEMO_I2C_CLK_FREQ CLOCK_GetIpFreq(kCLOCK_Lpi2c0)

#define DEMO_SAI_TX_SYNC_MODE kSAI_ModeSync
#define DEMO_SAI_RX_SYNC_MODE kSAI_ModeAsync
#define DEMO_SAI_MCLK_OUTPUT  true
#define DEMO_SAI_MASTER_SLAVE kSAI_Master

#define DEMO_AUDIO_DATA_CHANNEL (2U)
#define DEMO_AUDIO_BIT_WIDTH    kSAI_WordWidth16bits
#define DEMO_AUDIO_SAMPLE_RATE  (kSAI_SampleRate16KHz)
#define DEMO_AUDIO_MASTER_CLOCK (CLOCK_GetIpFreq(kCLOCK_Sai0))


#define DEMO_EDMA_CHANNEL     16U
#define DEMO_DMA              DMA0
#define DEMO_SAI_EDMA_CHANNEL kDmaRequestMux0SAI0Tx

#define BOARD_MASTER_CLOCK_CONFIG()
#define BOARD_SAI_RXCONFIG(config, mode) BOARD_SAI_RXConfig(config, mode);
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void BOARD_SAI_RXConfig(sai_transceiver_t *config, sai_sync_mode_t sync);
/*${prototype:end}*/

#endif /* _APP_H_ */
