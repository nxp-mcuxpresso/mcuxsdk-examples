/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "fsl_gpio.h"
#include "pin_mux.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/

#define CORE1_DOMAIN_ID 5      /* SoC-defined domain ID for Core 1 (NBU) */
#define APP_SEMA42      SEMA42
#define APP_SEMA42_GATE 0      /* This is the same between both cores */

/*${macro:end}*/
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
