/*
 * Copyright 2022 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define APP_SEMA42           SEMA42_1
#define APP_SEMA42_GATE      0
#define APP_CORTEX_M_DID     0
#define APP_CORTEX_A_DID     7
#define APP_SEMA42_GATE_ADDR 0x44260003 /* Address of the SEMA42 gate. */

#define EXAMPLE_TRDC_MRC_ACCESS_CONTROL_POLICY_ALL_INDEX  0
#define EXAMPLE_TRDC_MRC_ACCESS_CONTROL_POLICY_NONE_INDEX 1
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void APP_InitDomainConfig(void);
void APP_DeinitDomainConfig(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
