/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/

#define IPC_SHM_MU            NETC__MUA1__MUB
#define IPC_SHM_MU_IRQn       MU_E1_B_IRQn
#define IPC_SHM_MU_IRQHandler MU_E1_B_IRQHandler

/*${macro:end}*/
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
