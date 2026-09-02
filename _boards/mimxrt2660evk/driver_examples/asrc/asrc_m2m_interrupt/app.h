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
#define DEMO_CODEC_VOLUME (60U)

#define DEMO_SAI          AUDIO__SAI_0
#define DEMO_SAI_CLK_FREQ (CLOCK_GetRootClockFreq(kCLOCK_Root_AUDIO_sai0_mclk0))

#define DEMO_ASRC                        AUDIO__ASRC
#define DEMO_ASRC_PERIPHERAL_CLOCK       CLOCK_GetRootClockFreq(kCLOCK_Root_AUDIO_asrc_clk)
#define DEMO_ASRC_CHANNEL_PAIR           kASRC_ChannelPairA
#define DEMO_ASRC_OUTPUT_CLOCK_SOURCE    kASRC_ClockSourceBitClock0_AUD_PLL_DIV_CLK0
#define DEMO_ASRC_OUTPUT_SOURCE_CLOCK_HZ (16U * DEMO_AUDIO_SAMPLE_RATE_IN * 2U)

#define DEMO_AUDIO_MASTER_CLOCK    DEMO_SAI_CLK_FREQ
#define DEMO_AUDIO_SAMPLE_RATE_IN  kSAI_SampleRate16KHz
#define DEMO_AUDIO_SAMPLE_RATE_OUT kSAI_SampleRate48KHz

#define BOARD_MASTER_CLOCK_CONFIG() BOARD_MasterClockConfig()
#define BOARD_SAI_RXCONFIG(config, mode)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void BOARD_MasterClockConfig(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
