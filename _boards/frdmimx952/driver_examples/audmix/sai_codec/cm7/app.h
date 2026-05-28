/*
 * Copyright 2026 NXP
 * All rights reserved.
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
#define DEMO_SAI1             SAI3
#define DEMO_SAI2             SAI6
#define DEMO_SAI3             SAI6
#define DEMO_AUDMIX           WAKEUP__AUDMIX1
#define DEMO_SAI_BITWIDTH     kSAI_WordWidth32bits
#define DEMO_SAI_CHANNEL_NUM  8U
#define DEMO_SAI_TX_SYNC_MODE kSAI_ModeAsync
#define DEMO_SAI_MASTER_SLAVE kSAI_Master
#define DEMO_CODEC_VOLUME     75

#define DEMO_SAI_SAMPLE_RATE  (kSAI_SampleRate16KHz)

/* Get frequency of sai3 clock */
#define SAI_CLOCK_ROOT          kCLOCK_sai3
#define DEMO_SAI_CLK_FREQ       CLOCK_GetRate(SAI_CLOCK_ROOT)

/* I2C instance and clock */
#define DEMO_I2C                LPI2C4
#define LPI2C_MASTER_CLOCK_ROOT kCLOCK_lpi2c4
#define DEMO_I2C_CLK_FREQ       CLOCK_GetRate(LPI2C_MASTER_CLOCK_ROOT)

#define DEMO_SAI1_TX_SOURCE    Dma5RequestMuxSai3Tx
#define DEMO_SAI2_TX_SOURCE    Dma5RequestMuxSai6Tx

/* DMA */
#define DEMO_DMA               EDMA5_2
#define DEMO_EDMA_CHANNEL      2U
#define DEMO_SAI_EDMA_CHANNEL  Dma5RequestMuxSai3Tx
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
