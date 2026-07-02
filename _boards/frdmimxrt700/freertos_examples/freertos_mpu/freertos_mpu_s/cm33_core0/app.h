/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define mainNONSECURE_APP_START_ADDRESS DEMO_CODE_START_NS
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
