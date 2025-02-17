/*
 * Copyright 2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "dsp_config.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
/*${macro:end}*/

#define APP_SEMA42 SEMA42_4
#define SEMA_CORE_ID_DSP  4

#define SEMA_PRINTF_NUM	  0
#define SEMA_STARTUP_NUM  1
void DSP_PRINTF(const char* ptr, ...);

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
