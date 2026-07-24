/*
 * Copyright 2025 NXP
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

#define EXAMPLE_TRDC_INSTANCE     MAIN__TRDC
#define EXAMPLE_TRDC_DOMAIN_INDEX 0

#define EXAMPLE_TRDC_MRC_INDEX                            kTRDC_MAIN_MrcSRAMC
#define EXAMPLE_TRDC_MRC_START_ADDR                       0x25000000
#define EXAMPLE_TRDC_MRC_END_ADDR                         0x25040000
#define EXAMPLE_TRDC_MRC_REGION_INDEX                     0 /* NS 0x25000000 S 0x35000000 */
#define EXAMPLE_TRDC_MRC_ACCESS_CONTROL_POLICY_ALL_INDEX  0
#define EXAMPLE_TRDC_MRC_ACCESS_CONTROL_POLICY_NONE_INDEX 1

#define EXAMPLE_TRDC_MBC                                  kTRDC_MAIN_MBC_GPIO0
#define EXAMPLE_TRDC_MBC_ACCESS_CONTROL_POLICY_ALL_INDEX  0
#define EXAMPLE_TRDC_MBC_ACCESS_CONTROL_POLICY_NONE_INDEX 1

#define FSL_FEATURE_TRDC_HAS_MBC (1)
#define FSL_FEATURE_TRDC_HAS_MRC (1)
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
void APP_CheckAndResolveMbcAccessError(trdc_domain_error_t *error);
void APP_CheckAndResolveMrcAccessError(trdc_domain_error_t *error);
/*${prototype:end}*/

#endif /* _APP_H_ */
