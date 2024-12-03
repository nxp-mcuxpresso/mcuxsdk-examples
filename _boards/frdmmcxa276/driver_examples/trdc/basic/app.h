/*
 * Copyright 2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "fsl_trdc.h"
#include "fsl_debug_console.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define EXAMPLE_TRDC_INSTANCE         MBC0
#define EXAMPLE_TRDC_MBC_SLAVE_INDEX  0 /* Main flash */
#define EXAMPLE_TRDC_MBC_MEMORY_INDEX 63 /* Block size 8K */

/*
 * This platform only supports secure privilege mode.
 * This example uses two global access registers, one has read permission, and
 * the other doesn't have.

 */
#define EXAMPLE_TRDC_MBC_ACCESS_CONTROL_POLICY_INDEX           4
#define EXAMPLE_TRDC_MBC_ACCESS_CONTROL_POLICY_INDEX_NO_ACCESS 5
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void APP_SetTrdcGlobalConfig(void);
void APP_SetMbcUnaccessible(void);
void APP_TouchMbcMemory(void);
void APP_ResolveMbcAccessError(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
