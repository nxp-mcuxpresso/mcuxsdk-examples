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
#define EXAMPLE_TRDC_INSTANCE TRDC1
/* TRDC1 base address: 4427_0000h  AONMIX */
/* Location  |  MBC   | MBC Instance | Number of | Slave Port   | Slave Memory  *
 * Number    |        | Number       | Slavers   |              |               *
 * -----------------------------------------------------------  ----------------*
 * AONMIX    | MBC_A0 | 0            | 3         | SLV  2       | GPIO1         */

/* Location  |  MRC   | MRC Instance | Number of | Slave Memory | Number of MRC *
 * Number    |        | Number       | Slavers   |              | descriptors   *
 * -----------------------------------------------------------------------------*
 * AONMIX    | MRC_A0 | 0            | 1         | CM33 ROM     | 8             */
#define EXAMPLE_TRDC_DOMAIN_INDEX 2

#define EXAMPLE_TRDC_MRC_INDEX                            0
#define EXAMPLE_TRDC_MRC_REGION_INDEX                     0
#define EXAMPLE_TRDC_MRC_ACCESS_CONTROL_POLICY_ALL_INDEX  0
#define EXAMPLE_TRDC_MRC_ACCESS_CONTROL_POLICY_NONE_INDEX 2

#define EXAMPLE_TRDC_MBC_INDEX                            0
#define EXAMPLE_TRDC_MBC_SLAVE_INDEX                      2
#define EXAMPLE_TRDC_MBC_MEMORY_INDEX                     0 /* GIPIO1 */
#define EXAMPLE_TRDC_MBC_ACCESS_CONTROL_POLICY_ALL_INDEX  0
#define EXAMPLE_TRDC_MBC_ACCESS_CONTROL_POLICY_NONE_INDEX 2

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
/*${prototype:end}*/

#endif /* _APP_H_ */
