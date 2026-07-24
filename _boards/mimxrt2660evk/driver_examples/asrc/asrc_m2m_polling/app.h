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
/* SAI instance */
#define DEMO_SAI              AUDIO__SAI_0
#define DEMO_SAI_IRQ          AUDIO_SAI0_IRQn
#define SAI_TxIRQHandler      AUDIO_SAI0_IRQHandler
#define DEMO_SAI_CHANNEL      (0)
#define DEMO_SAI_TX_SYNC_MODE kSAI_ModeAsync
#define DEMO_SAI_RX_SYNC_MODE kSAI_ModeSync
#define DEMO_SAI_MASTER_SLAVE kSAI_Master

#define DEMO_AUDIO_DATA_CHANNEL (2U)
#define DEMO_AUDIO_BIT_WIDTH    kSAI_WordWidth16bits
#define DEMO_AUDIO_SAMPLE_RATE  (kSAI_SampleRate48KHz)
#define DEMO_AUDIO_MASTER_CLOCK DEMO_SAI_CLK_FREQ

/* SAI clock */
#define DEMO_SAI_CLK_FREQ (CLOCK_GetRootClockFreq(kCLOCK_Root_AUDIO_sai0_mclk0))

/* I2C for codec */
#define DEMO_I2C          BOARD_CODEC_I2C_BASEADDR
#define DEMO_I2C_CLK_FREQ CLOCK_GetRootClockFreq(kCLOCK_Root_MAIN_lpi2c1_fclk)

#define BOARD_MASTER_CLOCK_CONFIG()
#define BOARD_SAI_RXCONFIG(config, mode)

/* ASRC */
#define DEMO_ASRC_INPUT_CLOCK_SOURCE     kASRC_ClockSourceBitClock9_SAI0_TX_BCLK
#define DEMO_ASRC_OUTPUT_CLOCK_SOURCE    kASRC_ClockSourceBitClock8_SAI0_RX_BCLK
#define DEMO_ASRC_OUTPUT_SOURCE_CLOCK_HZ (16 * DEMO_AUDIO_SAMPLE_RATE_OUT * 2)
#define DEMO_ASRC_PERIPHERAL_CLOCK       200000000U
#define DEMO_ASRC                        AUDIO__ASRC
#define DEMO_ASRC_CHANNEL_PAIR           kASRC_ChannelPairA

#define DEMO_AUDIO_SAMPLE_RATE_IN  (kSAI_SampleRate48KHz)
#define DEMO_AUDIO_SAMPLE_RATE_OUT (kSAI_SampleRate32KHz)

#define DEMO_CODEC_VOLUME (75U)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
