/*
 * Copyright 2018 NXP
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
#define DEMO_PDM                      PDM
#define DEMO_PDM_CLK_FREQ             (24576000U)
#define DEMO_PDM_FIFO_WATERMARK       (7U)
#define DEMO_PDM_QUALITY_MODE         kPDM_QualityModeHigh
#define DEMO_PDM_CIC_OVERSAMPLE_RATE  (0U)
#define DEMO_PDM_ENABLE_CHANNEL_LEFT  (0U)
#define DEMO_PDM_ENABLE_CHANNEL_RIGHT (1U)
#define DEMO_PDM_SAMPLE_CLOCK_RATE    (1536000U * 4) /* 6.144MHZ */
#define DEMO_PDM_HWVAD_SIGNAL_GAIN    0

#define DEMO_SAI                 (I2S3)
#define DEMO_SAI_CLK_FREQ        (24576000U)
#define DEMO_SAI_FIFO_WATER_MARK (FSL_FEATURE_SAI_FIFO_COUNTn(DEMO_SAI) / 2U)
#define DEMO_SAI_CLOCK_SOURCE    (kSAI_BclkSourceMclkDiv)
#define DEMO_SAI_MASTER_SLAVE    kSAI_Master
#define DEMO_SAI_CHANNEL         0U
#define DEMO_CODEC_WM8524        1
#define DEMO_CODEC_BUS_PIN       (NULL)
#define DEMO_CODEC_BUS_PIN_NUM   (0)
#define DEMO_CODEC_MUTE_PIN      (GPIO5)
#define DEMO_CODEC_MUTE_PIN_NUM  (21)

#define DEMO_AUDIO_SAMPLE_RATE  (kSAI_SampleRate48KHz)
#define DEMO_AUDIO_MASTER_CLOCK DEMO_SAI_CLK_FREQ
#define DEMO_AUDIO_DATA_CHANNEL (2U)
#define DEMO_AUDIO_BIT_WIDTH    kSAI_WordWidth16bits

#define DEMO_BOARD_CODEC_INIT BOARD_Codec_Init

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void BOARD_WM8524_Mute_GPIO(uint32_t output);
void BOARD_MasterClockConfig(void);
void BOARD_Codec_Init(void);

/*${prototype:end}*/

#endif /* _APP_H_ */
