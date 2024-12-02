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
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void APP_SetXrdcConfig(void);
void APP_CheckAndResolvePeriphAccessError(xrdc_error_t *error);
void APP_CheckAndResolveMemoryAccessError(xrdc_error_t *error);
void APP_SetPeriphUnaccessible(void);
void APP_SetMemoryUnaccessible(void);
void APP_TouchPeriph(void);
void APP_TouchMemory(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
