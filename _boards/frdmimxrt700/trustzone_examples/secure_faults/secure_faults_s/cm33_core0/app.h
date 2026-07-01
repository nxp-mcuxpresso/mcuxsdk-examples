/*
 * Copyright 2026 NXP
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define AHB_SECURE_CTRL    AHBSC0
#define AHB_LAYERS_COUNT   AHBSC0_SEC_VIO_ADDRN_COUNT
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
#ifdef BOARD_TZM_SG_IN_SRAM
void BOARD_SystemInitHook(void);
#endif /* BOARD_TZM_SG_IN_SRAM */
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
