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
#define DEMO_DMA_RX_CHANNEL      (1U)
#define DEMO_SAI                 (SAI1)
#define DEMO_SAI_TX_SOURCE       kDma0RequestMuxSai1Tx
#define DEMO_SAI_RX_SOURCE       kDma0RequestMuxSai1Rx
#define DEMO_CODEC_INIT_DELAY_MS (1000U)
#ifndef DEMO_CODEC_VOLUME
#define DEMO_CODEC_VOLUME 50U
#endif
/* Get frequency of sai1 clock */
#define DEMO_SAI_CLK_FREQ CLOCK_GetSaiClkFreq(1U)

#define BOARD_SW_GPIO        BOARD_SW3_GPIO
#define BOARD_SW_PORT        BOARD_SW3_PORT
#define BOARD_SW_GPIO_PIN    BOARD_SW3_GPIO_PIN
#define BOARD_SW_IRQ         BOARD_SW3_IRQ
#define BOARD_SW_IRQ_HANDLER BOARD_SW3_IRQ_HANDLER
#define BOARD_SW_NAME        BOARD_SW3_NAME
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
