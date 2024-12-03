/*
 * Copyright 2022-2023 NXP
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

/* VoiceSpot and VoiceSeeker configuration */
#define DEVICE_ID             Device_IMX93_CM33
#define RX_BUFFER_LEN         (14 * 1024) /* 56ms * 4channels * 16KHz * 32bits*/
#define RX_BUFFER_THR         (1)         /* nb of periods. */
#define NB_FRAMES_PER_PERIOD  (9)         /* number of VoiceSeeker input frames per DMA period. */
#define MIC_ARRAY_COORDS      {{-58.0f, 0.0f, 0.0f}, {58.0f, 0.0f, 0.0f}}

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
