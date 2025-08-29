/*
 * Copyright 2025 NXP
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
#define DEMO_SAI                I2S0
#define DEMO_SAI_CHANNEL        1
#define DEMO_AUDIO_MASTER_CLOCK 12288000U
#define DEMO_SAI_TX_SYNC_MODE   kSAI_ModeAsync
#define DEMO_SAI_RX_SYNC_MODE   kSAI_ModeSync
#define DEMO_SAI_MASTER_SLAVE   kSAI_Slave
#define DEMO_DMA                DMA0
#define DEMO_EDMA_CHANNEL       0
#define DEMO_DMAMUX             DMAMUX


#define DEMO_AUDIO_DATA_CHANNEL (2U)
#define DEMO_AUDIO_BIT_WIDTH    kSAI_WordWidth16bits
#define DEMO_AUDIO_SAMPLE_RATE  (kSAI_SampleRate48KHz)

#define DEMO_CODEC_I2C_INSTANCE (1U)

#define BOARD_SAI_RXCONFIG(config, mode)
#define BOARD_MASTER_CLOCK_CONFIG()

#define DEMO_CODEC_VOLUME 100U
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
