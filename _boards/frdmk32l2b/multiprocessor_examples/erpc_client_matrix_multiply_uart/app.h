/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "fsl_lpuart_cmsis.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define BUTTON_INIT()       GPIO_PinInit(BOARD_SW1_GPIO, BOARD_SW1_GPIO_PIN, &button_config)
#define IS_BUTTON_PRESSED() !GPIO_PinRead(BOARD_SW1_GPIO, BOARD_SW1_GPIO_PIN)
#define ERPC_DEMO_UART      Driver_USART0
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
