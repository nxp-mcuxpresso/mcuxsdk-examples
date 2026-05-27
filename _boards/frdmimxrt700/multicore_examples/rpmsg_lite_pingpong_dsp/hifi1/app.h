/*
 * Copyright 2025, 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#if defined(CONFIG_RPMSG_LITE_PINGPONG_SWITCH_COMM) && (CONFIG_RPMSG_LITE_PINGPONG_SWITCH_COMM == 1)
#define RPMSG_LITE_LINK_ID    (RL_PLATFORM_IMXRT700_M33_0_HIFI1_LINK_ID)
#define APP_COMM_CORE kMCMGR_Core0
#else
#define RPMSG_LITE_LINK_ID    (RL_PLATFORM_IMXRT700_M33_1_HIFI1_LINK_ID)
#define APP_COMM_CORE APP_CM_CORE
#endif
#define RPMSG_LITE_SHMEM_BASE (void *)0x20588000
#define MCMGR_USED

#define APP_CM_CORE kMCMGR_Core1
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
