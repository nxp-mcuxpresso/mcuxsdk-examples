/*
 * Copyright 2021 NXP
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
#define DEMO_PDB_BASE       PDB0
#define DEMO_PDB_IRQ_ID     PDB0_IRQn
#define DEMO_PDB_IRQHandler ivINT_PDB0
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void DEMO_PDB_IRQHandler(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
