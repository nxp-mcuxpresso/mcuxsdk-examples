/*
 * Copyright 2019 NXP
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
#define DEMO_PDM_FIFO_WATERMARK       (0x7U)
#define DEMO_PDM_QUALITY_MODE         kPDM_QualityModeHigh
#define DEMO_PDM_CIC_OVERSAMPLE_RATE  (0U)
#define DEMO_PDM_ENABLE_CHANNEL_LEFT  (0U)
#define DEMO_PDM_ENABLE_CHANNEL_RIGHT (1U)
#define DEMO_PDM_SAMPLE_CLOCK_RATE    (1536000U * 4U) /* 6.144MHZ */

#define DEMO_SAI              (I2S3)
#define DEMO_SAI_CLK_FREQ     (24576000U)
#define DEMO_SAI_CLOCK_SOURCE (kSAI_BclkSourceMclkDiv)
#define DEMO_SAI_CHANNEL      (0U)
#define DEMO_SAI_MASTER_SLAVE (kSAI_Master)
/* demo audio sample rate */
#define DEMO_AUDIO_SAMPLE_RATE (kSAI_SampleRate48KHz)
/* demo audio master clock */
#define DEMO_AUDIO_MASTER_CLOCK DEMO_SAI_CLK_FREQ
/* demo audio data channel */
#define DEMO_AUDIO_DATA_CHANNEL (2U)
/* demo audio bit width */
#define DEMO_AUDIO_BIT_WIDTH kSAI_WordWidth32bits

#define I2C_RELEASE_SDA_GPIO  GPIO5
#define I2C_RELEASE_SDA_PIN   19U
#define I2C_RELEASE_SCL_GPIO  GPIO5
#define I2C_RELEASE_SCL_PIN   18U
#define I2C_RELEASE_BUS_COUNT 100U
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void BOARD_I2C_ReleaseBus(void);
void BOARD_MasterClockConfig(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
