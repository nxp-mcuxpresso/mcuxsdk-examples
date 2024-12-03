/*
 * Copyright 2018 NXP
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
#define DEMO_PDM PDM
#define DEMO_PDM_CLK_FREQ \
    (24000000U) / (CLOCK_GetRootPreDivider(kCLOCK_RootPdm)) / (CLOCK_GetRootPostDivider(kCLOCK_RootPdm))
#define DEMO_PDM_FIFO_WATERMARK       (4U)
#define DEMO_PDM_QUALITY_MODE         kPDM_QualityModeHigh
#define DEMO_PDM_CIC_OVERSAMPLE_RATE  (0U)
#define DEMO_PDM_ENABLE_CHANNEL_LEFT  (0U)
#define DEMO_PDM_ENABLE_CHANNEL_RIGHT (1U)
#define DEMO_PDM_SAMPLE_CLOCK_RATE    (640000U) /* 640KHZ */
#define DEMO_PDM_DMA_REQUEST_SOURCE   (24U)
#define DEMO_PDM_HWVAD_SIGNAL_GAIN    0

#define DEMO_DMA         SDMAARM3
#define DEMO_DMA_CHANNEL (1U)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
