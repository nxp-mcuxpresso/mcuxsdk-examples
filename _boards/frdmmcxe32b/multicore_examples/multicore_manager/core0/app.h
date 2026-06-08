/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "fsl_siul2.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
/* Flash address where the secondary core (M7_1) image is programmed and runs
 * in place. The secondary core is released by writing this address to the
 * MC_ME PRTN0_CORE1 interface; the image is not copied to RAM. */
#define CORE1_BOOT_ADDRESS 0x00600000

/* Only one button (SW3, labelled "User" on the board) is wired for this demo */
#define APP_ONE_BUTTON_ONLY

#define BUTTON_1_INIT()                                                                        \
    SIUL2_SetPinDirection(SIUL2,                                                               \
                          (uint32_t)(BOARD_SW3_GPIO) * 32U + (BOARD_SW3_GPIO_PIN), kPORT_IN)
#define IS_BUTTON_1_PRESSED() \
    ((0U == SIUL2_PortPinRead(SIUL2, BOARD_SW3_GPIO, BOARD_SW3_GPIO_PIN)) && \
     (secondary_core_started == 1))
#define BUTTON_1_NAME "SW3"

#define IS_BUTTON_2_PRESSED() \
    ((0U == SIUL2_PortPinRead(SIUL2, BOARD_SW3_GPIO, BOARD_SW3_GPIO_PIN)) && \
     (secondary_core_started == 0))
/*${macro:end}*/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
/*******************************************************************************
 * EOF
 ******************************************************************************/
