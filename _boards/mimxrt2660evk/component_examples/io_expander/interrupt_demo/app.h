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
 *   Input  = USB0_ID (P0_5)         On board it is pulled up. Connect OTG
 *                                   port J1 to a USB OTG device (U-Disk,
 *                                   phone, ...) to pull this pin down and
 *                                   drive an IO expander interrupt.
 *   Output = WIFI_WAKE_B_3V3 (P0_0) Routed to J25-2, which is easy to probe
 *                                   when watching the output follow the input.
 * Override here if your bench setup uses different free pins. */
#define APP_INPUT_PIN   BOARD_PCAL6524_USB0_ID
#define APP_OUTPUT_PIN  BOARD_PCAL6524_WIFI_WAKE_B_3V3

/*${macro:end}*/

/*${prototype:start}*/
void BOARD_InitHardware(void);
void APP_InputPinCallback(uint8_t pin, bool pinState, void *userData);

/* Board-layer lock callback the example installs on s_handle.lock after
 * BOARD_InitPCAL6524. See hardware_init.c. */
status_t APP_PCAL6524_Lock(bool lock);
/*${prototype:end}*/

#endif /* _APP_H_ */
