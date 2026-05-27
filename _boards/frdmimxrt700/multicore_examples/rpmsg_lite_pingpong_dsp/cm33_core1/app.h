/*
 * Copyright 2025, 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "mcmgr.h"
#include "dsp_support.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define RPMSG_LITE_LINK_ID_CM (RL_PLATFORM_IMXRT700_M33_0_M33_1_LINK_ID)
#define RPMSG_LITE_SHMEM_BASE_CM (void *)0x20200000
#define RPMSG_LITE_SHMEM_BASE_DSP (void *)0x20588000
#if defined(CONFIG_RPMSG_LITE_PINGPONG_SWITCH_COMM) && (CONFIG_RPMSG_LITE_PINGPONG_SWITCH_COMM == 1)
#define RPMSG_LITE_LINK_ID_DSP (RL_PLATFORM_IMXRT700_M33_1_HIFI4_LINK_ID)
/* Communication Address used in MCMGR start to send address to other Core
 * It start Hifi1 core, and it will send it different address based on CONFIG_RPMSG_LITE_PINGPONG_SWITCH_COMM
 * because Hifi1 will use one address to communicate with CM33_0 and other with CM33_1
 */
#define RPMSG_LITE_SHMEM_BASE_COMM (void *)(0x20200000 + 4096U)
#define APP_COMM_CORE kMCMGR_Core2
#else
#define RPMSG_LITE_LINK_ID_DSP (RL_PLATFORM_IMXRT700_M33_1_HIFI1_LINK_ID)
#define APP_COMM_CORE APP_DSP_CORE
#define RPMSG_LITE_SHMEM_BASE_COMM RPMSG_LITE_SHMEM_BASE_DSP
#endif
#define MCMGR_USED

#define APP_EVENT_SIZE (4)
#define APP_CM_CORE kMCMGR_Core0
#define APP_DSP_CORE kMCMGR_Core3
/*${macro:end}*/
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
