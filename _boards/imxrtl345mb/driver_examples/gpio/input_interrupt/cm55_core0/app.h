/*
 * Copyright 2026 NXP
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define GPIO CTRLSS__GPIO
#define APP_SW_PORT              BOARD_SW1_GPIO_PORT
#define APP_SW_PIN               BOARD_SW1_GPIO_PIN
#define APP_GPIO_INTA_IRQHandler GPIO_INTA_DriverIRQHandler
#define APP_SW_IRQ               GPIO_INTA_IRQn
#define APP_SW_CONNECTED_LEVEL   0U
#define APP_SW_NAME              "SW1"
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
