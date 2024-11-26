/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "fsl_wm8962.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
/* SAI and I2C instance and clock */
#define DEMO_SAI              SAI1
#define DEMO_SAI_CHANNEL      (0)
#define DEMO_SAI_TX_SYNC_MODE kSAI_ModeAsync
#define DEMO_SAI_MASTER_SLAVE kSAI_Master
#define DEMO_CODEC_VOLUME     75

#define DEMO_AUDIO_DATA_CHANNEL (2U)
#define DEMO_AUDIO_BIT_WIDTH    kSAI_WordWidth16bits
#define DEMO_AUDIO_SAMPLE_RATE  (kSAI_SampleRate16KHz)

/* Get frequency of sai clock */
#define SAI_CLOCK_ROOT          hal_clock_sai1
#define DEMO_SAI_CLK_FREQ       HAL_ClockGetRate(SAI_CLOCK_ROOT)
#define DEMO_AUDIO_MASTER_CLOCK DEMO_SAI_CLK_FREQ

/* I2C instance and clock */
#define DEMO_I2C                LPI2C3
#define LPI2C_MASTER_CLOCK_ROOT hal_clock_lpi2c3
#define DEMO_I2C_CLK_FREQ       HAL_ClockGetRate(LPI2C_MASTER_CLOCK_ROOT)

/* DMA */
#define DEMO_DMA               EDMA1
#define DEMO_EDMA_CHANNEL      2U
#define DEMO_SAI_EDMA_CHANNEL  kDma1RequestMuxSai1Tx
#define DEMO_XFER_BUFFER_SIZE  (3200U)
#define EXAMPLE_DMA_CLOCK_ROOT hal_clock_m33
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
