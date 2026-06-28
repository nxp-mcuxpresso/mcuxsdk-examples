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
/* PCAL6524 P2_4 / P2_5 terminate only at TP9 / TP12; no other peripheral
 * sits on them, so the demo needs no upstream IC init to be quiescent. */
#define APP_INPUT_PIN   BOARD_PCAL6524_P2_5
#define APP_OUTPUT_PIN  BOARD_PCAL6524_P2_4
/*${macro:end}*/

/*${prototype:start}*/
void BOARD_InitHardware(void);
void APP_InputPinCallback(uint8_t pin, bool pinState, void *userData);
status_t APP_PCAL6524_Lock(bool lock);
/*${prototype:end}*/

#endif /* _APP_H_ */
