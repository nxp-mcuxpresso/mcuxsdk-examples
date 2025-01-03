/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "fsl_dialog7212.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define DEMO_SAI_INSTANCE_INDEX  (1U)
#define DEMO_DMA_INDEX           (0U)
#define DEMO_DMA_TX_CHANNEL      (0U)
#define DEMO_DMAMUX_INDEX        (0U)
#define DEMO_SAI_TX_SOURCE       kDma0RequestMuxSai1Tx
#define DEMO_SAI                 SAI1
#define DEMO_CODEC_INIT_DELAY_MS (1000U)
#ifndef DEMO_CODEC_VOLUME
#define DEMO_CODEC_VOLUME 50U
#endif
/* Get frequency of sai1 clock */
#define DEMO_SAI_CLK_FREQ CLOCK_GetSaiClkFreq(1U)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
