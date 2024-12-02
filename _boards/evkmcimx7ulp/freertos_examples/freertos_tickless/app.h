/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
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
#define BOARD_SW_GPIO        BOARD_VOLP_GPIO
#define BOARD_SW_PORT        BOARD_VOLP_PORT
#define BOARD_SW_GPIO_PIN    BOARD_VOLP_GPIO_PIN
#define BOARD_SW_IRQ         BOARD_VOLP_IRQ
#define BOARD_SW_IRQ_HANDLER BOARD_VOLP_IRQ_HANDLER
#define BOARD_SW_NAME        BOARD_VOLP_NAME
/* @brief FreeRTOS tickless timer configuration. */
#define TICKLESS_LPTMR_BASE_PTR LPTMR0      /*!< Tickless timer base address. */
#define TICKLESS_LPTMR_IRQn     LPTMR0_IRQn /*!< Tickless timer IRQ number. */
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
