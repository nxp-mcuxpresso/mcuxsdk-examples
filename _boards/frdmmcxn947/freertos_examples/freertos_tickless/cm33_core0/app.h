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
#define BOARD_SW_GPIO          BOARD_SW3_GPIO
#define BOARD_SW_PORT          BOARD_SW3_GPIO_PORT
#define BOARD_SW_GPIO_PIN      BOARD_SW3_GPIO_PIN
#define BOARD_SW_IRQ           BOARD_SW3_IRQ
#define BOARD_SW_IRQ_HANDLER   BOARD_SW3_IRQ_HANDLER
#define BOARD_SW_NAME          BOARD_SW3_NAME
#define BOARD_PINT_PIN_INT_SRC kINPUTMUX_GpioPort0Pin6ToPintsel
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
extern void vPortLptmrIsr(void);
IRQn_Type vPortGetLptmrIrqn(void);
LPTMR_Type *vPortGetLptrmBase(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
