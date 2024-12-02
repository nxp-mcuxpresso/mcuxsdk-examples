/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
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
#define DMAMUX0                     DMA_CH_MUX0
#define LTC_REQUEST_INPUT_FIFO      kDmaRequestMux0LTCRx
#define LTC_REQUEST_OUTPUT_FIFO     kDmaRequestMux0LTCTx
#define DMA_CHANNEL_LTC_INPUT_FIFO  0
#define DMA_CHANNEL_LTC_OUTPUT_FIFO 1
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
