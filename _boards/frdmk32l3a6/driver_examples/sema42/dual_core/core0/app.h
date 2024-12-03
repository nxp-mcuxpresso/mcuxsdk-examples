/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "fsl_mu.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define LED_INIT()          LED1_INIT(LOGIC_LED_ON)
#define APP_MU              MUA
#define APP_SEMA42          SEMA420
#define APP_CORE1_BOOT_MODE kMU_CoreBootFromDflashBase
/*${macro:end}*/
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void APP_InitDomain(void);
void APP_DeinitDomain(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
