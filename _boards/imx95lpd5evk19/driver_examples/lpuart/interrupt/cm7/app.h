/*
 * Copyright 2023 NXP
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
/* Get source clock for LPIT driver */
#define DEMO_LPUART            LPUART3
#define LPUART_CLOCK_ROOT      hal_clock_lpuart3
#define DEMO_LPUART_CLK_FREQ   HAL_ClockGetIpFreq(BOARD_DEBUG_UART_CLOCK_ROOT_FOR_CM7_WITHOUT_SM)
#define DEMO_LPUART_IRQn       LPUART3_IRQn
#define DEMO_LPUART_IRQHandler LPUART3_IRQHandler
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
