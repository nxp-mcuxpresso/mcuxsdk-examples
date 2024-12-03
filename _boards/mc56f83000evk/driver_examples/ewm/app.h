/*
 * Copyright 2020 NXP
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
#define EXAMPLE_EWM         EWM
#define WDOG_EWM_IRQn       EWM_INT_IRQn
#define WDOG_EWM_PRIO       1
#define WDOG_EWM_IRQHandler ivINT_EWM_INT

#define SW_GPIO               BOARD_BUTTON_SW2_GPIO
#define SW_GPIO_PIN           BOARD_BUTTON_SW2_GPIO_PIN
#define SW_GPIO_PRESSED_VALUE 0
#define SW_NAME               "SW2"
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
