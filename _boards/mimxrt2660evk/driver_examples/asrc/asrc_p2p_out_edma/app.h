/*
 * Copyright 2026 NXP
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
#define DEMO_SAI         AUDIO__SAI_0
#define DEMO_SAI_IRQ     AUDIO_SAI0_IRQn
#define SAI_TxIRQHandler AUDIO_SAI0_IRQHandler

#define DEMO_SAI_DMA               AUDIO__EDMA3
#define DEMO_SAI_DMA_CHANNEL       kDmaRequestMux2AudioSAI0Tx
#define DEMO_ASRC_DMA              AUDIO__EDMA3
#define DEMO_ASRC_IN_EDMA_CHANNEL  kDmaRequestMux2AudioAsrcPairAInput
#define DEMO_ASRC_OUT_EDMA_CHANNEL kDmaRequestMux2AudioAsrcPairAOutput

#define DEMO_AUDIO_MASTER_CLOCK          (CLOCK_GetRootClockFreq(kCLOCK_Root_AUDIO_sai0_mclk0))
#define DEMO_ASRC_OUTPUT_CLOCK_SOURCE    kASRC_ClockSourceBitClock8_SAI0_RX_BCLK
#define DEMO_ASRC_OUTPUT_SOURCE_CLOCK_HZ (16 * DEMO_AUDIO_SAMPLE_RATE_OUT * 2)
#define DEMO_ASRC_PERIPHERAL_CLOCK       200000000U
#define DEMO_ASRC                        AUDIO__ASRC
#define DEMO_ASRC_CHANNEL_PAIR           kASRC_ChannelPairA

#define DEMO_AUDIO_SAMPLE_RATE_IN  (kSAI_SampleRate48KHz)
#define DEMO_AUDIO_SAMPLE_RATE_OUT (kSAI_SampleRate32KHz)

#define DEMO_I2C          BOARD_CODEC_I2C_BASEADDR
#define DEMO_I2C_CLK_FREQ CLOCK_GetRootClockFreq(kCLOCK_Root_MAIN_lpi2c1_fclk)

#define DEMO_ASRC_IN_CHANNEL  (1U)
#define DEMO_ASRC_OUT_CHANNEL (4U)
#define DEMO_SAI_CHANNEL      (0U)

#define DEMO_CODEC_VOLUME              (78U)
#define BOARD_SAI_EDMA_CONFIG(config)  Board_SaiEdmaConfig(config)
#define BOARD_ASRC_EDMA_CONFIG(config) Board_AsrcEdmaConfig(config)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void Board_SaiEdmaConfig(edma_config_t *config);
void Board_AsrcEdmaConfig(edma_config_t *config);
/*${prototype:end}*/

#endif /* _APP_H_ */
