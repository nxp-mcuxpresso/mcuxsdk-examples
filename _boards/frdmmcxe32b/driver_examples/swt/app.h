/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define SWT_BASE SWT_0
/* Timeout half second */
#define SWT_TIMEOUT CLOCK_SIRC_CLK_FREQ
#define SWT_IRQ_ID SWT0_IRQn
#define SWT_HANDLER SWT0_IRQHandler
#define SWT_REFRESH_COUNT 3U
/* SWT initial service key for keyed service mode */
#define SWT_SERVICE_KEY 100U
/* Systic count down to 0 at 0.7s, the example window period is set to 0.5-1s */
#define SWT_WINDOW_OPERATE_TIME 700U
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void API_SysTick_Start(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
