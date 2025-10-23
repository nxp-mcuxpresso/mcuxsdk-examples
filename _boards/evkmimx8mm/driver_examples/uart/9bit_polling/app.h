/*
 * Copyright 2022-2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
 
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define DEMO_UART          UART3
#define DEMO_UART_CLK_FREQ                                                                  \
    CLOCK_GetPllFreq(kCLOCK_SystemPll1Ctrl) / (CLOCK_GetRootPreDivider(kCLOCK_RootUart3)) / \
        (CLOCK_GetRootPostDivider(kCLOCK_RootUart3)) / 10
#define EXAMPLE_ADDRESS 0xFEU
#define TRANSFER_SIZE   8U
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
