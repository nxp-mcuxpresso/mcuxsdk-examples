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
#define DEMO_DMA_CHANNEL0                0
#define DEMO_DMA_CHANNEL1                1
#define DEMO_DMA_CHANNEL2                2
#define DEMO_DMA_CHANNEL_TRIGGER_INPUT   kINPUTMUX_DmaTriggerMux0ToDma
#define DEMO_DMA_CHANNEL_TRIGGER_OUTPUT0 kINPUTMUX_DmaChannel0TrigoutToTriginChannels
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
