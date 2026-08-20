/*
 * Copyright 2024-2026 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __APP_H__
#define __APP_H__

#ifdef MCMGR_USED
#include "mcmgr.h"
#endif /* MCMGR_USED */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define RPMSG_LITE_LINK_ID              (RL_PLATFORM_IMXRT700_M33_0_M33_1_LINK_ID)
#define RPMSG_LITE_NS_ANNOUNCE_STRING   "rpmsg-virtual-env-channel"
#ifndef CORE1_BOOT_ADDRESS
#define CORE1_BOOT_ADDRESS 0x20600000
#endif

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

void BOARD_InitHardware(void);

#endif /* __APP_H__ */
