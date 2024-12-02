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
#define APP_SEMA42           SEMA42_0
#define APP_SEMA42_GATE      0
#define APP_CORTEX_M_DID     2
#define APP_CORTEX_A_DID     1
#define APP_SEMA42_GATE_ADDR 0x4101b003 /* Address of the SEMA42 gate. */

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
