/*
 * Copyright 2019-2021 NXP
 * All rights reserved.
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
#define AHB_LAYERS_COUNT 18U
#if (DEMO_CODE_START_NS == 0x08100000U)
#define BOARD_InitTrustZone XIP_BOARD_InitTrustZone
#else
#define BOARD_InitTrustZone RAM_BOARD_InitTrustZone
#endif
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
