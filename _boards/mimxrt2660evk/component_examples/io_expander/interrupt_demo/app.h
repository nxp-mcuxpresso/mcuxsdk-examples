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
/* Demo pins on this board:
 *   Input  = USB0_ID  (pull-up; plug/unplug a USB OTG cable, or short the pin
 *                      to GND with a probe, to drive an event).
 *   Output = LCD_RST  (harmless when no LCD module is plugged in; otherwise
 *                      driving it toggles the panel's reset line).
 * No other expander pin on mimxrt2660evk is left "naked" (every pin terminates
 * at a fixed peripheral), so the demo deliberately picks the least disruptive
 * pair. Override here if your bench setup has different free pins. */
#define APP_INPUT_PIN   BOARD_PCAL6524_USB0_ID
#define APP_OUTPUT_PIN  BOARD_PCAL6524_LCD_RST
/*${macro:end}*/

/*${prototype:start}*/
void BOARD_InitHardware(void);
void APP_InputPinCallback(uint8_t pin, bool pinState, void *userData);

/* Board-layer lock callback the example installs on s_handle.lock after
 * BOARD_InitPCAL6524. See hardware_init.c. */
status_t APP_PCAL6524_Lock(bool lock);
/*${prototype:end}*/

#endif /* _APP_H_ */
