/*
 * Copyright 2017 NXP
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
#define RDC_DISABLE_A53_ACCESS 0xFC
#define RDC_DISABLE_M4_ACCESS  0xF3

/* VoiceSpot and VoiceSeeker configuration */
#define DEVICE_ID             Device_IMX8M_CM4
#define RX_BUFFER_LEN         (15 * 1024) /* 60ms * 4channels * 16KHz * 32bits*/
#define RX_BUFFER_THR         (3)         /* nb of periods. */
#define NB_FRAMES_PER_PERIOD  (6)         /* number of VoiceSeeker input frames per DMA period. */
#define MIC_ARRAY_COORDS      {{-17.5f, 0.0f, 0.0f}, {17.5f, 0.0f, 0.0f}}

#define APP_MEM2MEM_W_DMA_CHANNEL (5U)
#define APP_MEM2MEM_R_DMA_CHANNEL (6U)

/*${macro:end}*/
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
