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
/* Use SW3 PTD5 Interrupt EIRQ13 */
#define APP_SIUL2 (SIUL2_Type *)SIUL2_VIRT_ADDR(SIUL2_BASE, APP_VIRT_WRAPPER_PDAC_SLOT)
#define APP_SIUL2_GPIO_PORT kSIUL2_PTD
#define APP_SIUL2_GPIO_PIN  5
#define APP_SIUL2_GPIO_PIN_INPUTMUX_REG 29 /* Which IMCR controls the input mux. */

/*
 * EIRQ 0-7:   SIUL2_0_IRQn
 * EIRQ 8-15:  SIUL2_1_IRQn
 * EIRQ 16-23: SIUL2_2_IRQn
 * EIRQ 24-31: SIUL2_3_IRQn
 */
#define APP_SIUL2_EIRQ             13
#define APP_SIUL2_EIRQ_IRQn        SIUL2_1_IRQn
#define APP_SIUL2_EIRQ_IRQHandler  SIUL2_1_IRQHandler

#define APP_VIRT_WRAPPER VIRT_WRAPPER
#define APP_VIRT_WRAPPER_PDAC_SLOT kVIRT_WRAPPER_PDAC1
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
