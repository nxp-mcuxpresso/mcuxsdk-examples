/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "fsl_wm8904.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
/* SAI and I2C instance and clock */
#define DEMO_SAI              SAI3
#define DEMO_SAI_CHANNEL      (0)
#define DEMO_SAI_TX_SYNC_MODE kSAI_ModeAsync
#define DEMO_SAI_MASTER_SLAVE kSAI_Master
#define DEMO_CODEC_VOLUME     75

#define DEMO_AUDIO_DATA_CHANNEL (2U)
#define DEMO_AUDIO_BIT_WIDTH    kSAI_WordWidth16bits
#define DEMO_AUDIO_SAMPLE_RATE  (kSAI_SampleRate16KHz)

/* Get frequency of sai3 clock */
#define SAI_CLOCK_ROOT          hal_clock_sai3
#define DEMO_SAI_CLK_FREQ       HAL_ClockGetIpFreq(SAI_CLOCK_ROOT)
#define DEMO_AUDIO_MASTER_CLOCK DEMO_SAI_CLK_FREQ

/* I2C instance and clock */
#define DEMO_I2C                LPI2C4
#define LPI2C_MASTER_CLOCK_ROOT hal_clock_lpi2c4
#define DEMO_I2C_CLK_FREQ       HAL_ClockGetIpFreq(LPI2C_MASTER_CLOCK_ROOT)

/* DMA */
#define DEMO_DMA               EDMA5_2
#define DEMO_EDMA_CHANNEL      2U
#define DEMO_SAI_EDMA_CHANNEL  Dma5RequestMuxSai3Tx
#define DEMO_XFER_BUFFER_SIZE  (3200U)
#define EXAMPLE_DMA_CLOCK_ROOT kCLOCK_Root_WakeupAxi
#define BOARD_MASTER_CLOCK_CONFIG()
#define BOARD_SAI_RXCONFIG(config, mode)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
