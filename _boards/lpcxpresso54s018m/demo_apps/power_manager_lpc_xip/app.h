/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
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

#define APP_USART_RX_ERROR               kUSART_RxError
#define APP_RUNNING_INTERNAL_CLOCK       BOARD_BootClockFRO12M()
#define APP_USER_WAKEUP_KEY_NAME         "SW5"
#define APP_USER_WAKEUP_KEY_GPIO         BOARD_SW5_GPIO
#define APP_USER_WAKEUP_KEY_PORT         BOARD_SW5_GPIO_PORT
#define APP_USER_WAKEUP_KEY_PIN          BOARD_SW5_GPIO_PIN
#define APP_USER_WAKEUP_KEY_INPUTMUX_SEL kINPUTMUX_GpioPort1Pin1ToPintsel

#define APP_EXCLUDE_FROM_SLEEP \
    (SYSCON_PDRUNCFG_PDEN_SRAMX_MASK | SYSCON_PDRUNCFG_PDEN_SRAM0_MASK | SYSCON_PDRUNCFG_PDEN_SRAM1_2_3_MASK)
/* PDRUNCFG0 bit 13 (PDEN_SRAMX) controls only SRAMX address 0x0 to 0x0000FFFF. PDRUNCFG0 bit 29 (PDEN_VD6) controls
 * SRAMX address 0x00010000 to 0x0002FFFF. */

/* XIP IMGAG use SRAMX as data, heap, and stack section, its power should be left in deep sleep mode */
#define APP_EXCLUDE_FROM_DEEPSLEEP                                                                             \
    (SYSCON_PDRUNCFG_PDEN_SRAMX_MASK | SYSCON_PDRUNCFG_PDEN_SRAM0_MASK | SYSCON_PDRUNCFG_PDEN_SRAM1_2_3_MASK | \
     SYSCON_PDRUNCFG_PDEN_VD6_MASK)

#define APP_SYSCON_STARTER_MASK SYSCON_STARTER_PIN_INT0_MASK

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
