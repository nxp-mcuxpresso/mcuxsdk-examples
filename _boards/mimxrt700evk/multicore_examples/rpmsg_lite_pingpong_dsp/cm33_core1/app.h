/*
 * Copyright 2025 NXP
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
#define RPMSG_LITE_LINK_ID_DSP (RL_PLATFORM_IMXRT700_M33_1_HIFI1_LINK_ID)
#define RPMSG_LITE_SHMEM_BASE_CM (void *)0x20200000
#define RPMSG_LITE_SHMEM_BASE_DSP (void *)0x20588000
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
