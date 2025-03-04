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
#define RPMSG_LITE_LINK_ID (RL_PLATFORM_IMXRT700_M33_0_HIFI4_LINK_ID)
#define RPMSG_LITE_SHMEM_BASE (void *)(0x20200000 + 4096U)
#define MCMGR_USED

#define APP_CM_CORE kMCMGR_Core0
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
