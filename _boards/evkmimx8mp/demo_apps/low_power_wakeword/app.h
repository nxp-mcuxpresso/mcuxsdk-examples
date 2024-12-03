/*
 * Copyright 2020-2022 NXP
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
#define I2C_RELEASE_SDA_GPIO   GPIO5
#define I2C_RELEASE_SDA_PIN    19U
#define I2C_RELEASE_SCL_GPIO   GPIO5
#define I2C_RELEASE_SCL_PIN    18U
#define I2C_RELEASE_BUS_COUNT  100U
#define RDC_DISABLE_M7_ACCESS  0xF3

/* VoiceSpot and VoiceSeeker configuration */
#define DEVICE_ID             Device_IMX8M_CM7
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
void BOARD_I2C_ReleaseBus(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
