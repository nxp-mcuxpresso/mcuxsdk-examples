/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define DEMO_CODEC_WM8960
#define DEMO_SAI         AUDIO__SAI_0
#define DEMO_SAI_IRQ     AUDIO_SAI0_IRQn
#define SAI_TxIRQHandler AUDIO_SAI0_IRQHandler

#define DEMO_AUDIO_MASTER_CLOCK          (CLOCK_GetRootClockFreq(kCLOCK_Root_AUDIO_sai0_mclk0))
#define DEMO_ASRC_OUTPUT_CLOCK_SOURCE    kASRC_ClockSourceBitClock8_SAI0_RX_BCLK
#define DEMO_ASRC_OUTPUT_SOURCE_CLOCK_HZ (16 * DEMO_AUDIO_SAMPLE_RATE_OUT * 2)
#define DEMO_ASRC_PERIPHERAL_CLOCK       200000000U
#define DEMO_ASRC                        AUDIO__ASRC
#define DEMO_ASRC_CHANNEL_PAIR           kASRC_ChannelPairA

#define DEMO_AUDIO_SAMPLE_RATE_IN  (kSAI_SampleRate16KHz)
#define DEMO_AUDIO_SAMPLE_RATE_OUT (kSAI_SampleRate48KHz)

#define DEMO_I2C          BOARD_CODEC_I2C_BASEADDR
#define DEMO_I2C_CLK_FREQ CLOCK_GetRootClockFreq(kCLOCK_Root_MAIN_lpi2c1_fclk)

#define EXAMPLE_DMA           AUDIO__EDMA3
#define EXAMPLE_CHANNEL       (0U)
#define EXAMPLE_SAI_TX_SOURCE kDmaRequestMux2AudioSAI0Tx

#define DEMO_CODEC_VOLUME (75U)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
