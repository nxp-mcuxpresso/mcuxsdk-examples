/*
 * Copyright 2026 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "fsl_wm8962.h"
#include "fsl_edma.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
/* SAI instance and clock */
// #define DEMO_CODEC_WM8960
#define DEMO_SAI         SAI3
#define DEMO_SAI_IRQ     SAI3_IRQn
#define SAI_TxIRQHandler SAI3_IRQHandler

#define DEMO_SAI_DMA               EDMA5_2
#define DEMO_SAI_DMA_CHANNEL       Dma5RequestMuxSai3Tx
#define DEMO_ASRC_DMA              EDMA5_2
#define DEMO_ASRC_IN_EDMA_CHANNEL  Dma5RequestMuxAsrc1AIn
#define DEMO_ASRC_OUT_EDMA_CHANNEL Dma5RequestMuxAsrc1AOut
/* Select Audio/Video PLL (786.48 MHz) as sai3 clock source */
/* Clock pre divider for sai3 clock source */
/* Clock divider for sai3 clock source */
/* Get frequency of sai3 clock */
#define SAI_CLOCK_ROOT    kCLOCK_sai3
#define DEMO_SAI_CLK_FREQ CLOCK_GetRate(SAI_CLOCK_ROOT)

/* I2C instance and clock */
#define DEMO_I2C                LPI2C4
#define LPI2C_MASTER_CLOCK_ROOT kCLOCK_lpi2c4
#define DEMO_I2C_CLK_FREQ       CLOCK_GetRate(LPI2C_MASTER_CLOCK_ROOT)

#define DEMO_AUDIO_MASTER_CLOCK 	 DEMO_SAI_CLK_FREQ
#define DEMO_ASRC_OUTPUT_SOURCE_CLOCK_HZ (16 * kSAI_SampleRate32KHz * 2)
#define DEMO_ASRC_PERIPHERAL_CLOCK       CLOCK_GetRate(kCLOCK_asrc1)
#define DEMO_ASRC                        ASRC1
#define DEMO_ASRC_CHANNEL_PAIR           kASRC_ChannelPairA

#define DEMO_AUDIO_SAMPLE_RATE_IN  (kSAI_SampleRate48KHz)
#define DEMO_AUDIO_SAMPLE_RATE_OUT (kSAI_SampleRate32KHz)

#define DEMO_ASRC_OUTPUT_CLOCK_SOURCE kASRC_ClockSourceBitClock9_SAI3_TX
/* I2C instance and clock */
//#define DEMO_I2C LPI2C1

/* Select USB1 PLL (480 MHz) as master lpi2c clock source */
//#define DEMO_LPI2C_CLOCK_SOURCE_SELECT (0U)
/* Clock divider for master lpi2c clock source */
//#define DEMO_LPI2C_CLOCK_SOURCE_DIVIDER (5U)
/* Get frequency of lpi2c clock */
//#define DEMO_I2C_CLK_FREQ ((CLOCK_GetFreq(kCLOCK_Usb1PllClk) / 8) / (DEMO_LPI2C_CLOCK_SOURCE_DIVIDER + 1U))

/* DMA */
// #define EXAMPLE_DMA     DMA1
// #define EXAMPLE_CHANNEL       (0U)
#define DEMO_ASRC_IN_CHANNEL  (1U)
#define DEMO_ASRC_OUT_CHANNEL (4U)
#define DEMO_SAI_CHANNEL      (0U)

#define DEMO_CODEC_VOLUME              (78U)
#define BOARD_SAI_EDMA_CONFIG(config)
#define BOARD_ASRC_EDMA_CONFIG(config)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
