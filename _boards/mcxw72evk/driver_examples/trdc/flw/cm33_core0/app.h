/*
 * Copyright 2021 NXP
 * All rights reserved.
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
#define EXAMPLE_TRDC_FLW_BLK_COUNT                       16U
#define EXAMPLE_TRDC_FLW_ARRAY_BASEADDR                  0x0U
#define EXAMPLE_TRDC_PROCESSOR_MASTER_DOMAIN_ID          0
#define EXAMPLE_TRDC_MBC_ACCESS_CONTROL_POLICY_ALL_INDEX 0
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void APP_SetTrdcFlwConfig(void);
void APP_TouchMemory(void);
bool APP_CheckMemoryMap(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
