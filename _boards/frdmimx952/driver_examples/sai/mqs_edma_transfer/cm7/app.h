/*
 * Copyright 2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
/* SAI instance */
#define DEMO_SAI              SAI1
#define DEMO_SAI_CHANNEL      (0)
#define DEMO_SAI_TX_SYNC_MODE kSAI_ModeAsync
#define DEMO_SAI_MASTER_SLAVE kSAI_Master

#define DEMO_AUDIO_DATA_CHANNEL (2U)
#define DEMO_AUDIO_BIT_WIDTH    kSAI_WordWidth16bits
#define DEMO_AUDIO_SAMPLE_RATE  (kSAI_SampleRate48KHz)

/* Get frequency of sai1 clock */
#define SAI_CLOCK_ROOT          kCLOCK_sai1
#define DEMO_SAI_CLK_FREQ       CLOCK_GetRate(SAI_CLOCK_ROOT)
#define DEMO_AUDIO_MASTER_CLOCK DEMO_SAI_CLK_FREQ
#define HMCLK_FREQ              DEMO_SAI_CLK_FREQ

/* DMA (SAI1 TX uses DMA3 request mux on i.MX952) */
#define DEMO_DMA              DMA3
#define DEMO_EDMA_CHANNEL     Dma3RequestMuxSai1Tx
#define DEMO_SAI_EDMA_CHANNEL Dma3RequestMuxSai1Tx
#define DEMO_XFER_BUFFER_SIZE (3200U)

#define BOARD_MASTER_CLOCK_CONFIG()
#define BOARD_SAI_RXCONFIG(config, mode)

/* MQS output */
#define DEMO_BOARD_MQS_INIT BOARD_MQS_Init
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void BOARD_MQS_Init(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
