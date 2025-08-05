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

#define IPC_SHM_MU            NETC__MUA1__MUA
#define IPC_SHM_MU_IRQn       MU_E1_A_IRQn
#define IPC_SHM_MU_IRQHandler MU_E1_A_IRQHandler
#define ECAT_CHANNEL            SCMI_A2P
#define ECAT_DOMAIN_ID          24
#define ECAT_DISABLE_FLAGS      0x2
#define ECAT_ENABLE_FLAGS       0x0
#define ECAT_REST_STATE         0


/*${macro:end}*/
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
