/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "fsl_wm8960.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define DEMO_SAI          (I2S3)
#define DEMO_ASRC         ASRC
#define DEMO_SAI_CLK_FREQ 24576000U

#define DEMO_IRQn             I2S3_IRQn
#define DEMO_DMA              SDMAARM3
#define DEMO_SAI_SDMA_CHANNEL (1)
#define DEMO_ASRC_CONTEXT     kASRC_Context0
#define DEMO_SAI_TX_SOURCE    (5)
#define SAI_UserIRQHandler    I2S3_IRQHandler
/*set Bclk source to Mclk clock*/
#define DEMO_SAI_CLOCK_SOURCE     (1U)
#define SDMA_FREQ_EQUALS_ARM      (1U)
#define DEMO_ASRC_IN_SAMPLE_RATE  16000
#define DEMO_ASRC_OUT_SAMPLE_RATE 48000

#define I2C_RELEASE_SDA_GPIO  GPIO5
#define I2C_RELEASE_SDA_PIN   19U
#define I2C_RELEASE_SCL_GPIO  GPIO5
#define I2C_RELEASE_SCL_PIN   18U
#define I2C_RELEASE_BUS_COUNT 100U

#define DEMO_AUDIO_MASTER_CLOCK DEMO_SAI_CLK_FREQ
#define DEMO_AUDIO_BIT_WIDTH    kSAI_WordWidth16bits
#define DEMO_AUDIO_DATA_CHANNEL (2U)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void BOARD_I2C_ReleaseBus(void);
void BOARD_MASTER_CLOCK_CONFIG(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
