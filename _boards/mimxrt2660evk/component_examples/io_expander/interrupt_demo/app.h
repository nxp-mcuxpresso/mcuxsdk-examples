/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include <stdint.h>
#include <stdbool.h>
#include "fsl_common.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
/* Demo pins on this board (PCAL6524 U23):
 *   Input  = WIFI_WAKE_B_3V3 (P0_0) Routed to J25-2. The pin is pulled up on
 *                                   board; short J25-2 to any GND point (e.g.
 *                                   J34-14) with a jumper wire to drive it LOW
 *                                   and trigger an interrupt. Removing the
 *                                   short lets the pull-up restore HIGH, which
 *                                   triggers the opposite-edge interrupt, so
 *                                   the demo loops back and forth.
 *   Output = USB0_PWR (P2_6)        Follows the input level; verify it by
 *                                   measuring the voltage across R82.
 * Override here if your bench setup uses different free pins. */

#define APP_INPUT_PIN   BOARD_PCAL6524_WIFI_WAKE_B_3V3
#define APP_OUTPUT_PIN  BOARD_PCAL6524_USB0_PWR



/*${macro:end}*/

/*${prototype:start}*/
void BOARD_InitHardware(void);
void APP_InputPinCallback(uint8_t pin, bool pinState, void *userData);

/* Board-layer lock callback the example installs on s_handle.lock after
 * BOARD_InitPCAL6524. See hardware_init.c. */
status_t APP_PCAL6524_Lock(bool lock);
/*${prototype:end}*/

#endif /* _APP_H_ */
