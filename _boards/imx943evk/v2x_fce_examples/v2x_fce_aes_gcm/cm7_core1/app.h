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
 * V2X/FCE message unit base for this core.
 *
 * There is no fixed SDK base-address macro for the V2X/FCE (V2X-FH) MU on
 * i.MX943. The Linux device tree places this MU (v2x_mu2) at 0x47320000, so
 * that address is used here. Adjust to match the XRDC assignment for the core
 * running this demo.
 */
#define EXAMPLE_V2X_FCE_MU_BASE (0x47320000u)
#define V2X_FCE_MU              ((MU_Type *)EXAMPLE_V2X_FCE_MU_BASE)

/*
 * EdgeLock enclave (ELE) message unit base.
 *
 * On i.MX943 the V2X firmware core is not auto-started: before the V2X/FCE MU
 * (V2X_FCE_MU) will answer, the ELE must authenticate/release the V2X firmware.
 * That bring-up handshake (ELE_V2X_FW_AUTH_REQ 0xB1 + ELE_GET_STATE 0xB2) rides
 * the ELE MU, which board.h exposes as SOC_ELE_MU_INST_BASE (0x47550000).
 */
#ifndef EXAMPLE_ELE_MU_BASE
#define EXAMPLE_ELE_MU_BASE (SOC_ELE_MU_INST_BASE)
#endif
#define ELE_MU ((S3MU_Type *)EXAMPLE_ELE_MU_BASE)

/*
 * V2X (debug) message unit base.
 *
 * The V2X firmware version query (V2X_Base_GetFwVersion) uses the V2X debug MU,
 * which the Linux DTS places at 0x47350000 (v2x_mu). This is a different MU from
 * the V2X/FCE fast-crypto MU used for the AES-GCM operations.
 */
#ifndef EXAMPLE_V2X_MU_BASE
#define EXAMPLE_V2X_MU_BASE (0x47350000u)
#endif
#define V2X_MU ((MU_Type *)EXAMPLE_V2X_MU_BASE)

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
