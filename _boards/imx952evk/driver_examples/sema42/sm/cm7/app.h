/*
 * Copyright 2023 NXP
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
#define APP_CORTEX_M7_DID     9
#define APP_CORTEX_M33_DID     8
#define APP_SEMA42_GATE_ADDR 0x44260003 /* Address of the SEMA42 gate. */

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
