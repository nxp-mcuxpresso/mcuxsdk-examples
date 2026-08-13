/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "board.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
/*
 * V2X message unit base for this core.
 *
 * The V2X base ops bind to SE_TYPE_ID_V2X_DBG in Linux (drivers/firmware/imx/
 * se_ctrl.c). In imx94.dtsi that is the v2x_dbg node using mboxes = <&v2x_mu>,
 * and v2x_mu is mailbox@47350000, so this MU base is used directly on all cores.
 */
#define EXAMPLE_V2X_MU_BASE (0x47350000u)
#define V2X_MU              ((MU_Type *)EXAMPLE_V2X_MU_BASE)

/*
 * EdgeLock enclave (ELE) message unit base for the V2X firmware bring-up
 * handshake (ELE_V2X_FW_AUTH_REQ 0xB1 + ELE_GET_STATE 0xB2). board.h exposes it
 * as SOC_ELE_MU_INST_BASE (0x47550000).
 */
#ifndef EXAMPLE_ELE_MU_BASE
#define EXAMPLE_ELE_MU_BASE (SOC_ELE_MU_INST_BASE)
#endif
#define ELE_MU ((S3MU_Type *)EXAMPLE_ELE_MU_BASE)

/* DDR address of the V2X firmware image the ELE authenticates. */
#ifndef EXAMPLE_V2X_FW_IMG_DDR_ADDR
#if defined(V2X_FW_IMG_DDR_ADDR)
#define EXAMPLE_V2X_FW_IMG_DDR_ADDR (V2X_FW_IMG_DDR_ADDR)
#else
#define EXAMPLE_V2X_FW_IMG_DDR_ADDR (0x8b000000u)
#endif
#endif
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
