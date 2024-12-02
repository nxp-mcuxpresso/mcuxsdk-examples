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
#include "fsl_upower.h"
#include "fsl_debug_console.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define EXAMPLE_TRDC_INSTANCE TRDC

#define EXAMPLE_TRDC_PROCESSOR_MASTER_DOMAIN_ID 0
#define EXAMPLE_TRDC_DOMAIN_INDEX               0

#define EXAMPLE_TRDC_MRC_INDEX                            1
#define EXAMPLE_TRDC_MRC_REGION_INDEX                     0
#define EXAMPLE_TRDC_MRC_ACCESS_CONTROL_POLICY_ALL_INDEX  0
#define EXAMPLE_TRDC_MRC_ACCESS_CONTROL_POLICY_NONE_INDEX 1

#define EXAMPLE_TRDC_MBC_INDEX                            0
#define EXAMPLE_TRDC_MBC_SLAVE_INDEX                      3
#define EXAMPLE_TRDC_MBC_MEMORY_INDEX                     0 /* GIPIO A */
#define EXAMPLE_TRDC_MBC_ACCESS_CONTROL_POLICY_ALL_INDEX  0
#define EXAMPLE_TRDC_MBC_ACCESS_CONTROL_POLICY_NONE_INDEX 1

#define FSL_FEATURE_TRDC_HAS_MBC 1
#define FSL_FEATURE_TRDC_HAS_MRC 1
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void APP_SetTrdcGlobalConfig(void);
void APP_SetMrcUnaccessible(void);
void APP_SetMbcUnaccessible(void);
void APP_TouchMrcMemory(void);
void APP_TouchMbcMemory(void);
void APP_CheckAndResolveMrcAccessError(trdc_domain_error_t *error);
void APP_CheckAndResolveMbcAccessError(trdc_domain_error_t *error);
void APP_ResolveMbcAccessError();
void APP_ResolveMrcAccessError();
/*${prototype:end}*/

#endif /* _APP_H_ */
