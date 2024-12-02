/*
 * Copyright 2022-2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define APP_DEBUG_UART_BAUDRATE (115200U) /* Debug console baud rate. */
#define APP_LPTMR2_IRQ_PRIO     (5U)
#define APP_LPUART2_IRQ_PRIO    (5U)

/* SAI and I2C instance and clock */
#define DEMO_SAI              SAI3
#define DEMO_SAI_CHANNEL      (0)
#define DEMO_SAI_TX_SYNC_MODE kSAI_ModeAsync
#define DEMO_SAI_MASTER_SLAVE kSAI_Master
#define DEMO_CODEC_VOLUME     75

#define DEMO_AUDIO_DATA_CHANNEL (2U)
#define DEMO_AUDIO_BIT_WIDTH    kSAI_WordWidth16bits
#define DEMO_AUDIO_SAMPLE_RATE  (kSAI_SampleRate16KHz)

/* Get frequency of sai3 clock */
#define SAI_CLOCK_ROOT          hal_clock_sai3
#define DEMO_SAI_CLK_FREQ       HAL_ClockGetIpFreq(SAI_CLOCK_ROOT)
#define DEMO_AUDIO_MASTER_CLOCK DEMO_SAI_CLK_FREQ

/* I2C instance and clock */
#define DEMO_I2C                LPI2C4
#define LPI2C_MASTER_CLOCK_ROOT hal_clock_lpi2c4
#define DEMO_I2C_CLK_FREQ       HAL_ClockGetIpFreq(LPI2C_MASTER_CLOCK_ROOT)

/* DMA */
#define DEMO_DMA               EDMA5_2
#define DEMO_EDMA_CHANNEL      2U
#define DEMO_SAI_EDMA_CHANNEL  Dma5RequestMuxSai3Tx
#define DEMO_XFER_BUFFER_SIZE  (3200U)
#define EXAMPLE_DMA_CLOCK_ROOT kCLOCK_Root_WakeupAxi
#define BOARD_MASTER_CLOCK_CONFIG()
#define BOARD_SAI_RXCONFIG(config, mode)

/* VoiceSpot and VoiceSeeker configuration */
#define DEVICE_ID             Device_IMX95_CM7
#define RX_BUFFER_LEN         (36 * 1024) /* 144 ms * 4channels * 16KHz * 32bits*/
#define RX_BUFFER_THR         (1)         /* nb of periods. */
#define NB_FRAMES_PER_PERIOD  (18)        /* number of VoiceSeeker input frames per DMA period. */
#define MIC_ARRAY_COORDS      {{-33.5f, 0.0f, 0.0f}, {33.5f, 0.0f, 0.0f}};

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
