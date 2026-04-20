/*
 * Copyright 2026 NXP
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

#define APP_MU         MUB
#define APP_MU_CHANNEL (0)

#define APP_AON_LPUART             AON__LPUART0
#define APP_AON_LPUART_IRQn        LPUART0_AON_IRQn
#define APP_AON_LPUART_IRQ_HANDLER LPUART0_AON_IRQHandler

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_BootAs10MHzClocks(void);
void BOARD_InitHardware(void);
void BOARD_DeinitDebugConsole(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
