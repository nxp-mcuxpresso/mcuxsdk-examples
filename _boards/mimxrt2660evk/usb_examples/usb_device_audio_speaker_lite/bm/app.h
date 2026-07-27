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
#define DEMO_DMA_INDEX      (2U) /* the index is based on the DMA instance array */
#define DEMO_DMA_TX_CHANNEL (0U)
#define DEMO_DMA_RX_CHANNEL (1U)

#define DEMO_SAI                AUDIO__SAI_0
#define DEMO_SAI_INDEX          (0U)
#define DEMO_SAI_CHANNEL        (0U)
#define DEMO_SAI_DMA_TX_CHANNEL kDmaRequestMux2AudioSAI0Tx
#define DEMO_SAI_DMA_RX_CHANNEL kDmaRequestMux2AudioSAI0Rx

#define DEMO_AUDIO_MASTER_CLOCK DEMO_SAI_CLK_FREQ
#define DEMO_SAI_CLK_FREQ       CLOCK_GetRootClockFreq(kCLOCK_Root_AUDIO_sai0_mclk0)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
