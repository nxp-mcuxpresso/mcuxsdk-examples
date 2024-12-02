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

#define DEMO_I2C                        (I2C2)
#define DEMO_I2S_MASTER_CLOCK_FREQUENCY (24576000U)
#define DEMO_I2S_TX                     (I2S0)
#define DEMO_I2S_TX_MODE                kI2S_MasterSlaveNormalMaster
#define DEMO_I2S_CLOCK_DIVIDER                                            \
    (CLOCK_GetAudioPllOutFreq() / 48000U / 16U /                          \
     2U) /* I2S source clock 24.576MHZ, sample rate 48KHZ, bits width 16, \
single channel, so bitclock should be 48KHZ * 16 * 2 = 1536KHZ, divider should be 24.576MHZ / 1536KHZ / 2 */
#define DEMO_DMA (DMA0)

#define DMAREQ_DMIC1             17U
#define DEMO_DMIC_RX_CHANNEL     DMAREQ_DMIC1
#define DEMO_I2S_TX_CHANNEL      (13)
#define DEMO_DMIC_CHANNEL        kDMIC_Channel1
#define DEMO_DMIC_CHANNEL_ENABLE DMIC_CHANEN_EN_CH1(1)

#define DEMO_AUDIO_BIT_WIDTH   (16)
#define DEMO_AUDIO_SAMPLE_RATE (48000)
#define DEMO_AUDIO_PROTOCOL    kCODEC_BusI2S
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
